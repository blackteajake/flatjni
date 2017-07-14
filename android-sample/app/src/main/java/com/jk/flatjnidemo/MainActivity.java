package com.jk.flatjnidemo;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.TextView;

import com.github.jekinchen.flatjni.Greeter;
import com.github.jekinchen.flatjni.HelloReply;
import com.github.jekinchen.flatjni.SumResult;
import com.github.jekinchen.flatjni.User;

import java.nio.ByteBuffer;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final TextView tv = (TextView) findViewById(R.id.tv);
        tv.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                byte[] data = new byte[1024];
                tv.setText("...");
                HelloReply replay = Greeter.hello("jk");
                SumResult result = Greeter.sum(1, 2);
                User user = Greeter.echo(111, "jk", 34, true, data, 1234567);
                String text = replay != null ? replay.greeting() : "hello=null";
                text += "\n";
                text += result != null ? ("sum(1,2)=" + result.result()) : "sum=null";
                text += "\n";
                text += user != null ? "id:" + user.id() + "-name:" + user.name() + "-ts:"
                        + user.timestamp() + "-data check:"
                        + (ByteBuffer.wrap(data).equals(user.voiceDataAsByteBuffer())) : "user=null";
                tv.setText(text);
            }
        });
    }
}
