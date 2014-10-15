package com.wbxu.studandroid;

/**
 * 
 * 有关常量
 *
 */
public class NetInfo
{
    public static final int INFO_TYPE_BASE = 0x80000000;
    public static final int INFO_TYPE_UDP_INIT      = (INFO_TYPE_BASE + 1);
    public static final int INFO_TYPE_UDP_OK        = (INFO_TYPE_BASE + 2); 

    public static final int INFO_TYPE_GETTABLES     = (INFO_TYPE_BASE + 3);  //获取用户表
    public static final int INFO_TYPE_TABLES        = (INFO_TYPE_BASE + 4);  //用户表应答

    public static final int INFO_TYPE_GETCOLUMNS    = (INFO_TYPE_BASE + 5);  //获取字段
    public static final int INFO_TYPE_COLUMNS       = (INFO_TYPE_BASE + 6);  //表字段应答

    public static final int INFO_TYPE_GETUPDATETIME = (INFO_TYPE_BASE + 7);  //获取数据库最新更改时间
    public static final int INFO_TYPE_UPDATETIME    = (INFO_TYPE_BASE + 8);  //更新时间应答

    public static final int INFO_TYPE_SELECT        = (INFO_TYPE_BASE + 9);  //查询数据库
    public static final int INFO_TYPE_INSERT        = (INFO_TYPE_BASE + 10);  //插入数据
    public static final int INFO_TYPE_DEL           = (INFO_TYPE_BASE + 11);  //删除数据
    public static final int INFO_TYPE_UPDATE        = (INFO_TYPE_BASE + 12);   //修改数据
    public static final int INFO_TYPE_REQUEST       = (INFO_TYPE_BASE + 13);   //Client发向M Server， 
                                                            //是select, insert, del, update的总陈
    public static final int INFO_TYPE_REPLY         = (INFO_TYPE_BASE + 14);   //Server对Client的应答

    public static final int INFO_TYPE_REPLY_SELECT  = (INFO_TYPE_BASE + 15);   //Server对Client的Select的应答
    public static final int INFO_TYPE_REPLY_INSERT  = (INFO_TYPE_BASE + 16);
    public static final int INFO_TYPE_REPLY_DEL     = (INFO_TYPE_BASE + 17);
    public static final int INFO_TYPE_REPLY_UPDATE  = (INFO_TYPE_BASE + 18);
    public static final int INFO_TYPE_REPLY_ERROR   = (INFO_TYPE_BASE + 19);   //请求的操作执行失败
    public static final int INFO_TYPE_REPLY_SUCCESS = (INFO_TYPE_BASE + 20);   //请求的操作执行成功

    public static final int INFO_TYPE_GETALIAS      = (INFO_TYPE_BASE + 21);   //获取表、字段别名
    public static final int INFO_TYPE_ALIAS         = (INFO_TYPE_BASE + 22);   //表、字段别名的应答
}
