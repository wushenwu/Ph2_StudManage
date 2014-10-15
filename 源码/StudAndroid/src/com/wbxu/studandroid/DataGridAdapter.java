package com.wbxu.studandroid;

import java.util.HashMap;
import java.util.Map;

import android.app.Activity;
import android.graphics.Color;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.EditText;
import android.widget.GridView;
import android.widget.TextView;

/*
 * We want to make this like a DataGrid 
 *  with the help with GridView + TextView ( EditText)
 */
public class DataGridAdapter extends BaseAdapter
implements TextWatcher
{
    private Activity m_Activity;
    
    private String   m_strCols;     //Header columns "col1|col2|"
    private String[] m_strCol;      //
    private String   m_strRecords;  //Info records "re1co1|re1co2|re2co1|re2co2|"
    private String[] m_strRecord;   //ary for all fields
    private int      m_nCols;
    
    //used to know which row we're processing
    private Map<EditText, Integer> m_map_edit_row = new HashMap<EditText, Integer>();
    private EditText[] m_EditTexts;
    private int        m_nEditTexts;
    public  static int m_nCurRow;
    
    public DataGridAdapter()
    {
    }
    
    /*
     * 根据pos获取列数
     */
    private int getCol(int pos)
    {      
        int col = pos % (m_nCols); 
        return col; 
    }
    
    /*
     * 根据pos获取行数
     */
    private int getRow(int pos)
    { 
        int row = pos / (m_nCols); 
        return row; 
    } 
    
    /*
     * 获取pos对应的item
     */
    private String positionToString(int pos) 
    { 
        int row = getRow(pos); 
        int col = getCol(pos); 
        if (row == 0) 
        { 
            //now we use alias
            //return m_strCol[col]; 
            String strAlias = "";
            strAlias = ((UIActivity)m_Activity).m_Net.getAlias(m_strCol[col]);
            return strAlias;
            
        }
        
        return m_strRecord[(row - 1)* m_nCols + col];
    }
    
    public DataGridAdapter(Activity context, GridView gView, String strCols, String strRecords)
    {
        m_Activity      = context;
        m_strRecords    = strRecords; 
        m_strRecord     = m_strRecords.split("\\|");
        
        m_strCols       = strCols;
        m_strCol        = m_strCols.split("\\|");
        m_nCols         = m_strCol.length;
        
        //
        m_nEditTexts    = 0; 
        m_EditTexts     = new EditText[m_strRecord.length * 2];
        m_nCurRow       = -1;
    }

    @Override
    public int getCount()
    {
        //所有字段记录 + 头部
        return m_strRecord.length + m_nCols;
    }

    @Override
    public Object getItem(int position)
    {
        return positionToString(position);
    }

    @Override
    public long getItemId(int position)
    {
        return getRow(position);
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent)
    {
        int row = getRow(position); 
        int col = getCol(position);
       
        String item = positionToString(position); 
        
        //确保是EditView或者TextView(头部）
        if (convertView instanceof EditText
            || convertView instanceof TextView)
        {
            return convertView;
        }
        
        View view;
        
        //对头部和第1列作特殊处理
        if (0 == row
            || 0 == col)  
        {
            view = new TextView(m_Activity);
            view.setBackgroundColor(Color.rgb(0x43,0x6E,0xEE));
            ((TextView)view).setTextColor(Color.rgb(0, 0, 0));
            ((TextView)view).setText(item, TextView.BufferType.NORMAL);
            
            //special considering for 1st col --ids
            if (row != 0
                && 0 == col)
            {
                m_strRecord[(row - 1) * m_nCols] = item;
                m_nEditTexts++;
            }
        }
        else
        {
            view = new EditText(m_Activity);
            m_EditTexts[m_nEditTexts++] = (EditText)view;
            m_map_edit_row.put((EditText)view, Integer.valueOf(row));
            
            ((EditText)view).setText(item, TextView.BufferType.NORMAL);
            
            //监控后续输入，以更新m_strRecord
            ((EditText)view).addTextChangedListener(this);
            
            //隐藏键盘
            ((EditText)view).setInputType(0); 
        }
        
        return view; 
    }

    @Override
    public void beforeTextChanged(CharSequence s, int start, int count,
            int after)
    {
        //nothing 
    }

    @Override
    public void onTextChanged(CharSequence s, int start, int before, int count)
    {
        //nothing
    }

    /*
     *接收用户的输入，并显示到"DataGrid"中
     */
    @Override
    public void afterTextChanged(Editable s)
    {        
        for (int i = 0; i < m_nEditTexts; i++)
        {
            //special condisering for 1st col --ids
            if (0 == i % m_nCols)
            {
                continue;
            }
            
            if(s != m_EditTexts[i].getText())
            {
                continue;
            }
            
            //
            if (m_nCurRow != -1
                && m_nCurRow != m_map_edit_row.get(m_EditTexts[i]))
            {
                //sth wrong, should submit the operation first
                //but this can also occured when setselrecords
//                ((UIActivity)m_Activity).m_Net.sendMessage("alertinfo",
//                                                    "请先提交或者放弃相关操作",
//                                                     -1);
                return;
            }
                       
            m_nCurRow      = m_map_edit_row.get(m_EditTexts[i]);
            m_strRecord[i] = s.toString(); 
        }//end for
    }
}
