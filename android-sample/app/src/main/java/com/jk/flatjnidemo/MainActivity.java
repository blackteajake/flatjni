package com.jk.flatjnidemo;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.TextView;

import com.github.jekinchen.flatjni.Greeter;
import com.github.jekinchen.flatjni.HelloReply;
import com.github.jekinchen.flatjni.SumResult;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final TextView tv = (TextView) findViewById(R.id.tv);
        tv.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                tv.setText("...");
                HelloReply replay = Greeter.hello("jk");
                SumResult result = Greeter.sum(1, 2);
                String text = replay != null ? replay.greeting() : "hello replay null";
                text += result != null ? ("sum(1,2)=" + result.result()) : ", sum reply null";
                tv.setText(text);
            }
        });
    }
}
