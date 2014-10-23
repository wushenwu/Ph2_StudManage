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
/* ��Socket�Ļ�����װ                                                  
   ����Ĭ�ϲΡ������飬�ṩ����ĵ��û���
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
       //������ϸ������
       ShowErrorMessage();
       m_strErr = "CMySocket WSAStartup Error";
       m_IsErr  = TRUE;
   }
}

CMySocket::~CMySocket()
{
    Close();

    //������������������Ϊջ�ռ������ͷŶ�������������״̬�ı����
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
/* Function : ���ṩ��IP, Port������Ӧת����Ȼ��bind                    */
/************************************************************************/
BOOL CMySocket::Bind(const char *IP, u_short Port)
{
    sockaddr_in addr            = {0};
    addr.sin_family             = AF_INET;
    addr.sin_port               = htons(Port);  //�����Ǵ�˴���
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
    addr.sin_port               = htons(Port);  //��Ҫ���ǶԴ�˵Ĵ���
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
/* ��̬����������ͬsend, ���ӳ�����                                  */
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
/* CMySocket ����ʹ��
����ֵ: ʵ�ʷ��͵ĳ���                                                  */
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
/* ��̬����������ͬsendto, ���ӳ��������                             */
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
/* ��̬����������ͬrecv
   ����tagHDR����ճ������                                               */
/************************************************************************/
BOOL CMySocket::Recv(SOCKET client, char *buf, int len, int flags)
{
    //Ӧ�ý���ճ������, tagInfo:  len, (type, data)
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
    
    //Ӧ��һֱ��ȡ��ֱ������nTotalLen
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
/* ͨ�õ����ݽ���, ����ͬrecv, ���ӳ��������                        */
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
/* �ض������µ����ݽ��մ���
1)�Ƚ���tagHDR, ��ȡ������Ϣ
2)�����ó���������*ppbuf�Ŀռ��С*plen���бȽ�
3)������㣬�������㹻�ռ�
4)Ȼ���ٽ�����������                                    

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
}tagInfo;

*/
/************************************************************************/
BOOL CMySocket::RecvFrom(SOCKET s,                  //[in]
                         char **ppbuf,              //[in,out] �������ݵĿռ� 
                         int *plen,                 //[in,out] �ռ��С
                         int flags,                 
                         struct sockaddr *from,     
                         int *fromlen
                         )
{
    //�Ƚ���tagHDR,��ȡ������Ϣ
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

    //�����Ҫ���������㹻�Ŀռ�
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
/* ��ȡSOCKET sk����Ӧ��sockaddr_in������Ӧ��ip: port�ַ�����Ϣ         */
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
/* ��ȡsockaddr_in *paddrin ��Ӧ��ip: port�ַ�����Ϣ                    */
/************************************************************************/
void CMySocket::GetAddress(struct sockaddr_in *paddrin,     //[in]
                           CString &strIPPort               //[in,out]
                           )
{
    char *cIP = inet_ntoa(paddrin->sin_addr);
    strIPPort.Format("%s: %d", cIP, ntohs(paddrin->sin_port));
}

/************************************************************************/
/* ��ȡ����Ĵ�����Ϣ                                                   */
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
/* ��WSAGetLastError�����Ѻõ�չʾ                                      */
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

