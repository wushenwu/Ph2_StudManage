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

static CMainFrame *gs_pMainFrame = NULL;
static struct sockaddr gs_oriaddr;   //null is ok, just a pad

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
    DWORD dwRet = ::GetPrivateProfileString(SECTION_NAME, 
                                            KEY_IP, 
                                            "127.0.0.1", 
                                            szBuff, 
                                            MAX_PATH, 
                                            strConfigFile);
    
    //do sth check
    m_strIP     = szBuff;
    m_uhPort    = ::GetPrivateProfileInt(SECTION_NAME, 
                                        KEY_PORT, 
                                        9753, 
                                        strConfigFile);
  
    return TRUE;
}

//forward declare
static UINT ProcessMServerThread(LPVOID pParam);

/************************************************************************/
/* ������clients�ķ���
ͬʱ�������������̣߳��Լ��̳߳�                                       */
/************************************************************************/
void CNetWork::OnStartService() 
{
    BOOL bRet = m_MySocket.Create_UDP();
    if (!bRet)
    {
        m_MySocket.GetError();
        return;
    }
    
    bRet = m_MySocket.Bind(m_strIP, m_uhPort);
    if (!bRet)
    {
        m_MySocket.GetError();
        return;
    }
    
    //�̳߳�
    m_ThreadPool.Create();
    
    //�½��߳��Զ�M Server��Ϣ���н���
    AfxBeginThread(ProcessMServerThread, this);

    //not a good habit doing like this
    gs_pMainFrame = reinterpret_cast<CMainFrame*>(AfxGetMainWnd());
}

/************************************************************************/
/* ���������߳�
ÿ�����յ����ݺ󣬽�����ӵ��̳߳��н��д���                         */
/************************************************************************/
UINT ProcessMServerThread(LPVOID pParam)
{  
    CNetWork *pNet = (CNetWork *)pParam;
    ASSERT(pNet   != NULL);
    
    SOCKET servSocket = pNet->m_MySocket.m_Socket;
    
#define MSGSIZE 1024
    char *pszBuf = new char[MSGSIZE];
    int  nBufLen = MSGSIZE;

    struct sockaddr_in addrin;
    int nLen  = sizeof(struct sockaddr_in);
    BOOL bRet = TRUE;
    
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
void CNetWork::ClassifyInfo(char *szBuff, sockaddr_in addrin)
{
    //�����ӳ��л�ȡ���õ����Ӷ���
    int nIndex                  = -1;
    static _ConnectionPtr pConn = NULL;
    pConn = gs_pMainFrame->m_DataBase.GetConnFromPool(&nIndex);
    if (NULL == pConn)
    {
        return;
    }

    char *pData     = NULL;
    u_int nLen      = 0;
    u_int infoType  = CMyPacket::GetType(szBuff, &pData, &nLen); 
    CString strRet  = "";

    gs_pMainFrame->LogInfo(pData + sizeof(struct sockaddr));
    
    switch (infoType)
    {
    case INFO_TYPE_UDP_INIT:
        {
            //no need for conn, so release it immediately
            gs_pMainFrame->m_DataBase.ReleaseConn(nIndex);

            ProcessUDPInit(pData, nLen, addrin);
            return;
        } 
        
    case INFO_TYPE_GETTABLES:   
        {
            CMyPacket::NotifyInfo(INFO_TYPE_TABLES,
                                  gs_pMainFrame->m_DataBase.GetTblNames(),
                                  m_MySocket.m_Socket,
                                  *(struct sockaddr *)pData, //�������Ϣԭ�ⲻ���ػ�Ӧ
                                  (struct sockaddr *)&addrin,
                                  sizeof(struct sockaddr)
                                  );
            break;
        }

    case INFO_TYPE_GETALIAS:   
        {
            CMyPacket::NotifyInfo(INFO_TYPE_ALIAS,
                                gs_pMainFrame->m_DataBase.GetAlias(),
                                m_MySocket.m_Socket,
                                *(struct sockaddr *)pData,   //�������Ϣԭ�ⲻ���ػ�Ӧ
                                (struct sockaddr *)&addrin,
                                sizeof(struct sockaddr)
                                );
            break;
        }

    case INFO_TYPE_GETCOLUMNS:
        {
            ProcessSelect(pData, pConn, addrin, TRUE, strRet);

            CMyPacket::NotifyInfo(
                                INFO_TYPE_COLUMNS,
                                strRet.GetBuffer(0),    
                                m_MySocket.m_Socket,
                                *(sockaddr *)pData,   //a hack, indicate send to which client
                                (sockaddr *)&addrin,
                                  sizeof(sockaddr)
                                );

            break;
        }

    case INFO_TYPE_GETUPDATETIME:
        {
            CMyPacket::NotifyInfo(
                                INFO_TYPE_UPDATETIME,
                                gs_pMainFrame->m_DataBase.GetUpdateTime().GetBuffer(0),    
                                m_MySocket.m_Socket,
                                *(sockaddr *)pData,   //a hack, indicate send to which client 
                                (struct sockaddr *)pData, //ֱ�ӽ��������pData����ʶ��clients
                                sizeof(sockaddr)
                                );
            break;
        }
        
    case INFO_TYPE_SELECT:
        {
            ProcessSelect(pData, pConn, addrin, FALSE, strRet);
            
            CMyPacket::NotifyInfo(INFO_TYPE_REPLY_SELECT,
                                  strRet.GetBuffer(0),
                                  m_MySocket.m_Socket,
                                  *(struct sockaddr *)pData, //pData�а�����clients��Ϣ
                                  (struct sockaddr *)pData, //ֱ�ӽ��������pData����ʶ��clients
                                  sizeof(struct sockaddr)
                                  );

            //we also send the updatetime
            CMyPacket::NotifyInfo(
                                INFO_TYPE_UPDATETIME,
                                gs_pMainFrame->m_DataBase.GetUpdateTime().GetBuffer(0),    
                                m_MySocket.m_Socket,
                                *(sockaddr *)pData,   //a hack, indicate send to which client 
                                (struct sockaddr *)pData, //ֱ�ӽ��������pData����ʶ��clients
                                sizeof(sockaddr)
                                );

            break;
        }

    //����Щ����ͬ���Ĵ���
    case INFO_TYPE_DEL:
    case INFO_TYPE_INSERT:
    case INFO_TYPE_UPDATE:
        {
            BOOL bSucceed = FALSE;
            ProcessModify(pData, pConn, addrin, bSucceed, strRet);

            CMyPacket::NotifyInfo(bSucceed ? INFO_TYPE_REPLY_SUCCESS : INFO_TYPE_REPLY_ERROR,
                                  strRet.GetBuffer(0),
                                  m_MySocket.m_Socket,
                                  *(struct sockaddr *)pData,
                                  (struct sockaddr *)pData, //ֱ�ӽ��������pData����ʶ��clients
                                  sizeof(struct sockaddr)
                                  );

            //used for cache
            gs_pMainFrame->m_DataBase.RefreshUpdateTime();

            break;
        }

    default:
        {
            gs_pMainFrame->LogInfo("Unknow Info Type!!");
            break;
        }
    }

RELEASE:
    //then release the conn 
    gs_pMainFrame->m_DataBase.ReleaseConn(nIndex);
}

void CNetWork::OnStopService()
{
    m_MySocket.Close();   
    
    m_ThreadPool.Destroy();
}

/************************************************************************/
/* ����������Ļ�Ӧ                                                     */
/************************************************************************/
BOOL CNetWork::ProcessUDPInit(char *pRequest, int nLen, sockaddr_in addrin)
{
    CString strIPPort;
    CMySocket::GetAddress(&addrin, strIPPort);
    gs_pMainFrame->LogInfo(strIPPort);
    
    CMyPacket::NotifyInfo(INFO_TYPE_UDP_OK,
                         "OK!",
                         m_MySocket.m_Socket,
                         gs_oriaddr,
                         (sockaddr *)&addrin,
                         sizeof(addrin)
                         );

    return TRUE;
}

/************************************************************************/
/* �����ݿ��в�ѯ������ݣ�
1) bColumns Ϊtrue����ʾ�����ȡ�ֶ���Ϣ
2) bColumns Ϊfalse, ���ȡ���������ļ�¼   

ֱ�ӽ������Ϣ������clients (addrin����ʶ��                            */
/************************************************************************/
void CNetWork::ProcessSelect(char *pData,
                             _ConnectionPtr pConn, 
                             struct sockaddr_in addrin,
                             BOOL bColumns,
                             CString &strRet
                             )

{
    CString strPart;

    try 
    {
        //exec the command
        _RecordsetPtr rs = pConn->Execute(pData + sizeof(struct sockaddr), NULL, adConnectUnspecified);
        long lCount      = rs->Fields->GetCount();
        long i = 0;
        
        //for INFO_TYPE_GETCOLUMNS
        if (bColumns)
        {
            while (i < lCount)
            {
                strPart = (char*)(_bstr_t)rs->Fields->Item[i]->Name;                   
                strRet += strPart;
                strRet += "|";

                i++;
            }
        }
        //for INFO_TYPE_SELECT, request from Clients
        else
        {
            while (!rs->adoEOF)
            {
                for (i = 0; i < lCount; i++)
                {
                    strPart = (char*)(_bstr_t)rs->Fields->Item[i]->Value;                   
                    strRet += strPart;
                    strRet += "|";
                }

                rs->MoveNext();
            }
        }
    }
    catch(_com_error& e)
    {
        gs_pMainFrame->LogInfo(GetErrorDescription(e));
    }
    catch(...)
    {	
        gs_pMainFrame->LogInfo("Unknown Error Occured");
	}
    
    gs_pMainFrame->LogInfo(strRet);
}

/************************************************************************/
/* ��ȡ���ݿ�������                                                   */
/************************************************************************/
CString CNetWork::GetErrorDescription(_com_error& e)
{
    _bstr_t bstrSource(e.Source());
    _bstr_t bstrDescription(e.Description());
    _TCHAR szTemp[1024];
    
    CString strInfo ;
    wsprintf(szTemp, _T("Message : %s\n"), e.ErrorMessage());
    strInfo = szTemp;
    wsprintf(szTemp, _T("Code : 0x%08lx\n"), e.Error());
    strInfo += szTemp;
    wsprintf(szTemp, _T("Source : %s\n"), bstrSource.length() ? (LPCTSTR)bstrSource : _T("null"));
    strInfo += szTemp;
    wsprintf(szTemp, _T("Description : %s\n"), bstrDescription.length() ? (LPCTSTR)bstrDescription : _T("null"));
    strInfo += szTemp;
    
    return strInfo;
}

/************************************************************************/
/* ��Insert, Delete, Update�ļ��д���
ֱ�ӽ������Ϣ������clients (addrin����ʶ��                                                                     */
/************************************************************************/
void CNetWork::ProcessModify(char *pData, 
                             _ConnectionPtr pConn, 
                             sockaddr_in addrin, 
                             BOOL &bSucceed,
                             CString &strRet
                             )
{  
    bSucceed = FALSE;
    try 
    {
        //exec the command
        _RecordsetPtr rs = pConn->Execute(pData + sizeof(struct sockaddr), NULL, adConnectUnspecified);
        bSucceed = TRUE;
    }
    catch(_com_error& e)
    {
        _bstr_t bstrDescription(e.Description());
        strRet = (LPCTSTR)bstrDescription;       
    }
    catch(...)
    {	
        strRet = "Unknown Error Occured";
    }
    
    gs_pMainFrame->LogInfo(strRet);  
}
