package com.wbxu.studandroid;

/**
 * 
 * �йس���
 *
 */
public class NetInfo
{
    public static final int INFO_TYPE_BASE = 0x80000000;
    public static final int INFO_TYPE_UDP_INIT      = (INFO_TYPE_BASE + 1);
    public static final int INFO_TYPE_UDP_OK        = (INFO_TYPE_BASE + 2); 

    public static final int INFO_TYPE_GETTABLES     = (INFO_TYPE_BASE + 3);  //��ȡ�û���
    public static final int INFO_TYPE_TABLES        = (INFO_TYPE_BASE + 4);  //�û���Ӧ��

    public static final int INFO_TYPE_GETCOLUMNS    = (INFO_TYPE_BASE + 5);  //��ȡ�ֶ�
    public static final int INFO_TYPE_COLUMNS       = (INFO_TYPE_BASE + 6);  //���ֶ�Ӧ��

    public static final int INFO_TYPE_GETUPDATETIME = (INFO_TYPE_BASE + 7);  //��ȡ���ݿ����¸���ʱ��
    public static final int INFO_TYPE_UPDATETIME    = (INFO_TYPE_BASE + 8);  //����ʱ��Ӧ��

    public static final int INFO_TYPE_SELECT        = (INFO_TYPE_BASE + 9);  //��ѯ���ݿ�
    public static final int INFO_TYPE_INSERT        = (INFO_TYPE_BASE + 10);  //��������
    public static final int INFO_TYPE_DEL           = (INFO_TYPE_BASE + 11);  //ɾ������
    public static final int INFO_TYPE_UPDATE        = (INFO_TYPE_BASE + 12);   //�޸�����
    public static final int INFO_TYPE_REQUEST       = (INFO_TYPE_BASE + 13);   //Client����M Server�� 
                                                            //��select, insert, del, update���ܳ�
    public static final int INFO_TYPE_REPLY         = (INFO_TYPE_BASE + 14);   //Server��Client��Ӧ��

    public static final int INFO_TYPE_REPLY_SELECT  = (INFO_TYPE_BASE + 15);   //Server��Client��Select��Ӧ��
    public static final int INFO_TYPE_REPLY_INSERT  = (INFO_TYPE_BASE + 16);
    public static final int INFO_TYPE_REPLY_DEL     = (INFO_TYPE_BASE + 17);
    public static final int INFO_TYPE_REPLY_UPDATE  = (INFO_TYPE_BASE + 18);
    public static final int INFO_TYPE_REPLY_ERROR   = (INFO_TYPE_BASE + 19);   //����Ĳ���ִ��ʧ��
    public static final int INFO_TYPE_REPLY_SUCCESS = (INFO_TYPE_BASE + 20);   //����Ĳ���ִ�гɹ�

    public static final int INFO_TYPE_GETALIAS      = (INFO_TYPE_BASE + 21);   //��ȡ���ֶα���
    public static final int INFO_TYPE_ALIAS         = (INFO_TYPE_BASE + 22);   //���ֶα�����Ӧ��
}
