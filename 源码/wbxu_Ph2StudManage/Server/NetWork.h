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
/* CNetWork �����ݽ�������ǰ�ˣ�
���翪���������ӡ��������ݡ�������Ϣ���ࡢ��������ģ�������            */
/************************************************************************/

class CNetWork  
{
public:
    CNetWork();
	virtual ~CNetWork();

public:
    BOOL LoadServInfo();        //�������ļ��л�ȡ�Զ˵�IP, Port��Ϣ
    void OnStopService();
    void OnStartService();      //�������رն�clients�����罻��

    void ClassifyInfo(char *szBuff,         //�����յ����ݣ�����sockaddr ��䣬�Լ�ʵ������
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
                        BOOL bColumns,              //��ȡ�ֶλ�������
                        CString &strRet             //��ѯ�����
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
