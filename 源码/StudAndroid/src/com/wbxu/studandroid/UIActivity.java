package com.wbxu.studandroid;

import java.nio.ByteBuffer;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.SystemClock;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.GridView;
import android.widget.Spinner;
import android.widget.Toast;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.WindowManager;

public class UIActivity extends Activity
implements OnItemSelectedListener, OnClickListener, OnItemClickListener
{   
    public final static int MSG_KEY_INFO = 0x123;
    public final static int MSG_KEY_CLEAR= (MSG_KEY_INFO + 1);
    private final static String PAD      = "*";
    public static NetProcess m_Net       = null;
    
    //views related
    private DataGridAdapter m_dataDataGridAdapter = null; 
    private Spinner m_spinTbls                    = null;             
    private GridView m_dataGrid                   = null;
    
    //operation related
    private final int OP_NONE   = 0;
    private final int OP_SUBMIT = 1;
    private int    m_nCurOpCode = OP_NONE;
    private int    m_nID        = -1;       //ID of a data record, from DB     
    
    //
    private String m_strCurTbl  = "";
    private String m_strCurCols = "";
    private int    m_nCols      = 0;
    
    //owned by MainThread, used to alert info, update views, etc
    public Handler uiHandler = new Handler() 
    {
        @Override
        public void handleMessage(Message msg) 
        {
            updateView(msg);
        }
    };
    
    public void updateView(Message msg)
    {
        Bundle data = msg.getData();
        
        switch (msg.what)
        {
            case NetInfo.INFO_TYPE_REPLY_SELECT:
            {
                setSelResults(data.getString("setselrecords"));
                break;
            }
            
            case MSG_KEY_INFO:
            {
                Toast.makeText(this, 
                        data.getString("alertinfo"), 
                        Toast.LENGTH_LONG
                        ).show();
                break;
            }
            
            case MSG_KEY_CLEAR:
            {
                setNewRecords();
                break;
            }

            default:
                break;
        }
    }
       
    /*
     *started by MainActivity
     *create thread, 
     *  interact with User, 
     *  and recv data, request MainThread to update sth
     */
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_process);
        
        Intent intent = getIntent();
        Bundle data   = intent.getBundleExtra("IP_Port");       
        m_Net = new NetProcess(this, 
                                data.getString("IP"), 
                                data.getInt("Port")
                                );
        m_Net.createThread();
        
        //just wait items for Spinner is ok.
        while(!NetProcess.m_bOK)
        {
            SystemClock.sleep(10);
        }
        
        //findviews and setlistener
        operateInit();
        
        //to make not lose input focus
        getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_ADJUST_PAN);
    }
    
    public void operateInit()
    {
        //get thz views
        Button btnSearch = (Button)findViewById(R.id.btnSearch);
        Button btnAdd = (Button)findViewById(R.id.btnAdd);
        Button btnDel = (Button)findViewById(R.id.btnDel);
        Button btnUpdate = (Button)findViewById(R.id.btnUpdate);
        Button btnSubmit = (Button)findViewById(R.id.btnSubmit);
        Button btnCancel = (Button)findViewById(R.id.btnCancel);
        
        btnSearch.setOnClickListener(this);
        btnAdd.setOnClickListener(this);
        btnDel.setOnClickListener(this);
        btnUpdate.setOnClickListener(this);
        btnSubmit.setOnClickListener(this);
        btnCancel.setOnClickListener(this);
        
        m_spinTbls = (Spinner)findViewById(R.id.spinnerTbl);
        m_dataGrid = (GridView)findViewById(R.id.dataGrid);
        m_dataGrid.setOnItemClickListener(this);
       
        ArrayAdapter<String> adapter = 
                new ArrayAdapter<String>(this, 
                                         R.layout.activity_item, 
                                         NetProcess.m_strTbls       //m_strTbls already being alias
                                         );
        m_spinTbls.setAdapter(adapter);
        m_spinTbls.setOnItemSelectedListener(this);
    }

    /*
     * Responding to User Selection
     * Set Header Columns in "DataGrid" ( GridView + TextView (EditText))
     */
    @Override
    public void onItemSelected(AdapterView<?> parent, 
            View view, 
            int position,
            long id)
    {
        //the strTbl already being aliased, like '班级表'
        String strTbl = (String)parent.getItemAtPosition(position);   
        
        //strCols still being oriname
        String strCols = m_Net.getColumns(m_Net.getOri(strTbl));
        if (null == strCols)
        {
            return;
        }
        
        //m_strCurTbl already being aliased, m_strCurCols being oriname
        m_strCurTbl = strTbl;
        m_strCurCols = strCols;
        
        //set columns for this table
        m_nCols  = strCols.split("\\|").length;
        m_dataGrid.setNumColumns(m_nCols); 
        m_dataGrid.setStretchMode(GridView.STRETCH_COLUMN_WIDTH);

        m_dataDataGridAdapter = new DataGridAdapter(this,
                                m_dataGrid,
                                strCols,    //oriname
                                repeateString(PAD + "|", m_nCols)
                                );
        
        m_dataGrid.setAdapter(m_dataDataGridAdapter);
        
    }

    @Override
    public void onNothingSelected(AdapterView<?> parent)
    {
        //nothing to do
    }
    
    /*
     * str ===> strstrstr.....
     */
    protected String repeateString(String str, int nCount)
    {
        String strRet = "";
        for(int i = 0; i < nCount; i++)
        {
            strRet += str;
        }
        return strRet;
    }
   
    /*
     * To indicate user to input sth,
     */
    protected void setNewRecords()
    {
        m_dataDataGridAdapter = new DataGridAdapter(this,
                                    m_dataGrid,
                                    m_strCurCols,
                                    repeateString(PAD + "|", m_nCols)
                                    );

        m_dataGrid.setAdapter(m_dataDataGridAdapter);
    }

    @Override
    public void onClick(View v)
    {
        switch (v.getId())
        {
            case R.id.btnSearch:
            {
                m_nCurOpCode = NetInfo.INFO_TYPE_SELECT;
                setNewRecords();
                break;
            }
            
            case R.id.btnAdd:
            {
                m_nCurOpCode = NetInfo.INFO_TYPE_INSERT;
                setNewRecords();
                break;
            }
            
            case R.id.btnDel:
            {
                m_nCurOpCode = NetInfo.INFO_TYPE_DEL;
                alertInfo("请单击id列，然后提交");
                break;
            }
            
            case R.id.btnUpdate:
            {
                m_nCurOpCode = NetInfo.INFO_TYPE_UPDATE;
                alertInfo("请修改某行信息，然后提交");
                break;
            }
            
            case R.id.btnCancel:
            {
                m_nCurOpCode = OP_NONE;
                break;
            }
            
            case R.id.btnSubmit:
            {   
                processSubmit();
                break;
            }
        }   
    }
        
    /*
     *  we need pad with the Operation Type, and the current table
     *  struct sockaddr pad = {NULL};
        memcpy(&pad, &pReq->m_Operation, sizeof(long));
        strcpy((char *)&pad + sizeof(long), pReq->m_strTbl.GetBuffer(0));
     */
    protected void processSubmit()
    {
        if (OP_NONE == m_nCurOpCode)
        {
            alertInfo("尚未选择相关操作");
            return;
        }
        
        if (NetInfo.INFO_TYPE_DEL == m_nCurOpCode
            && -1 == m_nID)
        {
            alertInfo("请点击某行的id 进行选择");
            return;
        }
        
        //get a row of item from DataGrid
        String strReq = getRecord();
        
        //构建strPad:  nOpcode, table name
        ByteBuffer byteBuffer = ByteBuffer.allocate(4 * 4);
        byteBuffer.put(NetProcess.toBytes(m_nCurOpCode));
        
        //m_strCurTbl being alias
        //byteBuffer.put(m_strCurTbl.getBytes());
        byteBuffer.put(m_Net.getOri(m_strCurTbl).getBytes());
        
        byte[] bPad = new byte[4 * 4];
        byteBuffer.position(0);
        byteBuffer.get(bPad, 0, 4 * 4);
        
        //now send our req
        m_Net.sendRequest(NetInfo.INFO_TYPE_REQUEST,
                        bPad, 
                        strReq);
    }

    /*
     * get the row info currently processed.
     */
    public String getRecord()
    {
        String strRecord = "";
        String strCol    = "";
        int nRow = m_dataDataGridAdapter.m_nCurRow;
        for(int i = 0; i < m_nCols; i++)
        {
            strCol = (String)m_dataDataGridAdapter.getItem(nRow * m_nCols + i);
            if (0 == strCol.length()
                || 0 == strCol.compareTo(PAD))
            {
                continue;
            }
            
            //m_strCurCols being oriname
            strRecord += m_strCurCols.split("\\|")[i];
            strRecord += "|";
            strRecord += strCol;
            strRecord += "|";            
        }
        
        alertInfo(strRecord);
        
        return strRecord;
    }
    
    /*
     * Update "DataGrid" with the queryed results
     */
    public void setSelResults(String strRet)
    {
        m_dataDataGridAdapter = new DataGridAdapter(this,
                                            m_dataGrid,
                                            m_strCurCols,
                                            strRet
                                            );

        m_dataGrid.setAdapter(m_dataDataGridAdapter);
    }
    
    public void alertInfo(String strInfo)
    {
        Toast.makeText(this, strInfo, Toast.LENGTH_LONG).show();
    }

    @Override
    public void onItemClick(AdapterView<?> parent, View view, int position,
            long id)
    {
        //thz only function with TextView, we've already customed the Grid
        //we really want to know the EditText content in this way, but actually cannot.
        //so we just add TextWatcher for every EditText in DataGridAdapter
        
        //as for the 1st colums, they are TextViewed, so here we go.
        if (m_nCurOpCode != NetInfo.INFO_TYPE_DEL)
        {
            return;
        }
        
        if (0 == m_dataDataGridAdapter.getItemId(position))
        {
            return;
        }
        
        String strID = (String)m_dataDataGridAdapter.getItem(position);
        if (0 == strID.compareTo(PAD))
        {
            return;
        }
        
        m_nID        = Integer.valueOf(strID);
    }
}
