package com.xaye.myjni;

/**
 * Author xaye
 *
 * @date: 2024/10/22
 */
public class JNI3 {

    static {
        System.loadLibrary("pclib");
    }


    public native long startProducerConsumer();
    public native void stopProducerConsumer(long ptr);
}
