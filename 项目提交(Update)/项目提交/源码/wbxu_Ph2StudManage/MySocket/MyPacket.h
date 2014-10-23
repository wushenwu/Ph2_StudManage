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
ͨ�����ݸ�ʽ,��tagHDR, tagInfo

��Ϣ����
*/
#define INFO_TYPE_BASE          0x80000000
#define INFO_TYPE_UDP_INIT      (INFO_TYPE_BASE + 1)
#define INFO_TYPE_UDP_OK        (INFO_TYPE_BASE + 2) 

#define INFO_TYPE_GETTABLES     (INFO_TYPE_BASE + 3)  //��ȡ�û���
#define INFO_TYPE_TABLES        (INFO_TYPE_BASE + 4)  //�û���Ӧ��

#define INFO_TYPE_GETCOLUMNS    (INFO_TYPE_BASE + 5)  //��ȡ�ֶ�
#define INFO_TYPE_COLUMNS       (INFO_TYPE_BASE + 6)  //���ֶ�Ӧ��

#define INFO_TYPE_GETUPDATETIME (INFO_TYPE_BASE + 7)  //��ȡ���ݿ����¸���ʱ��
#define INFO_TYPE_UPDATETIME    (INFO_TYPE_BASE + 8)  //����ʱ��Ӧ��

#define INFO_TYPE_SELECT        (INFO_TYPE_BASE + 9)  //��ѯ���ݿ�
#define INFO_TYPE_INSERT        (INFO_TYPE_BASE + 10)  //��������
#define INFO_TYPE_DEL           (INFO_TYPE_BASE + 11)  //ɾ������
#define INFO_TYPE_UPDATE        (INFO_TYPE_BASE + 12)   //�޸�����
#define INFO_TYPE_REQUEST       (INFO_TYPE_BASE + 13)   //Client����M Server�� 
                                                        //��select, insert, del, update���ܳ�
#define INFO_TYPE_REPLY         (INFO_TYPE_BASE + 14)   //Server��Client��Ӧ��

#define INFO_TYPE_REPLY_SELECT  (INFO_TYPE_BASE + 15)   //Server��Client��Select��Ӧ��
#define INFO_TYPE_REPLY_INSERT  (INFO_TYPE_BASE + 16)
#define INFO_TYPE_REPLY_DEL     (INFO_TYPE_BASE + 17)
#define INFO_TYPE_REPLY_UPDATE  (INFO_TYPE_BASE + 18)
#define INFO_TYPE_REPLY_ERROR   (INFO_TYPE_BASE + 19)   //����Ĳ���ִ��ʧ��
#define INFO_TYPE_REPLY_SUCCESS (INFO_TYPE_BASE + 20)   //����Ĳ���ִ�гɹ�

#define INFO_TYPE_GETALIAS      (INFO_TYPE_BASE + 21)   //��ȡ���ֶα���
#define INFO_TYPE_ALIAS         (INFO_TYPE_BASE + 22)   //���ֶα�����Ӧ��

/************************************************************************/
/* �������ĵĸ�ʽ
ע�⣺
1) ����sockaddr m_oriaddr�����ñȽ���a hack                         
2) ��������ע�����������Դ������M Server����ԭ�ⲻ����ת����Server
    ��Server���յ��󣬿ɽ�����Ϣԭ�ⲻ������Ӧ���M Server
          ������M Server��֪����Ӧ��ظ�˭���Ӷ���ӵ�ά����������Ӧ���Ķ�Ӧ��
                �����ö�������ݽṹ����ά��

    ��Ȼ����Server��Client���ֱ��ʱ��ServerҲ��ͨ����m_oriaddr��ֱ�ӽ�Ӧ��ظ�Client,
            ������M Server������ת��

3)�������ķ�����M Server��Serverת����Ϣʱ��
4) ����Client�Ƕ���������������Ҫ��M Server�ṩ����Ҫ�����ı����Ϣ����ʱ��Ҳ������ڴˡ�
5) ��������Ҫ��Щ��Ϣʱ��Ҳ���ÿմ���
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
    struct sockaddr m_oriaddr;     //��ע�����������Դ������������Ϣ����table name
    char       *m_pInfo;   
}tagInfo;

/************************************************************************/
/* �Է��ͽ������ݣ���ȡ�����������͵ķ�װ
��֮ǰCMyPacket�ļ򻯰�                                                 */
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
                             struct sockaddr oriaddr, //��ע�����������Դ�����߱����������Ϣ
                             struct sockaddr *toaddr, //to where
                             int nToLen);             //len of dst addr
    
	static u_int GetType(char *szBuff,  //which buffer's type to get
                        char **pData,   //the real data
                        u_int *nLen);   //the real len

};

#endif // !defined(AFX_MYPACKET_H__24A6D46B_6070_4CB1_8BDC_8FFCBBDC11F9__INCLUDED_)
