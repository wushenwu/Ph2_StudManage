// DataBase.cpp: implementation of the CDataBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Server.h"
#include "DataBase.h"

#include "MainFrm.h"

#include <objbase.h>    //for CoInitialize

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/************************************************************************/
/* 对数据库操作的封装，连接池等                                         */
/************************************************************************/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataBase::CDataBase()
{
    ::CoInitialize( NULL );	

    m_pConn  = NULL;
    memset(m_pConnPool, 0, sizeof(m_pConnPool));
    memset(m_hEvents, 0, sizeof(m_hEvents));

    m_strTblNames   = "";
    m_strAlias      = "";
    m_strUpdateTime = "";
}

CDataBase::~CDataBase()
{
    FlushUpdateTime();

    //clean up
    for (int i = 0; i < CONNECTION_POOL_CAPACITY; i++)
    {
        if (m_pConnPool[i] != NULL)
        {    
            m_pConnPool[i]->Close();
            m_pConnPool[i] = NULL;
        }

        if (m_hEvents[i] != NULL)
        {
            CloseHandle(m_hEvents[i]);
            m_hEvents[i] = NULL;
        }
    }
    
    if (m_pConn != NULL)
    {
        m_pConn->Close();
        m_pConn = NULL;
    }

    ::CoUninitialize(); 
}

/************************************************************************/
/* 连接数据库，使用类似*.udl 导航的风格                                */
/************************************************************************/
BOOL CDataBase::ConnectDB()
{
    using namespace DBLink;

#define  AUTO
#ifdef AUTO
    
    IDataSourceLocatorPtr dlPrompt = NULL;
    HRESULT hRet = dlPrompt.CreateInstance(__uuidof(DataLinks));
    if (FAILED(hRet))
    {
        return FALSE;
    }
    
    m_pConn = dlPrompt->PromptNew();
    if (NULL == m_pConn)
    {
        return FALSE;
    }

    try 
    {
        m_pConn->Open(m_pConn->ConnectionString, "", "", adConnectUnspecified);
    }
    catch (_com_error e)
    {
        AfxMessageBox((char *)e.Description());	
    }   

#else
    #define DBCONNSTR "Provider=SQLOLEDB.1;\
                        Integrated Security=SSPI;\
                        Persist Security Info=False;\
                        Initial Catalog=Ph2_Project;\
                        Data Source=PC-201401211616"

    try 
    {
        m_pConn.CreateInstance("ADODB.Connection");
        m_pConn->Open(DBCONNSTR, "", "", adConnectUnspecified);
    }
    catch (_com_error e)
    {
        AfxMessageBox((char *)e.Description());	
    }   
#endif

    //
    LoadTblNames();
    
    return TRUE;
}

/************************************************************************/
/* 从数据库中加载用户表名，进行内部保存，服务于后续的clients请求       */
/************************************************************************/
BOOL CDataBase::LoadTblNames()
{
    LPCTSTR sql = _T("select name from sysobjects where type = 'U'\
                        and name <> 'dtproperties'\
                        and name <> 't_alias'");
    BOOL bRet   = TRUE;
    CString strTbl;
    
    try
    {
        _RecordsetPtr  rs = m_pConn->Execute(sql, NULL, adConnectUnspecified);
        
        while (!rs->adoEOF)
        {
            strTbl = (char*)(_bstr_t)rs->Fields->Item[(long)0]->Value;
            m_strTblNames += strTbl;
            m_strTblNames += "|";
            
            rs->MoveNext();
        }
    }
    catch (_com_error e)
    {
        AfxMessageBox((char *)e.Description());	
        
        bRet = FALSE;
    }
        
    return bRet;
}

/************************************************************************/
/* 反馈table names, 用户clients的UI显示                                 */
/************************************************************************/
char *CDataBase::GetTblNames()
{
    return m_strTblNames.GetBuffer(0);
}

/************************************************************************/
/* 创建连接池
将连接对象与同步对象进行绑定，以确定连接对象的可用性                    */
/************************************************************************/
BOOL CDataBase::CreateConnectionPool()
{
    BOOL bRet = TRUE;
    for (int i = 0; i < CONNECTION_POOL_CAPACITY; i++)
    {
        try 
        {
            m_pConnPool[i].CreateInstance("ADODB.Connection");         
            m_pConnPool[i]->Open(m_pConn->ConnectionString, "", "", adConnectUnspecified);

            //to justify the availability of ConnPool
            m_hEvents[i] = ::CreateEvent(NULL, FALSE, TRUE, NULL);
            if (NULL == m_hEvents[i])
            {
                bRet = FALSE;
                break;
            }
        }
        catch(_com_error e)
        {
            AfxMessageBox((char *)e.Description());	
            
            bRet = FALSE;
            break;
        }
    }//for 
    
    if (bRet)
    {
        return TRUE;
    }
    
    //clean up
    for (int j = 0; j < i; j++)
    {
        m_pConnPool[j]->Close();
        m_pConnPool[j] = NULL;

        ::CloseHandle(m_hEvents[j]);
        m_hEvents[j] = NULL;
    }
     
    return FALSE;
}

/*
Desc    : get an available conn from pool to execute command
Param   : pIndex used to indicate which one is available.
Return  : return CADORecordset * if available, and set the index
          otherwise NULL.
Comments: Be care to check the return value.
*/
_ConnectionPtr CDataBase::GetConnFromPool(int *pIndex)
{
    DWORD dwRet = ::WaitForMultipleObjects(
                                CONNECTION_POOL_CAPACITY, 
                                m_hEvents, 
                                FALSE, 
                                INFINITE);

    if (WAIT_FAILED == dwRet)
    {
        return NULL;
    }

    *pIndex = dwRet - WAIT_OBJECT_0;
    return m_pConnPool[*pIndex];
}

/************************************************************************/
/* 释放之前所申请的index所对应的连接对象                                */
/************************************************************************/
void CDataBase::ReleaseConn(int index)
{
    ASSERT(index >= 0 && index < CONNECTION_POOL_CAPACITY);

    SetEvent(m_hEvents[index]);
}

/************************************************************************/
/* 从t_alias中获取表、字段的别名    
这里也维护了UpdateTime信息                                              */
/************************************************************************/
char *CDataBase::GetAlias()
{
    LPCTSTR sql     = _T("select * from t_alias");
    BOOL bRet       = TRUE;
    CString strOri  = "";
    CString strAlias= "";

    try
    {
        _RecordsetPtr  rs = m_pConn->Execute(sql, NULL, adConnectUnspecified);
    
        while (!rs->adoEOF)
        {
            strOri     = (char*)(_bstr_t)rs->Fields->Item[(long)0]->Value;
            strAlias   = (char*)(_bstr_t)rs->Fields->Item[(long)1]->Value;
            
            //a hack, we just use the t_alias to store the updatetime
            if (0 == strOri.Compare("updatetime"))
            {
                m_strUpdateTime = strAlias;
            }

            m_strAlias += strOri;
            m_strAlias += "|";
            m_strAlias += strAlias;
            m_strAlias += "|";
        
            rs->MoveNext();
        }
    }
    catch (_com_error e)
    {
        AfxMessageBox((char *)e.Description());	
    
        bRet = FALSE;
    }

    int n = 0;

    return m_strAlias.GetBuffer(0);   
}

void CDataBase::RefreshUpdateTime()
{
    CTime t         = CTime::GetCurrentTime();
    m_strUpdateTime = t.Format("%Y/%m/%d %H:%M:%S");
}

/************************************************************************/
/* write UpdateTime to DB                                                                     */
/************************************************************************/
void CDataBase::FlushUpdateTime()
{
    CString strsql = "update t_alias set updatetime = " + m_strUpdateTime;
    try 
    {
        //exec the command
        _RecordsetPtr rs = m_pConn->Execute(strsql.GetBuffer(0), 
                                            NULL, 
                                            adConnectUnspecified);
    }
    catch(_com_error& e)
    {
        _bstr_t bstrDescription(e.Description());        
    }
    catch(...)
    {	
        
    }  
}

CString CDataBase::GetUpdateTime()
{
    return m_strUpdateTime;
}
