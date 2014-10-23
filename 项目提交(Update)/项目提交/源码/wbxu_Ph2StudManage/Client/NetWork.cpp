// NetWork.cpp: implementation of the CNetWork class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Server.h"
#include "NetWork.h"

#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static CMainFrame *gs_pMainFrame = NULL;
static sockaddr_in gs_addrTO;
static sockaddr gs_oriaddr;  //null is ok, just a pad

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
    DWORD dwRet = ::GetPrivateProfileString(M_SECTION_NAME, 
                                            KEY_IP, 
                                            "127.0.0.1", 
                                            szBuff, 
                                            MAX_PATH, 
                                            strConfigFile);
    
    //do sth check
    m_strMIP = szBuff;
    
    m_uhMPort = ::GetPrivateProfileInt(M_SECTION_NAME, KEY_PORT, 8642, strConfigFile);

    //���ڷ���˵�ַ�Ĵ���
    gs_addrTO.sin_family = AF_INET;      
    gs_addrTO.sin_port = htons(m_uhMPort);  
    gs_addrTO.sin_addr.S_un.S_addr = inet_addr(m_strMIP);
  
    return TRUE;
}

//forward declare
UINT ProcessRecvFromThread(LPVOID pParam);

/************************************************************************/
/* ��M Serverȡ����ϵ                                                                     */
/************************************************************************/
void CNetWork::OnStartService() 
{
    BOOL bRet = m_MySocket.Create_UDP();
    if (!bRet)
    {
        m_MySocket.GetError();
        return;
    }

    /*
    Ԥ�Ƚ���sendto, �Զ�recvfrom ����block
    //For client applications using this function,
    the socket can become bound implicitly to a local address 
    through sendto, WSASendTo, or WSAJoinLeaf.
    */

    //for (int i = 0; i < 1000; i++)
    CMyPacket::NotifyInfo( INFO_TYPE_UDP_INIT,
                            "Hi, I'm Client, going to talk with you",
                            m_MySocket.m_Socket,
                            gs_oriaddr,
                            (sockaddr *)&gs_addrTO,
                            sizeof(sockaddr));

    //�½��߳��Զ�M Server��Ϣ���н���
    AfxBeginThread(ProcessRecvFromThread, this);

    //
    gs_pMainFrame = reinterpret_cast<CMainFrame*>(AfxGetMainWnd());
}

/************************************************************************/
/* ���������߳�
ÿ�����յ����ݺ󣬶�����з��ദ��                         */
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
    int nLen = sizeof(sockaddr_in);
    BOOL bRet = TRUE;
    
    while (TRUE)
    {
        memset(pszBuf, 0, nBufLen);
        bRet = CMySocket::RecvFrom(servSocket,
                                    &pszBuf,
                                    &nBufLen,
                                    0,
                                    (sockaddr *)&addrin,
                                    &nLen
                                    );
        if (!bRet)
        {
            break;
        }

        pNet->ClassifyInfo(pszBuf, addrin);
    }
    
    return 0;
}

/************************************************************************/
/* �Խ��յ������ݣ�����Ϣ���ͽ��з��ദ��
�����UI��                                                    */
/************************************************************************/
void CNetWork::ClassifyInfo(char *szBuff, sockaddr_in addrin)
{
    char *pData;
    u_int nLen = 0;
    u_int infoType = CMyPacket::GetType(szBuff, &pData, &nLen);  

    gs_pMainFrame->LogInfo(pData + sizeof(struct sockaddr));
    
    switch (infoType)
    {
    //////////////////////////////////////////////////////////////////////////
    //ack and reply from M Server
    case INFO_TYPE_UDP_OK:
        {
            gs_pMainFrame->SetConnected();

            //now we get alias
            CMyPacket::NotifyInfo( INFO_TYPE_GETALIAS,
                                    "hi, I'm Client, I wanna prefetch alias names",
                                    m_MySocket.m_Socket,
                                    gs_oriaddr,
                                    (sockaddr *)&gs_addrTO,
                                    sizeof(sockaddr)
                                    );

            
            //also table names
            CMyPacket::NotifyInfo( INFO_TYPE_GETTABLES,
                                    "Hi, I'm Client, want to know tables",
                                    m_MySocket.m_Socket,
                                    gs_oriaddr,             //just a pad
                                    (sockaddr *)&gs_addrTO,
                                    sizeof(sockaddr));
            break;
        } 
        
    case INFO_TYPE_TABLES:
        {   
            //Update UI
            gs_pMainFrame->SetTblNames(pData + sizeof(struct sockaddr), this);

            //also get columns of thz tables
            GetAllColumns(pData + sizeof(struct sockaddr));

            break;
        }

    case INFO_TYPE_COLUMNS:
        {
            m_map_tbl_cols[pData] = pData + sizeof(struct sockaddr);
            
            gs_pMainFrame->LogInfo(pData);
            
            break;
        }

    case INFO_TYPE_ALIAS:
        {
            SaveAlias(pData + sizeof(struct sockaddr));
            break;
        }

    //////////////////////////////////////////////////////////////////////////
    //ack and reply from Server directly
    case INFO_TYPE_REPLY_SELECT:
        {
            gs_pMainFrame->SetSelResults(pData + sizeof(struct sockaddr));
            break;
        }

    case INFO_TYPE_REPLY_SUCCESS:
        {
            AfxMessageBox("�ɹ�!");
            break;
        }
    
    case INFO_TYPE_REPLY_ERROR:
        {
            AfxMessageBox(pData + sizeof(struct sockaddr));
            break;
        }

    case INFO_TYPE_UPDATETIME:
        {
            gs_pMainFrame->SetUpdateTime(pData + sizeof(struct sockaddr));
            break;
        }
    } 
}

void CNetWork::OnStopService()
{
    m_MySocket.Close();   
}

/************************************************************************/
/* ��ȡ��tablename����Ӧ���ֶ�
UI�����ݵķ���                                                          */
/************************************************************************/
void CNetWork::GetAllColumns(char *pszTblNames)
{
    CString strTbls = pszTblNames; 
    CString strSub = _T("");

    for (int i = 0; ; i++)
    {
        if (!AfxExtractSubString(strSub, strTbls, i, '|')
            || 0 == strSub.GetLength())
        {
            break;
        }
        
        //now get our hands dirty
        CMyPacket::NotifyInfo(INFO_TYPE_GETCOLUMNS,
                                "Get cols",             //anything is ok, nonsence
                                m_MySocket.m_Socket,
                                *(struct sockaddr *)strSub.GetBuffer(0),  //a hack, we pad the sockaddr with ,
                                (sockaddr *)&gs_addrTO,
                                sizeof(sockaddr)
                                );
    }   
}

/*
InfoType  InfoLen  ReqType  TblName  Field|Values
*/
/************************************************************************/
/* ����ɾ���ġ�������
���������͡�tablename                                                */
/************************************************************************/
void CNetWork::SendRequest(CRequest *pReq)
{
    struct sockaddr pad = {NULL};
    memcpy(&pad, &pReq->m_Operation, sizeof(long));
    strcpy((char *)&pad + sizeof(long), pReq->m_strTbl.GetBuffer(0));

    CMyPacket::NotifyInfo(INFO_TYPE_REQUEST,
                           pReq->m_strReq.GetBuffer(0),
                           m_MySocket.m_Socket,
                            pad,   //a hack
                            (sockaddr *)&gs_addrTO,
                            sizeof(sockaddr)
                           );
}

/*
Function    :  establish the map between ori name and its alias
Param       :  pszBuff  sth like ori|ali|ori1|ali1|....
Return      :  None
Effects     :  map<CString, CString> m_map_ori_alias;  //ԭ���ֶ��� ---- ����
               map<CString, CString> m_map_alias_ori;  //����         -----ԭ���ֶ���
*/
void CNetWork::SaveAlias(char *pszBuff)
{
    CString strInfo = pszBuff; 
    CString strOri  = _T("");
    CString strAlias= _T("");
    
    int i = 0;
    for (i = 0; ; i += 2)
    {
        if (!AfxExtractSubString(strOri, strInfo, i, '|'))
        {
            break;
        }
        
        AfxExtractSubString(strAlias, strInfo, i + 1, '|');
        
        m_map_ori_alias[strOri]     = strAlias;
        m_map_alias_ori[strAlias]   = strOri;
    }
}

CString CNetWork::GetAlias(CString &strOri)
{
    return m_map_ori_alias[strOri];
}

CString CNetWork::GetOriName(CString &strAlias)
{
    return m_map_alias_ori[strAlias];
}

CString CNetWork::GetColsForTbl(CString &strTblName)
{
    return m_map_tbl_cols[strTblName];
}

void CNetWork::GetUpdateTime()
{
    CMyPacket::NotifyInfo(INFO_TYPE_GETUPDATETIME,
                        "",
                        m_MySocket.m_Socket,
                        *(struct sockaddr *)&gs_oriaddr, 
                        (struct sockaddr *)&gs_addrTO,
                        sizeof(sockaddr)
                        );  
}
