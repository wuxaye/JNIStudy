package com.xaye.myjni;

import java.lang.reflect.Array;

/**
 * Author xaye
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
