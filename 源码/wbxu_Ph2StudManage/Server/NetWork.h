// NetWork.h: interface for the CNetWork class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETWORK_H__2D804AB6_434D_4E11_AA44_20689484B075__INCLUDED_)
#define AFX_NETWORK_H__2D804AB6_434D_4E11_AA44_20689484B075__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable: 4786)

#include <afxtempl.h>
#include <map>
using namespace std;

#include "..\\MySocket\\MySocket.h"
#include "..\\MySocket\\MyPacket.h"
#include "..\\ThreadPool\\MyThreadPool.h"
#include "..\\ThreadPool\\IWorkItemBase.h"

/************************************************************************/
/* CNetWork 是数据交互的最前端，
比如开启网络连接、接收数据、数据信息分类、触发其他模块操作等            */
/************************************************************************/

class CNetWork  
{
public:
    CNetWork();
	virtual ~CNetWork();

public:
    BOOL LoadServInfo();        //从配置文件中获取对端的IP, Port信息
    void OnStopService();
    void OnStartService();      //开启、关闭对clients的网络交互

    void ClassifyInfo(char *szBuff,         //所接收的数据，包括sockaddr 填充，以及实际数据
                        sockaddr_in addrin  //who send this
                        );   


public:
	void ProcessModify(char *pData,                 //sql command
                      _ConnectionPtr pConn,         //which connptr should deal with this
                      struct sockaddr_in addrin,    //whom should reply to   
                      BOOL &bSucceed,               
                      CString &strRet
                      );

	void ProcessSelect(char *pData,
                        _ConnectionPtr pConn, 
                        struct sockaddr_in addrin, 
                        BOOL bColumns,              //获取字段还是内容
                        CString &strRet             //查询结果集
                        );

	BOOL ProcessUDPInit(char *pRequest, 
                        int nLen, 
                        struct sockaddr_in addrin
                        );

    static CString GetErrorDescription(_com_error& e);

public:
    CMySocket m_MySocket;
    CMyThreadPool m_ThreadPool;

private:
    CString m_strIP;    
    unsigned int m_uhPort; 
};

#endif // !defined(AFX_NETWORK_H__2D804AB6_434D_4E11_AA44_20689484B075__INCLUDED_)
