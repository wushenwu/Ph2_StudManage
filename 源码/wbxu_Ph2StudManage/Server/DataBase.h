// DataBase.h: interface for the CDataBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATABASE_H__639F15F0_376A_439D_BB8E_3CD943AE1491__INCLUDED_)
#define AFX_DATABASE_H__639F15F0_376A_439D_BB8E_3CD943AE1491__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/***********************************************************************
* Module:  CDataBase
* Purpose: Encapsulation for DataBase Related, dealing with SQL Server
           like Connection to DB, Connection Pool
 ***********************************************************************/

#import "C:\\Program Files\\Common Files\\System\\Ole DB\\oledb32.dll" rename_namespace("DBLink")

#define ADO_H "C:\\Program Files\\Common Files\\System\\ado\\msado15.dll"
#import ADO_H rename("EOF", "adoEOF")  no_namespace

/************************************************************************/
/* 对数据库操作的封装，连接池等                                         */
/************************************************************************/
class CDataBase  
{
public:
	CDataBase();
	virtual ~CDataBase();

    enum { CONNECTION_POOL_CAPACITY = 5 };

public:
    BOOL ConnectDB();
    BOOL CreateConnectionPool();
    void ReleaseConn(int index);
    _ConnectionPtr GetConnFromPool(int *pIndex);
    
    char *GetTblNames();
    char *GetAlias();
	
    CString GetUpdateTime();
    void FlushUpdateTime();
	void RefreshUpdateTime();

private:
    BOOL LoadTblNames();

protected:
    //for self usage
    _ConnectionPtr m_pConn;                     

    //we bind _ConnectionPtr with Events, to justify the availability
    _ConnectionPtr m_pConnPool[CONNECTION_POOL_CAPACITY]; 
    HANDLE m_hEvents[CONNECTION_POOL_CAPACITY]; 

    //
    CString m_strTblNames;
    CString m_strAlias;  
    CString     m_strUpdateTime;
};  

#endif // !defined(AFX_DATABASE_H__639F15F0_376A_439D_BB8E_3CD943AE1491__INCLUDED_)
