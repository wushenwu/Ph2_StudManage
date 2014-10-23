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
/* 对发送交互数据，获取交互数据类型的封装
是之前CMyPacket的简化版                                                 */
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
/* 从接收到的数据szBuff中获取消息类型，实际的数据位置，以及大小                                                                     */
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
将实际信息szText，以及其他辅助信息struct sockddr oriaddr 按照tagInfo格式进行封包，
1) 先将tagHDR 发送出去，便于接收方准备接收空间
2) 再将完整数据tagInfo发送出去

自评：
这里先发送tagHDR, 再发送tagInfo。同时要求接收方先接收tagHDR,再接收tagInfo.
这是有问题的，因为UDP没有保序、可靠的机制。
所以，这样处理是有问题的。

typedef struct _tagHDR
{
    u_int       m_nLen;     
    u_int       m_nType;    
}tagHDR;

typedef struct _tagInfo
{
  tagHDR     m_hdr;  
  struct sockaddr m_oriaddr;     //标注请求的最终来源，或者其他信息，如table name
  char       *m_pInfo;   
}tagInfo;                                                                     */
/************************************************************************/
BOOL CMyPacket::NotifyInfo(u_long nType,
                              char *szText,             //Info to send
                              SOCKET skSend,            //who send this
                              struct sockaddr oriaddr,  //标注请求的最终来源, 或者表名等填充信息
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

    //先将长度信息发送出去
    CMySocket::SendTo(skSend,
                     (char *)pInfo,
                     sizeof(tagHDR),
                     0,
                     toaddr,
                     nToLen);

    //再发送所有数据    
    CMySocket::SendTo(skSend,       
                    (char *)pInfo,
                    nTotalLen,      //
                    0,              //flg
                    toaddr,
                    nToLen);
    
    return TRUE;   
}