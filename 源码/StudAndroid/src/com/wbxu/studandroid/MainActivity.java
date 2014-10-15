package com.wbxu.studandroid;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.widget.Button;
import android.widget.EditText;
import android.view.View;

public class MainActivity extends Activity 
implements View.OnClickListener
{   
    private String m_strIP;
    private int    m_nPort;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        EditText editIP     = (EditText)findViewById(R.id.editIP);
        EditText editPort   = (EditText)findViewById(R.id.editPort);
        Button   btnNetInit = (Button)findViewById(R.id.btnInit);
        btnNetInit.setOnClickListener(this);
                
        m_strIP     = editIP.getText().toString();
        String strPort   = editPort.getText().toString();
        m_nPort        = Integer.valueOf(strPort);  
    }
    
    @Override
    public void onClick(View v)
    {
        //开启新Activity, 进行网络处理
        if (R.id.btnInit == v.getId())
        {
            Bundle data = new Bundle();
            data.putString("IP", m_strIP);
            data.putInt("Port", m_nPort);
            
            Intent intent = new Intent(this, UIActivity.class);
            intent.putExtra("IP_Port", data);
            
            startActivity(intent);
        }
    }
}
