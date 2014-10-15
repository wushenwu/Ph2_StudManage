// RecvFromWorkItem.cpp: implementation of the CRecvFromWorkItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Server.h"
#include "RecvFromWorkItem.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRecvFromWorkItem::CRecvFromWorkItem(char *pszBuff,        //接收到的数据
                                     sockaddr_in addrin,   //发送端addr
                                     CNetWork *pNet)
{   
    tagHDR *pHDR = (tagHDR *)pszBuff;
    int nLen     = pHDR->m_nLen + sizeof(tagHDR) + sizeof(struct sockaddr);
    m_pszBuff    = NULL;
    m_pszBuff    = new char[nLen];
    if (NULL == m_pszBuff)
    {
        //do sth
    }

    memcpy(m_pszBuff, pszBuff, nLen);

    m_addrin  = addrin;
    m_pNet    = pNet;
}

CRecvFromWorkItem::~CRecvFromWorkItem()
{
    if (m_pszBuff != NULL)
    {
        delete[] m_pszBuff;
        m_pszBuff = NULL;
    }
}

void CRecvFromWorkItem::Execute()
{    
    m_pNet->ClassifyInfo(m_pszBuff, m_addrin);
}
void CRecvFromWorkItem::Abort()
{
}
