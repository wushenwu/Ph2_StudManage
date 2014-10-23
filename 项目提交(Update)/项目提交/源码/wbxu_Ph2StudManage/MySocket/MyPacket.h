// MyPacket.h: interface for the CMyPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYPACKET_H__24A6D46B_6070_4CB1_8BDC_8FFCBBDC11F9__INCLUDED_)
#define AFX_MYPACKET_H__24A6D46B_6070_4CB1_8BDC_8FFCBBDC11F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Winsock2.h>
#include "MySocket.h"

/*
通信数据格式,见tagHDR, tagInfo

消息类型
*/
#define INFO_TYPE_BASE          0x80000000
#define INFO_TYPE_UDP_INIT      (INFO_TYPE_BASE + 1)
#define INFO_TYPE_UDP_OK        (INFO_TYPE_BASE + 2) 

#define INFO_TYPE_GETTABLES     (INFO_TYPE_BASE + 3)  //获取用户表
#define INFO_TYPE_TABLES        (INFO_TYPE_BASE + 4)  //用户表应答

#define INFO_TYPE_GETCOLUMNS    (INFO_TYPE_BASE + 5)  //获取字段
#define INFO_TYPE_COLUMNS       (INFO_TYPE_BASE + 6)  //表字段应答

#define INFO_TYPE_GETUPDATETIME (INFO_TYPE_BASE + 7)  //获取数据库最新更改时间
#define INFO_TYPE_UPDATETIME    (INFO_TYPE_BASE + 8)  //更新时间应答

#define INFO_TYPE_SELECT        (INFO_TYPE_BASE + 9)  //查询数据库
#define INFO_TYPE_INSERT        (INFO_TYPE_BASE + 10)  //插入数据
#define INFO_TYPE_DEL           (INFO_TYPE_BASE + 11)  //删除数据
#define INFO_TYPE_UPDATE        (INFO_TYPE_BASE + 12)   //修改数据
#define INFO_TYPE_REQUEST       (INFO_TYPE_BASE + 13)   //Client发向M Server， 
                                                        //是select, insert, del, update的总陈
#define INFO_TYPE_REPLY         (INFO_TYPE_BASE + 14)   //Server对Client的应答

#define INFO_TYPE_REPLY_SELECT  (INFO_TYPE_BASE + 15)   //Server对Client的Select的应答
#define INFO_TYPE_REPLY_INSERT  (INFO_TYPE_BASE + 16)
#define INFO_TYPE_REPLY_DEL     (INFO_TYPE_BASE + 17)
#define INFO_TYPE_REPLY_UPDATE  (INFO_TYPE_BASE + 18)
#define INFO_TYPE_REPLY_ERROR   (INFO_TYPE_BASE + 19)   //请求的操作执行失败
#define INFO_TYPE_REPLY_SUCCESS (INFO_TYPE_BASE + 20)   //请求的操作执行成功

#define INFO_TYPE_GETALIAS      (INFO_TYPE_BASE + 21)   //获取表、字段别名
#define INFO_TYPE_ALIAS         (INFO_TYPE_BASE + 22)   //表、字段别名的应答

/************************************************************************/
/* 交互报文的格式
注意：
1) 这里sockaddr m_oriaddr的作用比较灵活，a hack                         
2) 可用来标注请求的最终来源，这样M Server将其原封不动地转发给Server
    当Server接收到后，可将此信息原封不动地再应答给M Server
          这样，M Server就知道将应答回给谁，从而间接的维护了请求与应答间的对应，
                而不用额外的数据结构进行维护

    当然，当Server与Client间可直达时，Server也可通过此m_oriaddr，直接将应答回给Client,
            而无需M Server进行再转发

3)上面更多的发生在M Server给Server转发消息时。
4) 而从Client角度来看，它可能需要给M Server提供诸如要操作的表等信息，此时，也可填充在此。
5) 当都不需要这些信息时，也可置空处理。
*/
/************************************************************************/
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

/************************************************************************/
/* 对发送交互数据，获取交互数据类型的封装
是之前CMyPacket的简化版                                                 */
/************************************************************************/
class CMyPacket  
{
public:
    CMyPacket();
	virtual ~CMyPacket();

public:
    static BOOL NotifyInfo(  u_long nType,            // type                         
                             char *szText,            //Info
                             SOCKET skSend,           //who send this
                             struct sockaddr oriaddr, //标注请求的最终来源，或者表名等填充信息
                             struct sockaddr *toaddr, //to where
                             int nToLen);             //len of dst addr
    
	static u_int GetType(char *szBuff,  //which buffer's type to get
                        char **pData,   //the real data
                        u_int *nLen);   //the real len

};

#endif // !defined(AFX_MYPACKET_H__24A6D46B_6070_4CB1_8BDC_8FFCBBDC11F9__INCLUDED_)
