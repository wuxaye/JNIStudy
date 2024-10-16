package com.xaye.myjni;

import android.content.Context;
import android.util.Log;
import android.widget.Toast;

/**
 * Author xaye
 *
 * @date: 2024/9/30
 */
public class JNI2 {
    static {
        System.loadLibrary("mysecondlib");
    }

    public Context context;

    public JNI2(Context context) {
        this.context = context;
    }

    public void printHelloWorld() {
        Toast.makeText(context, "Hello World From Java!", Toast.LENGTH_SHORT).show();
    }

    public int add(int x, int y) {
        Log.d("JNI2", "add: " + (x + y));
        return x + y;
    }

    public void printString(String str) {
        Toast.makeText(context, str, Toast.LENGTH_SHORT).show();
    }

    public native void cCallJavaVoidMethod();
    public native void cCallJavaIntMethod();
    public native void cCallJavaStringMethod();

    public native String encode(String str);//加密

    public native String decode(String str);//解密

    public native void startCThread(); //C++ 创建线程

}

