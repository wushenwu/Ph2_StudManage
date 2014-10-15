// NetWork.h: interface for the CNetWork class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETWORK_H__2D804AB6_434D_4E11_AA44_20689484B075__INCLUDED_)
#define AFX_NETWORK_H__2D804AB6_434D_4E11_AA44_20689484B075__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/*
Server �� M Server�����罻��
*/

#pragma warning(disable: 4786)

#include <afxtempl.h>
#include <map>
using namespace std;

#include "..\\MySocket\\MySocket.h"
#include "..\\MySocket\\MyPacket.h"

#include "Request.h"

class CNetWork  
{
public:
    CNetWork();
	virtual ~CNetWork();

public: 
    void OnStopService();
    void OnStartService();

public:
    void ClassifyInfo(char *szBuff, 
                        sockaddr_in addrin
                        ); 
	CString GetColsForTbl(CString &strTblName);
	CString GetOriName(CString &strAlias);
	CString GetAlias(CString &strOri);
    void SendRequest(CRequest *pReq);

protected:
    BOOL LoadServInfo();
	void SaveAlias(char *pszBuff);
	void GetAllColumns(char *pszTblNames);

public:
    CMySocket m_MySocket;

private:
    //
    map<CString, CString> m_map_tbl_cols;   // tblname : colunms
    map<CString, CString> m_map_ori_alias;  //ԭ���ֶ��� ---- ����
    map<CString, CString> m_map_alias_ori;  //����         -----ԭ���ֶ���

    CString m_strMIP;    
    unsigned int m_uhMPort;
};

#endif // !defined(AFX_NETWORK_H__2D804AB6_434D_4E11_AA44_20689484B075__INCLUDED_)
