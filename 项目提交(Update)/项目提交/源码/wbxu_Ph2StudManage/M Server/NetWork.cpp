// NetWork.cpp: implementation of the CNetWork class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Server.h"
#include "NetWork.h"

#include "RecvFromWorkItem.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/************************************************************************/
/* M Server��Խ������ݵĴ���                                          */
/************************************************************************/

static CMainFrame *gs_pMainFrame = NULL;
static sockaddr_in gs_addrTO;
static sockaddr gs_oriaddr;      //null is ok, just a pad

//thz are just examples, u can add more
static char *gs_szCheckItem[] = {
            "select",
            "delete",
            "insert",
            "update",
            ">",
            "=",
            "and",
            " or ",
            ";",
            /*
            add your check item here
            */
            NULL
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNetWork::CNetWork()
{
    LoadServInfo();
}

CNetWork::~CNetWork()
{

}

/************************************************************************/
/* �������ļ��м���server��ip, port��Ϣ                                 */
/************************************************************************/
BOOL CNetWork::LoadServInfo()
{
    CString strConfigFile;
    ::GetCurrentDirectory(MAX_PATH, 
                        strConfigFile.GetBufferSetLength(MAX_PATH));
    strConfigFile.ReleaseBuffer();
    strConfigFile.Format("%s\\%s", strConfigFile, CONFIG_FILE);
    
    TCHAR szBuff[MAX_PATH];

    //M Server info
    DWORD dwRet = ::GetPrivateProfileString(M_SECTION_NAME, 
                                            KEY_IP, 
                                            "127.0.0.1", 
                                            szBuff, 
                                            MAX_PATH, 
                                            strConfigFile);
    
    m_strMIP = szBuff;
    
    m_uhMPort = ::GetPrivateProfileInt(M_SECTION_NAME, 
                                        KEY_PORT, 
                                        8642, 
                                        strConfigFile);

    //Server info
    dwRet = ::GetPrivateProfileString(SECTION_NAME, 
                                            KEY_IP, 
                                            "127.0.0.1", 
                                            szBuff, 
                                            MAX_PATH, 
                                            strConfigFile);
    
    m_strIP = szBuff;
    
    m_uhPort = ::GetPrivateProfileInt(SECTION_NAME, KEY_PORT, 9753, strConfigFile);

    //���ڷ���˵�ַ�Ĵ���
    gs_addrTO.sin_family            = AF_INET;      
    gs_addrTO.sin_port              = htons(m_uhPort);  
    gs_addrTO.sin_addr.S_un.S_addr  = inet_addr(m_strIP);
  
    return TRUE;
}

//forward declare
static UINT ProcessRecvFromThread(LPVOID pParam);

/************************************************************************/
/* ������clients�ķ���
ͬʱ�������������̣߳��Լ��̳߳�  
�Լ���Serverȡ����ϵ                                                 */
/************************************************************************/
void CNetWork::OnStartService() 
{
    BOOL bRet = m_MySocket.Create_UDP();
    if (!bRet)
    {
        m_MySocket.GetError();
        return;
    }
    
    bRet = m_MySocket.Bind(m_strMIP, m_uhMPort);
    if (!bRet)
    {
        m_MySocket.GetError();
        return;
    }
    
    //�̳߳�
    m_ThreadPool.Create();

    /*
    Ԥ�Ƚ���sendto, �Զ�recvfrom ����block
    //For client applications using this function,
    the socket can become bound implicitly to a local address 
    through sendto, WSASendTo, or WSAJoinLeaf.
    */
    //for (int i = 0; i < 1000; i++)
    CMyPacket::NotifyInfo( INFO_TYPE_UDP_INIT,
                            "Hi, I'm M Server, going to talk with you",
                            m_MySocket.m_Socket,
                            gs_oriaddr,
                            (sockaddr *)&gs_addrTO,
                            sizeof(sockaddr));

    //�½��߳��Զ�Server / Client��Ϣ���н���
    AfxBeginThread(ProcessRecvFromThread, this);

    //
    gs_pMainFrame = reinterpret_cast<CMainFrame*>(AfxGetMainWnd());
}

/************************************************************************/
/* ���������߳�
ÿ�����յ����ݺ󣬽�����ӵ��̳߳��н��д���                         */
/************************************************************************/
UINT ProcessRecvFromThread(LPVOID pParam)
{  
    CNetWork *pNet = (CNetWork *)pParam;
    ASSERT(pNet != NULL);
    
    SOCKET servSocket = pNet->m_MySocket.m_Socket;
    
#define MSGSIZE 1024
    char *pszBuf = new char[MSGSIZE];
    int  nBufLen = MSGSIZE;
    sockaddr_in addrin;
    int nLen     = sizeof(struct sockaddr_in);
    BOOL bRet    = TRUE;
    
    while (TRUE)
    {
        memset(pszBuf, 0, nBufLen);
        bRet = CMySocket::RecvFrom(servSocket,
                                    &pszBuf,
                                    &nBufLen,
                                    0,
                                    (struct sockaddr *)&addrin,
                                    &nLen
                                    );
        if (!bRet)
        {
            break;
        }

        //���̳߳ؽ��д���
        pNet->m_ThreadPool.AddWork(new CRecvFromWorkItem(pszBuf,
                                                         addrin,
                                                         pNet)
                                                         );
    }
    
    return 0;
}

/************************************************************************/
/* �Խ��յ������ݣ�����Ϣ���ͽ��зַ�
�������ӳؽ��д���                                                      */
/************************************************************************/
void CNetWork::ClassifyInfo(char *szBuff, struct sockaddr_in addrin)
{
    char *pData    = NULL;
    u_int nLen     = 0;
    u_int infoType = CMyPacket::GetType(szBuff, &pData, &nLen);    

    gs_pMainFrame->LogInfo(pData + sizeof(struct sockaddr));
    
    switch (infoType)
    {
    //////////////////////////////////////////////////////////////////////////
    //ack from server, secrets just between Server and M Server
    case INFO_TYPE_UDP_OK:      
        {
            gs_pMainFrame->SetConnected();  

            //we also prefetch table names
            CMyPacket::NotifyInfo( INFO_TYPE_GETTABLES,
                                    "hi, I'm M Server, I wanna prefetch table names",
                                    m_MySocket.m_Socket,
                                    gs_oriaddr,
                                    (sockaddr *)&gs_addrTO,
                                    sizeof(sockaddr));

            break;
        }

    case INFO_TYPE_TABLES:
        {
            //we just store thz, later replying for clients
            m_strTblNames = pData + sizeof(struct sockaddr);

            //also get the map between table name and columns
            GetColumns(m_strTblNames.GetBuffer(0));

            //now we also get alias, 
            CMyPacket::NotifyInfo( INFO_TYPE_GETALIAS,
                                    "hi, I'm M Server, I wanna prefetch alias names",
                                    m_MySocket.m_Socket,
                                    gs_oriaddr,
                                    (sockaddr *)&gs_addrTO,
                                    sizeof(sockaddr));

            break;
        }

    case INFO_TYPE_ALIAS:
        {
            //we just store thz, later for UI
            m_strAlias = pData + sizeof(struct sockaddr);
            
            break;
        }

    case INFO_TYPE_COLUMNS:
        {
            //establish the map between table and its colunms
            m_map_tbl_cols[pData] = pData + sizeof(struct sockaddr);

            gs_pMainFrame->LogInfo(pData);

            break;
        }

    //////////////////////////////////////////////////////////////////////////
    //req from client
    case INFO_TYPE_UDP_INIT:    //from client
        {
            ProcessUDPInit(pData, nLen, addrin);
            break;
        } 

    case INFO_TYPE_GETTABLES:   
        {
            //we do have, so share it.
            CMyPacket::NotifyInfo( INFO_TYPE_TABLES,
                                    m_strTblNames.GetBuffer(0), 
                                    m_MySocket.m_Socket,
                                    *(struct sockaddr *)&addrin,
                                    (struct sockaddr *)&addrin,
                                    sizeof(sockaddr));

            break;
        }
        
    case INFO_TYPE_GETCOLUMNS:
        {
            //already get from server, share it.
            CMyPacket::NotifyInfo(INFO_TYPE_COLUMNS,
                                  m_map_tbl_cols[pData].GetBuffer(0),    //a hack, within client u need to pad sockaddr with tbl
                                  m_MySocket.m_Socket,
                                  *(struct sockaddr *)pData,
                                  (struct sockaddr *)&addrin,
                                  sizeof(struct sockaddr)
                                  );
            break;
        }

    case INFO_TYPE_GETALIAS:
        {
            CMyPacket::NotifyInfo(INFO_TYPE_ALIAS,
                                    m_strAlias.GetBuffer(0),    
                                    m_MySocket.m_Socket,
                                    *(struct sockaddr *)pData,
                                    (struct sockaddr *)&addrin,
                                    sizeof(struct sockaddr)
                                  );
            break;
        }

    case INFO_TYPE_GETUPDATETIME:
        {
            CMyPacket::NotifyInfo(INFO_TYPE_GETUPDATETIME,
                                    "",
                                    m_MySocket.m_Socket,
                                    *(struct sockaddr *)&addrin,  //a hack, let server talk to client directly
                                    (struct sockaddr *)&gs_addrTO,
                                    sizeof(struct sockaddr)
                                  );

            break;
        }

    case INFO_TYPE_REQUEST:
        {
            ProcessSQLReq(pData, addrin);
            break;
        }
    
    /////////////////////////////////////////////////////////////////////////
    // reply from Server to Clients
    // now take care of this, the origin of the request
    // we just let the server reply directly to client with the help of sockaddr.  (if possible)

    //////////////////////////////////////////////////////////////////////////
    default:
        {
            gs_pMainFrame->LogInfo("Unknow Info Type");
            break;
        }
    }
}

void CNetWork::OnStopService()
{
    m_MySocket.Close();   
    
    m_ThreadPool.Destroy();
}

BOOL CNetWork::ProcessUDPInit(char *pRequest, int nLen, struct sockaddr_in addrin)
{
    //reply to client
    CString strIPPort;
    CMySocket::GetAddress(&addrin, strIPPort);
    gs_pMainFrame->LogInfo(strIPPort);
    
    CMyPacket::NotifyInfo(INFO_TYPE_UDP_OK,
                         "ok!",
                         m_MySocket.m_Socket,
                         gs_oriaddr,
                         (struct sockaddr *)&addrin,
                         sizeof(addrin)
                         );

    return TRUE;
}

/************************************************************************/
/* �������ֶεĺϷ��Լ�飬ƴ�ӻ�ȡsql��䣬��ת����Server              */
/************************************************************************/
void CNetWork::ProcessSQLReq(char *pData, struct sockaddr_in addrin)
{
    //�����ݺϷ��Խ��м��
    if (!CheckLegal(pData + sizeof(struct sockaddr)))
    {
        CMyPacket::NotifyInfo(INFO_TYPE_REPLY,
                              "���ڷǷ��ֶ�!!!",
                              m_MySocket.m_Socket,
                              *(struct sockaddr *)&gs_oriaddr,
                              (struct sockaddr *)&addrin,
                              sizeof(struct sockaddr)
                              );
        return;
    }

    int nOpCode = *(int *)pData;    
    if (INFO_TYPE_SELECT == nOpCode)
    {
        ProcessSQLSelect(pData, addrin);
    }
    else if (INFO_TYPE_INSERT == nOpCode)
    {
        ProcessSQLInsert(pData, addrin);
    }
    else if (INFO_TYPE_DEL == nOpCode)
    {
        ProcessSQLDel(pData, addrin);
    }
    else if (INFO_TYPE_UPDATE == nOpCode)
    {
        ProcessSQLUpdate(pData, addrin);
    }
}

/************************************************************************/
/* ��ȡpszTblNames�и�table ���Ӧ���ֶ�    
pszTblNames looks like "t_class|t_course|t_teacher...                  */
/************************************************************************/
void CNetWork::GetColumns(char *pszTblNames)
{
    CString strTbls = pszTblNames; 
    CString strSub = _T("");
    CString strSQL = _T("");
    for (int i = 0; ; i++)
    {
        if (!AfxExtractSubString(strSub, strTbls, i, '|')
            || 0 == strSub.GetLength())
        {
            break;
        }

        //now get our hands dirty
        strSQL.Format("select * from %s where 1 = 0", strSub);
        CMyPacket::NotifyInfo(INFO_TYPE_GETCOLUMNS,
                              strSQL.GetBuffer(0),
                              m_MySocket.m_Socket,
                              *(struct sockaddr *)strSub.GetBuffer(0),  //a hack, we pad the sockaddr with ,
                              (struct sockaddr *)&gs_addrTO,
                              sizeof(struct sockaddr)
                              );
    }   
}

/************************************************************************/
/* ƴ�Ӳ�ѯ��䣬ת����Server                                          */
/************************************************************************/
void CNetWork::ProcessSQLSelect(char *pData, struct sockaddr_in addrin)
{
    CString strTbl = pData + sizeof(long);
    CString strReq = pData + sizeof(struct sockaddr);
    CString strSub = "";
    CString strSQL = "";
    CString strTmp = "";

    strSQL.Format("select * from %s where 1 = 1 ", strTbl);    
    for (int i = 0; ; i++)
    {
        if (!AfxExtractSubString(strSub, strReq, i, '|')
            || 0 == strSub.GetLength())
        {
            break;
        }
        
        if (i % 2 == 0)
        {
            strTmp.Format("and %s like '%%", strSub);
            strSQL += strTmp;
        }
        else
        {
            strTmp.Format("%s%%'", strSub);
            strSQL += strTmp;
        }
    }
    
    gs_pMainFrame->LogInfo(strSQL);
    
    //now the server's turn to finish this
    CMyPacket::NotifyInfo(INFO_TYPE_SELECT,
                        strSQL.GetBuffer(0),
                        m_MySocket.m_Socket,
                        *(struct sockaddr *)&addrin,  //a hack
                        (struct sockaddr *)&gs_addrTO,
                        sizeof(struct sockaddr)
                          );
}

/************************************************************************/
/* ƴ��insert��䣬ת��Serverִ��                                       */
/************************************************************************/
void CNetWork::ProcessSQLInsert(char *pData, struct sockaddr_in addrin)
{
    CString strTbl = pData + sizeof(long);
    CString strReq = pData + sizeof(struct sockaddr);
    CString strSub = "";
    CString strSQL = "";
    CString strTmp = "";
    
    strSQL.Format("insert %s values(", strTbl);    
    for (int i = 0; ; i++)
    {
        if (!AfxExtractSubString(strSub, strReq, i, '|')
            || 0 == strSub.GetLength())
        {
            break;
        }

        //�����ֶ���Ϣ
        if (0 == i % 2)
        {
            continue;
        }

        if (i != 1)
        {
            strSQL += ", ";
        }
        
        strTmp.Format("'%s'", strSub);
        strSQL += strTmp;
    }

    strSQL += ")";
    
    gs_pMainFrame->LogInfo(strSQL);
    
    //now the server's turn to finish this
    CMyPacket::NotifyInfo(INFO_TYPE_INSERT,
                        strSQL.GetBuffer(0),
                        m_MySocket.m_Socket,
                        *(struct sockaddr *)&addrin,  //a hack
                        (struct sockaddr *)&gs_addrTO,
                        sizeof(struct sockaddr)
                          );
}

/************************************************************************/
/* ƴ��delete��䣬ת��Serverִ��                                       */
/************************************************************************/
void CNetWork::ProcessSQLDel(char *pData, struct sockaddr_in addrin)
{
    CString strTbl = pData + sizeof(long);
    CString strReq = pData + sizeof(struct sockaddr);
    CString strSub = "";
    CString strSQL = "";
    CString strTmp = "";
    
    strSQL.Format("delete from %s where id = ", strTbl);  
    AfxExtractSubString(strSub, strReq, 1, '|');
    strSQL += strSub;    
    
    gs_pMainFrame->LogInfo(strSQL);
    
    //now the server's turn to finish this
    CMyPacket::NotifyInfo(INFO_TYPE_DEL,
                            strSQL.GetBuffer(0),
                            m_MySocket.m_Socket,
                            *(struct sockaddr *)&addrin,  //a hack
                            (struct sockaddr *)&gs_addrTO,
                            sizeof(struct sockaddr)
                          );  
}

/************************************************************************/
/* ƴ��update��䣬ת��Serverִ��                                      */
/************************************************************************/
void CNetWork::ProcessSQLUpdate(char *pData, sockaddr_in addrin)
{
    //perhaps be achieved by combination of del and insert
    CString strTbl = pData + sizeof(long);
    CString strReq = pData + sizeof(struct sockaddr);
    CString strSub = "";
    CString strSQL = "";
    CString strTmp = "";
    
    strSQL.Format("update %s set ", strTbl);    
    for (int i = 2; ; i++)  //just skip id|xx|
    {
        if (!AfxExtractSubString(strSub, strReq, i, '|')
            || 0 == strSub.GetLength())
        {
            break;
        }

        if (i % 2 == 0
            && i != 2)
        {
            strSQL += ",";
        }
        
        if (i % 2 == 0)
        {
            strTmp.Format("%s=", strSub);
            strSQL += strTmp;
        }
        else
        {
            strTmp.Format("'%s'", strSub);
            strSQL += strTmp;
        }
    }
    
    //get id
    AfxExtractSubString(strSub, strReq, 1, '|');
    strSQL += " where id = ";
    strSQL += strSub;
    
    gs_pMainFrame->LogInfo(strSQL);
    
    //now the server's turn to finish this
    CMyPacket::NotifyInfo(INFO_TYPE_INSERT,
                        strSQL.GetBuffer(0),
                        m_MySocket.m_Socket,
                        *(struct sockaddr *)&addrin,  //a hack
                        (struct sockaddr *)&gs_addrTO,
                        sizeof(struct sockaddr)
                          );
}

/*
Desc    :   �����ݺϷ��Խ��м�⣬����SQL Injection

Return  :   TRUE �Ϸ��� FALSE ���Ϸ�

            U can make it better
*/
BOOL CNetWork::CheckLegal(char *pData)
{
    for (char **pptr = gs_szCheckItem; *pptr != NULL; pptr++)
    {
        if (strstr(pData, *pptr))
        {
            return FALSE;
        }
    }
    
    return TRUE;
}


