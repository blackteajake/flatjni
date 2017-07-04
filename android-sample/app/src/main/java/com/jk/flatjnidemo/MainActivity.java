package com.jk.flatjnidemo;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;

import com.jk.greeter.HelloReply;
import com.jk.greeter.HelloRequest;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        HelloReply reply = HelloRequest.sayHello("kkkkkkkkkkkkkkkkkkkkk");
        Log.e("JK", "isOK=" + reply.isOK());
    }
}
