// MySocket.h: interface for the CMySocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSOCKET_H__E4574608_E4F7_4387_8F37_7082F8B3EE3E__INCLUDED_)
#define AFX_MYSOCKET_H__E4574608_E4F7_4387_8F37_7082F8B3EE3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Winsock2.h>
#pragma  comment(lib, "Ws2_32.lib")

//#define  HOST ("127.0.0.1")
//#define  PORT   (3412) 

/************************************************************************/
/* 配置文件相关                                                         */
/************************************************************************/
#define CONFIG_FILE          "Server.ini"
#define SECTION_NAME         "Server"
#define M_SECTION_NAME       "M Server"
#define KEY_IP               "ip"
#define KEY_PORT             "port" 

/************************************************************************/
/* 对Socket的基本封装                                                  
   利用默认参、错误检查，提供方便的调用机制
/************************************************************************/
class CMySocket  
{
public:
    CMySocket(BYTE low = 2, BYTE high = 2);
	virtual ~CMySocket();

public:
    BOOL Create_TCP(int af = AF_INET, 
                    int type = SOCK_STREAM,     
                    int  proto = IPPROTO_TCP
                    );
    
    BOOL Create_UDP(int af = AF_INET, 
                    int type = SOCK_DGRAM,
                    int  proto = IPPROTO_UDP
                    );

    BOOL Connect(const char *IP, u_short Port);
    BOOL Accept(struct sockaddr *addr, SOCKET *clientsocket);
    BOOL Listen(int backlog = SOMAXCONN);
	BOOL Bind(const char *IP, u_short Port);
    BOOL Close();

    int Send(const char *buf, int len, int flags);

    static BOOL SendTo(SOCKET client, 
                       char *buf, 
                       int len, 
                       int flags, 
                       struct sockaddr *addr, 
                        int nLen
                        );
    
    static BOOL RecvFrom(SOCKET s, 
                        char **ppbuf, 
                        int *plen, 
                        int flags,  
                        struct sockaddr *from, 
                        int *fromlen
                        );

    static BOOL RecvRaw(SOCKET client, 
                        char *buf, 
                        int len, 
                        int flags,
                        int *pnLen = NULL
                        );  //未处理粘包

	static BOOL Recv(SOCKET client, 
                    char *buf, 
                    int len, 
                    int flags
                    ); //解决粘包

    static BOOL Send(SOCKET client, 
                    char *buf, 
                    int len,
                    int flags
                    );
	
public:
    static void ShowErrorMessage();
    TCHAR * GetError();

	static void GetAddress(struct sockaddr_in *paddrin, 
                            CString &strIPPort
                            );

    static BOOL GetAddress(SOCKET sk, 
                            struct sockaddr_in *addrin,
                            CString &strIPPort
                            );
public:
    SOCKET m_Socket;

protected:
    WSAData m_wsaData;
    CString m_strErr;   //记录错误信息
    BOOL m_IsErr;
};

#endif // !defined(AFX_MYSOCKET_H__E4574608_E4F7_4387_8F37_7082F8B3EE3E__INCLUDED_)
