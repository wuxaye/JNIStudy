package com.xaye.myjni.jni;

import java.lang.reflect.Array;

/**
 * Author xaye
 *
 * Java与C++ 数据传递
 *
 * @date: 2024/9/30
 */
public class JNI {
    static {
        System.loadLibrary("myjni");
    }

    public native int passInt(int x,int y);

    public native String passString(String str);

    public native int[] passArray(int[] array);
}
