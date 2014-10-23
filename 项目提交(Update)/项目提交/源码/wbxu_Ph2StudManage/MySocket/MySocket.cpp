// MySocket.cpp: implementation of the CMySocket class.
//
//////////////////////////////////////////////////////////////////////

/*
#define MAKEWORD(low,high) \
        ((WORD)((BYTE)(low)) | (((WORD)(BYTE)(high))<<8)))

int WSAStartup(
    WORD wVersionRequested,
    LPWSADATA lpWSAData
    );

SOCKET socket(
  int af,       
  int type,     
  int protocol  
);

int bind(
  SOCKET s,                          
  const struct sockaddr FAR *name,   
  int namelen                        
  );

int listen(
  SOCKET s,    
  int backlog  
);

SOCKET accept(
  SOCKET s,
  struct sockaddr FAR *addr,
  int FAR *addrlen
);

int connect(
  SOCKET s,                          
  const struct sockaddr FAR *name,  
  int namelen                        
);

int recv(
  SOCKET s,       
  char FAR *buf,  
  int len,        
  int flags       
);

int send(
  SOCKET s,              
  const char FAR *buf,  
  int len,               
  int flags              
);

*/

#include "..\\M Server\\StdAfx.h"
#include "MySocket.h"
#include "MyPacket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/************************************************************************/
/* 对Socket的基本封装                                                  
   利用默认参、错误检查，提供方便的调用机制
/************************************************************************/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMySocket::CMySocket(BYTE low, BYTE high)
{   
   m_Socket = INVALID_SOCKET;
   m_IsErr  = FALSE;
   memset(&m_wsaData, 0, sizeof(WSADATA));

   //////////////////////////////////////////////////////////////////////////
   WORD wVerReq = MAKEWORD(low, high);
   int nRet     = ::WSAStartup(wVerReq, &m_wsaData);
   if (nRet)
   {
       //其他详细错误处理
       ShowErrorMessage();
       m_strErr = "CMySocket WSAStartup Error";
       m_IsErr  = TRUE;
   }
}

CMySocket::~CMySocket()
{
    Close();

    //不能这样处理，可能因为栈空间对象的释放而导致整个网络状态的变更。
    //WSACleanup();  
}

BOOL CMySocket::Create_TCP(int af, int type, int proto)
{
    m_Socket = ::socket(af, type, proto);
    if (INVALID_SOCKET == m_Socket)
    {
        ShowErrorMessage();
        m_strErr    = "CMySocket Create Error";
        m_IsErr     = TRUE;

        return FALSE;
    }

    m_IsErr = FALSE;
    return TRUE;
}

BOOL CMySocket::Create_UDP(int af, int type, int proto)
{
    m_Socket = ::socket(af, type, proto);
    if (INVALID_SOCKET == m_Socket)
    {
        ShowErrorMessage();
        m_strErr    = "CMySocket Create Error";
        m_IsErr     = TRUE;
        
        return FALSE;
    }
    
    m_IsErr = FALSE;
    return TRUE;
}

/************************************************************************/
/* Function : 对提供的IP, Port进行相应转换，然后bind                    */
/************************************************************************/
BOOL CMySocket::Bind(const char *IP, u_short Port)
{
    sockaddr_in addr            = {0};
    addr.sin_family             = AF_INET;
    addr.sin_port               = htons(Port);  //别忘记大端处理
    addr.sin_addr.S_un.S_addr   = inet_addr(IP);

    int nRet = ::bind(m_Socket, (struct sockaddr *)&addr, sizeof(sockaddr));
    if (SOCKET_ERROR == nRet)
    {
        ShowErrorMessage();
        closesocket(m_Socket);

        m_strErr = "CMySocket Bind Error";
        m_IsErr  = TRUE;
        return FALSE;
    }

    m_IsErr = FALSE;
    return TRUE;
}

BOOL CMySocket::Listen(int backlog)
{
    int nRet = ::listen(m_Socket, backlog);
    if (SOCKET_ERROR == nRet )
    {
        ShowErrorMessage();
        closesocket(m_Socket);

        m_strErr    = "CMySocket Listen Error";
        m_IsErr     = TRUE;
        return FALSE;
    }

    m_IsErr = FALSE;
    return TRUE;
}

BOOL CMySocket::Accept(struct sockaddr *addr, SOCKET *clientsocket)
{
    int nLen  = sizeof(struct sockaddr);
    SOCKET sk = ::accept(m_Socket,
                         addr,
                         &nLen);

    if (INVALID_SOCKET == sk)
    {
        ShowErrorMessage();
        closesocket(m_Socket);

        m_strErr    = "CMySocket Accept Error";
        m_IsErr     = TRUE;

        *clientsocket = INVALID_SOCKET;
        return FALSE;
    }

    *clientsocket   = sk;
    m_IsErr         = FALSE;
    return TRUE;
}

BOOL CMySocket::Connect(const char *IP, u_short Port)
{
    sockaddr_in addr            = {0};
    addr.sin_family             = AF_INET;
    addr.sin_port               = htons(Port);  //不要忘记对大端的处理
    addr.sin_addr.S_un.S_addr   = inet_addr(IP);
    
    int nRet = ::connect(m_Socket, (struct sockaddr *)&addr, sizeof(struct sockaddr));
    if (SOCKET_ERROR == nRet)
    {
        ShowErrorMessage();
        closesocket(m_Socket);

        m_strErr = "CMySocket Connect Error";
        m_IsErr  = TRUE;
        return FALSE;
    }
    
    m_IsErr = FALSE;
    return TRUE;
}

BOOL CMySocket::Close()
{
    if (INVALID_SOCKET == m_Socket)
    {
        m_strErr = "CMySocket close INVALID_SOCKET";
        m_IsErr  = TRUE;
        return FALSE;
    }
    
    int nRet = ::closesocket(m_Socket);
    if (SOCKET_ERROR == nRet)
    {
        ShowErrorMessage();
        m_strErr = "CMySocket close error";
        m_IsErr  = TRUE;
        
        return FALSE;
    }
    
    m_Socket = INVALID_SOCKET;
    m_IsErr  = FALSE;
    return TRUE;
}

/************************************************************************/
/* 静态方法，参数同send, 增加出错处理                                  */
/************************************************************************/
BOOL CMySocket::Send(SOCKET client, char *buf, int len, int flags)
{
    int nRet = ::send(client, buf, len, flags);
    if (SOCKET_ERROR == nRet)
    {
        ShowErrorMessage();
        return FALSE;
    }
    
    return TRUE;
}

/************************************************************************/
/* CMySocket 对象使用
返回值: 实际发送的长度                                                  */
/************************************************************************/
int CMySocket::Send(const char *buf, 
                    int len, 
                    int flags)
{
    int nRet = ::send(m_Socket, buf, len, flags);
    if (SOCKET_ERROR == nRet)
    {
        ShowErrorMessage();
    }
    
    return nRet;
}

/************************************************************************/
/* 静态方法，参数同sendto, 增加出错处理机制                             */
/************************************************************************/
BOOL CMySocket::SendTo(SOCKET client, 
                       char *buf, 
                       int len,
                       int flags, 
                       struct sockaddr *addr, 
                       int nLen)
{
    int nRet = ::sendto(client, buf, len, flags, addr, nLen);
    if (SOCKET_ERROR == nRet)
    {
        ShowErrorMessage();
        return FALSE;
    }
    
    return TRUE;
}

/************************************************************************/
/* 静态方法，参数同recv
   依据tagHDR进行粘包处理                                               */
/************************************************************************/
BOOL CMySocket::Recv(SOCKET client, char *buf, int len, int flags)
{
    //应该进行粘包处理, tagInfo:  len, (type, data)
    u_int nTotalLen = 0;
    u_int nRecv     = 0;
    int nRet        = 0;
    
    nRet = ::recv(client, buf, sizeof(tagHDR), flags);
    if (0 == nRet)
    {
        ShowErrorMessage();
        //gracefully closed
        return FALSE;
        
    }
    else if (SOCKET_ERROR == nRet)
    {
        //erro
        ShowErrorMessage();
        return FALSE;
    }
    
    //应该一直读取，直到读够nTotalLen
    nTotalLen = *(u_int *)buf;
    nRecv     = 0; //len
    while (nRecv < nTotalLen)
    {
        nRet = ::recv(client, buf + sizeof(tagHDR) + nRecv, nTotalLen - nRecv, flags);
        if (0 == nRet)
        {
            //gracefully closed
            ShowErrorMessage();
            return FALSE;
        }
        else if (SOCKET_ERROR == nRet)
        {
            //error
            ShowErrorMessage();
            return FALSE;
        }
        
        nRecv += nRet;
    }
    
    return TRUE;
}

/************************************************************************/
/* 通用的数据接收, 参数同recv, 增加出错检查机制                        */
/************************************************************************/
BOOL CMySocket::RecvRaw(SOCKET client, 
                        char *buf, 
                        int len, 
                        int flags, 
                        int *pnLen
                        )
{
    int nRet = ::recv(client, buf, len, flags);
    if (0 == nRet)
    {
        //gracefully closed
        ShowErrorMessage();
        return FALSE;
    }
    else if (SOCKET_ERROR == nRet)
    {
        if (WSAEWOULDBLOCK == WSAGetLastError())
        {
            return FALSE;
        }

        //error
        ShowErrorMessage();
        return FALSE;
    }

    if (pnLen != NULL)
    {
        *pnLen = nRet;
    }
    return TRUE;
}

/************************************************************************/
/* 特定场景下的数据接收处理
1)先接收tagHDR, 获取长度信息
2)将所得长度与现有*ppbuf的空间大小*plen进行比较
3)如果不足，则申请足够空间
4)然后再接收所有数据                                    

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
}tagInfo;

*/
/************************************************************************/
BOOL CMySocket::RecvFrom(SOCKET s,                  //[in]
                         char **ppbuf,              //[in,out] 接收数据的空间 
                         int *plen,                 //[in,out] 空间大小
                         int flags,                 
                         struct sockaddr *from,     
                         int *fromlen
                         )
{
    //先接收tagHDR,获取长度信息
    tagHDR hdr  = {0};
    int nRet    = ::recvfrom(s, (char *)&hdr, sizeof(tagHDR), flags, from, fromlen);
    if (0 == nRet)
    {
        ShowErrorMessage();
        return FALSE;
    }
    else if (SOCKET_ERROR == nRet)
    {
        ShowErrorMessage();
        return FALSE;
    }

    //如果需要，就申请足够的空间
    int nTotalLen = sizeof(tagHDR) + hdr.m_nLen + sizeof(struct sockaddr);
    if ( nTotalLen > *plen)
    {
        delete [] *ppbuf;
        *ppbuf = new char[nTotalLen];
        *plen = nTotalLen;
    }

    if (NULL == *ppbuf)
    {
        return FALSE;
    }

    nRet = ::recvfrom(s, *ppbuf, *plen, flags, from, fromlen);
    if (0 == nRet)
    {
        ShowErrorMessage();
        return FALSE;
    }
    else if (SOCKET_ERROR == nRet)
    {
        ShowErrorMessage();
        return FALSE;
    }

    return TRUE;
}

/************************************************************************/
/* 获取SOCKET sk所对应的sockaddr_in，及对应的ip: port字符串信息         */
/************************************************************************/
BOOL CMySocket::GetAddress(SOCKET sk,                   //[in]
                           struct sockaddr_in *paddrin, //[in,out] 
                           CString &strIPPort           //[in,out]
                           )
{
    int nLen = sizeof(sockaddr_in);
    int nRet = getpeername(sk, (sockaddr *)paddrin, &nLen);
    if (SOCKET_ERROR == nRet)
    {
        ShowErrorMessage();
        return FALSE;
    }
    
    char *cIP = inet_ntoa(paddrin->sin_addr);
    strIPPort.Format("%s: %d", cIP, ntohs(paddrin->sin_port));
    
    return TRUE;
}

/************************************************************************/
/* 获取sockaddr_in *paddrin 对应的ip: port字符串信息                    */
/************************************************************************/
void CMySocket::GetAddress(struct sockaddr_in *paddrin,     //[in]
                           CString &strIPPort               //[in,out]
                           )
{
    char *cIP = inet_ntoa(paddrin->sin_addr);
    strIPPort.Format("%s: %d", cIP, ntohs(paddrin->sin_port));
}

/************************************************************************/
/* 获取最近的错误信息                                                   */
/************************************************************************/
TCHAR * CMySocket::GetError()
{
    if (m_IsErr)
    {
        return m_strErr.GetBuffer(0);
    }
    else
    {
        return "No Error";
    }   
}

/************************************************************************/
/* 对WSAGetLastError进行友好的展示                                      */
/************************************************************************/
void CMySocket::ShowErrorMessage()
{
    LPVOID lpMsgBuf;
    FormatMessage( 
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM | 
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        ::WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        (LPTSTR) &lpMsgBuf,
        0,
        NULL 
        );
    
    ::MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
    LocalFree(lpMsgBuf);
}

