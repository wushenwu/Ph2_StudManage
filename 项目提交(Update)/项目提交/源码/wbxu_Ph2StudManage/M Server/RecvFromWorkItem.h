// RecvFromWorkItem.h: interface for the CRecvFromWorkItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECVFROMWORKITEM_H__08961EA4_B058_478C_83CA_029E9DE6D6A4__INCLUDED_)
#define AFX_RECVFROMWORKITEM_H__08961EA4_B058_478C_83CA_029E9DE6D6A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\\ThreadPool\\IWorkItemBase.h"
#include "NetWork.h"

class CRecvFromWorkItem : public IWorkItemBase
{
public:
	CRecvFromWorkItem(char *pszBuff,        //接收到的数据
                      sockaddr_in addrin,   //发送端addr
                      CNetWork *pNet);
	virtual ~CRecvFromWorkItem();

public:
    virtual void Execute();
    virtual void Abort();

private:
    char *m_pszBuff;
    sockaddr_in m_addrin;
    CNetWork *m_pNet;
};

#endif // !defined(AFX_RECVFROMWORKITEM_H__08961EA4_B058_478C_83CA_029E9DE6D6A4__INCLUDED_)
