//  MyPacket.cpp: implementation of the CMyPacket class.
//
//////////////////////////////////////////////////////////////////////

#include "..\\Server\\StdAfx.h"
#include "MyPacket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/************************************************************************/
/* �Է��ͽ������ݣ���ȡ�����������͵ķ�װ
��֮ǰCMyPacket�ļ򻯰�                                                 */
/************************************************************************/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyPacket::CMyPacket()
{

}

CMyPacket::~CMyPacket()
{

}

static void DumpData(char *pData, int nLen)
{
    CString str;
    for (int i = 0; i < nLen; i++)
    {
        str.Format("%c", pData[i]);
        OutputDebugString(str);
    }
}

/************************************************************************/
/* �ӽ��յ�������szBuff�л�ȡ��Ϣ���ͣ�ʵ�ʵ�����λ�ã��Լ���С                                                                     */
/************************************************************************/
u_int CMyPacket::GetType(char *szBuff, char **pData, u_int *nLen)
{
    tagInfo *pInfo = (tagInfo *)szBuff;
    *pData         = &szBuff[sizeof(tagHDR)];
    *nLen          = *(u_int *)szBuff;

    return pInfo->m_hdr.m_nType;    
}

/************************************************************************/
/* 
��ʵ����ϢszText���Լ�����������Ϣstruct sockddr oriaddr ����tagInfo��ʽ���з����
1) �Ƚ�tagHDR ���ͳ�ȥ�����ڽ��շ�׼�����տռ�
2) �ٽ���������tagInfo���ͳ�ȥ

������
�����ȷ���tagHDR, �ٷ���tagInfo��ͬʱҪ����շ��Ƚ���tagHDR,�ٽ���tagInfo.
����������ģ���ΪUDPû�б��򡢿ɿ��Ļ��ơ�
���ԣ�����������������ġ�

typedef struct _tagHDR
{
    u_int       m_nLen;     
    u_int       m_nType;    
}tagHDR;

typedef struct _tagInfo
{
  tagHDR     m_hdr;  
  struct sockaddr m_oriaddr;     //��ע�����������Դ������������Ϣ����table name
  char       *m_pInfo;   
}tagInfo;                                                                     */
/************************************************************************/
BOOL CMyPacket::NotifyInfo(u_long nType,
                              char *szText,             //Info to send
                              SOCKET skSend,            //who send this
                              struct sockaddr oriaddr,  //��ע�����������Դ, ���߱����������Ϣ
                              struct sockaddr *toaddr,  //where send to 
                              int nToLen)               //length of dest addr
{
    int nDataLen    = _tcslen(szText) + sizeof(TCHAR);
    int nTotalLen   = sizeof(tagHDR) + sizeof(sockaddr) + nDataLen;
    char *pTmpBuf   = new char[nTotalLen];
    if (NULL == pTmpBuf)
    {
        return FALSE;
    }
    
    tagInfo *pInfo          = (tagInfo *)pTmpBuf;
    pInfo->m_hdr.m_nLen     = nDataLen;
    pInfo->m_hdr.m_nType    = nType;
    pInfo->m_oriaddr        = oriaddr;
    strcpy((char *)&(pInfo->m_pInfo), szText);

    //�Ƚ�������Ϣ���ͳ�ȥ
    CMySocket::SendTo(skSend,
                     (char *)pInfo,
                     sizeof(tagHDR),
                     0,
                     toaddr,
                     nToLen);

    //�ٷ�����������    
    CMySocket::SendTo(skSend,       
                    (char *)pInfo,
                    nTotalLen,      //
                    0,              //flg
                    toaddr,
                    nToLen);
    
    return TRUE;   
}