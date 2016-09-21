package com.samsung.android.sdk.accessory.example.helloaccessory.provider;

import android.app.Activity;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends Activity {

    static public TextView tv;
    static final Handler mHandler = new Handler();

    static public void setText(String str)
    {
        final String srtt = str;
        mHandler.post(new Runnable() {
            @Override
            public void run() {
                tv.setText(srtt);
            }
        });
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        tv = (TextView)findViewById(R.id.textView4);
    }
}
