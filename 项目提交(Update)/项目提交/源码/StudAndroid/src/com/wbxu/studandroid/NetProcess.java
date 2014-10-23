package com.wbxu.studandroid;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.util.HashMap;
import java.util.Map;

import android.content.Context;
import android.os.Bundle;
import android.os.Message;

public class NetProcess
{    
    //info about tabls and their colums, and map between oriname and alias name
    private  Map<String, String> m_map_tbl_cols = new HashMap<String, String>();
    private Map<String, String> m_map_ori_alias = new HashMap<String, String>();
    private Map<String, String> m_map_alias_ori = new HashMap<String, String>();
    public static String[]              m_strTbls;
    
    public static Boolean m_bOK = false;
    
    //network related
    private Context        m_context;
    private DatagramSocket m_Socket;
    private String         m_strIP;
    private int            m_nPort;
    private InetAddress    m_addr;
   
    public NetProcess()
    {
    }
    
    public NetProcess(Context context,String strIP, int nPort)
    {
        m_context = context;
        m_strIP = strIP;
        m_nPort = nPort;
    }
    
    /*
     * ��������ת��ΪС�˴洢��bytes
     */
    public static byte[] toBytes(int i)
    {
        byte[] result = new byte[4];

        result[3] = (byte) (i >> 24);
        result[2] = (byte) (i >> 16);
        result[1] = (byte) (i >> 8);
        result[0] = (byte) (i /*>> 0*/);

        return result;
    }
    
    /*
     * ��С�˵�byte[] ת���ɶ�Ӧ������
     */
    public static int toInt(byte[] bBuf)
    {
        int nRet = 0;
        
        for (int i = 0; i < 4; i++)
        {
            nRet += (bBuf[i] << (8 * i));           
        }
        
        return nRet;
    }
    
    /*
     * ��bOri�л�ȡstring��byte
     * bOri �� h e l l 00 00 00 ====> h e l l
     */
    public static byte[] getStringByte(byte[] bOri)
    {
        int i;
        for(i = 0; i < bOri.length; i++)
        {
            if(0x00 == bOri[i])
            {
                break;
            }
        }
        
        byte[] bRet = new byte[i];
        for (int j = 0; j < i; j++)
        {
            bRet[j]= bOri[j]; 
        }
        
        return bRet;
    }
    
    /*
     * Function: �������� (�ȷ���tagHDR, �ٷ���ȫ�����ݣ�
     * 
     * Param   :nInfoType���������ͣ�
     *          strPad ���������ݣ����sockaddr���֣�����Ϊtable name��
     *                  ���ܳ���4��4 byte
     *          strInfo �����ž������Ϣ������ʲô�ֶΣ�ʲôֵ��
     *                 
     * Return  : true �ɹ���falseʧ��
     * 
     * 
     * typedef struct _tagHDR
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
    public Boolean sendRequest(int nInfoType, byte[] bPad, String strInfo)
    {
        Boolean bRet = false;
        byte[] bEncoded = null;
        
        try
        {
            bEncoded = strInfo.getBytes("gbk");
        } 
        catch (UnsupportedEncodingException e1)
        {
            // TODO Auto-generated catch block
            e1.printStackTrace();
        }
        
        //��׼��tagHDR����
        int nLen        = bEncoded.length + 1;//strInfo.length() + 1;
        ByteBuffer bHDR = ByteBuffer.allocate(4         //m_nLen
                                              + 4       //m_nType
                                              );
        bHDR.put(toBytes(nLen));                        //m_nLen
        bHDR.put(toBytes(nInfoType));                   //m_nType
        
        byte[] bufHDR = bHDR.array();           
        DatagramPacket pktHDR = new DatagramPacket(bufHDR, 
                                                    bufHDR.length,
                                                    m_addr, 
                                                    m_nPort);
        
        //׼����������        
        ByteBuffer bTotal = ByteBuffer.allocate(4       //m_nLen
                                              + 4       //m_nType
                                              + 4*4     //sockaddr, just pad
                                              + nLen    //m_pInfo
                                              );
        bTotal.put(toBytes(nLen));                      //m_nLen
        bTotal.put(toBytes(nInfoType));                 //m_nType
        bTotal.put(bPad);                               //a hack, sockaddr
        bTotal.position(4 + 4 + 4*4);                   //skip to pInfo
        bTotal.put(bEncoded/*strInfo.getBytes()*/);     //m_pInfo
 
        byte[] bufTotal = bTotal.array();           
        DatagramPacket pktTotal = new DatagramPacket(bufTotal, 
                                                    bufTotal.length,
                                                    m_addr, 
                                                    m_nPort);
        try
        {
            m_Socket.send(pktHDR);
            m_Socket.send(pktTotal);
            bRet = true;   
            
        } catch (IOException e)
        {
            e.printStackTrace();
        }
        
        return bRet;
    }
    
    /**
     * Func     �� �����ʼ��
     * Return   : �ɹ�����true, ����false
     */
     protected Boolean Init()
    {       
        Boolean bRet = false;
        
        try
        {            
            //get the datagram socket
            m_Socket = new DatagramSocket();
            
            m_addr   = InetAddress.getByName(m_strIP);
            
            sendRequest(NetInfo.INFO_TYPE_UDP_INIT,
                        "".getBytes(),
                        "Hi, I'm Android Client, I wanna talk with u"
                        );
            
            //to get alias
            sendRequest(NetInfo.INFO_TYPE_GETALIAS, 
                        "".getBytes(), 
                        "I wann get alias"
                        );
            
            bRet    = true;
            
        } 
        catch (SocketException e)
        {
            e.printStackTrace();
        } 
        catch (UnknownHostException e)
        {
            e.printStackTrace();
        } 
        
        String strInfo = "�����ʼʧ��";
        if (true)
        {
            strInfo = "�����ʼ���ɹ�";
        }
        
        sendMessage("alertinfo", strInfo, -1);
        
        return bRet;
    }
     
     /*
      * �������ݣ�
      * 1���Ƚ���tagHDR���� (������sendRequest
      * 2���ٽ���ȫ������
      * 
      */
     public void recvInfo()
     {
         //
         byte[] bufHDR         = new byte[4 + 4];   //m_nLen, m_nType
         DatagramPacket pktHDR = new DatagramPacket(bufHDR, 4 + 4);
         
         try
         {
            m_Socket.receive(pktHDR);
            bufHDR = pktHDR.getData();
            
            //��ȡnLen
            int nLen = toInt(bufHDR);
            if (nLen < 0)
            {
                nLen += 256;
            }
            
            //����nLen����ռ䣬����ȫ������
			int nTotalLen 			= 4 + 4 + 4 * 4 + nLen;	//m_nLen, m_nType, pad, info
            byte[] bufTotal         = new byte[nTotalLen];
            DatagramPacket pktTotal = new DatagramPacket(bufTotal, nTotalLen);
            
            m_Socket.receive(pktTotal);
            bufTotal = pktTotal.getData();
            
            //��ȡnType
            ByteBuffer btTotal = ByteBuffer.wrap(bufTotal);
            btTotal.position(4);    //skip m_nLen
            byte[] bType       = new byte[4];
            btTotal.get(bType, 0, 4);
            int nType          = toInt(bType);
            
            //��ȡ�����Ϣ��������table name ��
            byte[] bPad = new byte[4*4];
            btTotal.get(bPad, 0, 4*4);
            //cut some 00
            byte[] bString = getStringByte(bPad);           
            String strPad = new String(bString, "gbk");  
            
			//��ȡ info
			btTotal.position(4 + 4 + 4 * 4);
            byte[] btInfo = new byte[nLen];
            btTotal.get(btInfo);
            //cut some 00
            bString = getStringByte(btInfo);            
            String strInfo = new String(bString, "gbk");
            
			//now we need nType, strInfo
			classifyInfo(nType, strPad, strInfo);
         }
         catch (IOException e)
         {
            e.printStackTrace();
         }
     }
     
     /*
      * �Խ��յ���Ϣ���ݽ��з��ദ��
      */
     public void classifyInfo(int nType, String strPad, String strInfo)
     {
         switch (nType)
         {
             case NetInfo.INFO_TYPE_UDP_OK:
             {
                 sendRequest(NetInfo.INFO_TYPE_GETTABLES, "".getBytes(), "get tbls");
                 break;
             }
             
             case NetInfo.INFO_TYPE_ALIAS:
             {
                 saveAlias(strInfo);
                 break;
             }
             
             case NetInfo.INFO_TYPE_TABLES:
             {
                 //just a hit
                 sendMessage("alertinfo", strInfo, -1);
                 
                 //now to get columns                 
                 String[] strTbls = strInfo.split("\\|");
                 for (String str : strTbls)
                {
                     sendRequest(NetInfo.INFO_TYPE_GETCOLUMNS,
                                 str.getBytes(),   //table name
                                 "get cols " + str);
                }
                 
                 //m_strTbls = strTbls;
                 //now we use alias instead
                 String[] strTblsAlias = strTbls;
                 int nLen = strTblsAlias.length;
                 for (int i = 0; i < nLen; i++)
                {
                     strTblsAlias[i] = m_map_ori_alias.get(strTbls[i]); 
                }
                 
                 //m_strTbls already be alias.
                 m_strTbls  = strTblsAlias;
                 m_bOK      = true;
                 
                 break;
             }
             
             case NetInfo.INFO_TYPE_COLUMNS:
             {
                 m_map_tbl_cols.put(strPad, strInfo); 
                 break;
             }
             
             case NetInfo.INFO_TYPE_UPDATETIME:
             {
                 //it's main thread's turn
                 sendMessage("setupdatetime", strInfo, NetInfo.INFO_TYPE_UPDATETIME);
                 break;
             }
             
             case NetInfo.INFO_TYPE_REPLY_SELECT:
             {
                 //Ӧ�÷�����Ϣ�������߳�������view
                 sendMessage("setselrecords", strInfo, NetInfo.INFO_TYPE_REPLY_SELECT);
                 break;
             }
             
             case NetInfo.INFO_TYPE_REPLY_SUCCESS:
             {
                 sendMessage("setNewRecords", "", UIActivity.MSG_KEY_CLEAR);
                 sendMessage("alertinfo", "�����ɹ�", -1);
                 break;
             }
             
             case NetInfo.INFO_TYPE_REPLY_ERROR:
             {
                 sendMessage("setNewRecords", "", UIActivity.MSG_KEY_CLEAR);
                 sendMessage("alertinfo", strInfo, -1);
                 break;
             }
         }
     }
     
    /*
     * �������ڽ������ݵ��߳�
     */
    public void createThread()
    {
        Thread t = new Thread()
        {
          public void run() 
          {   
              //��ʼ����������
              Init();
              
              while (true)
              {
                  recvInfo();
              }
          }
        };
        t.start();
    }
    
    /*
     * ��������̣߳������߳������в���
     */
    public void sendMessage(String strKey, String strInfo, int what)
    {
        Bundle data = new Bundle();
        data.putString(strKey, strInfo);
        
        Message msg = new Message();
        msg.setData(data);
        msg.what = what;
        if(-1 == what)
        {
            msg.what = ((UIActivity)m_context).MSG_KEY_INFO;
        }
        
        ((UIActivity)m_context).uiHandler.sendMessage(msg);
    }
    
    /*
     * establish the map between oriname and alias
     * strInfo looks like ori1|alias1|ori2|alias2|...
     */
    public void saveAlias(String strInfo)
    {
        String[] strOriAlias = strInfo.split("\\|");
        int nLen = strOriAlias.length;
        for (int i = 0; i < nLen; i += 2)
        {
            m_map_ori_alias.put(strOriAlias[i], strOriAlias[i+1]);
            m_map_alias_ori.put(strOriAlias[i+1], strOriAlias[i]);
        }
    }
    
    /*
     * return Alias according to ori
     */
    public String getAlias(String strOri)
    {
        return m_map_ori_alias.get(strOri);
    }
    
    /*
     * return oriname according to alias
     */
    public String getOri(String strAlias)
    {
        return m_map_alias_ori.get(strAlias);   
    }
    
    /*
     * get columns of a specific table
     */
    public String getColumns(String strTbl)
    {
        return m_map_tbl_cols.get(strTbl);
    }
}
