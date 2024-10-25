package com.xaye.myjni.jni;

import android.util.Log;

/**
 * Author xaye
 *
 * @date: 2024/10/22
 */
public class SerialPort {

    static {
        System.loadLibrary("serialportlib");
    }

    // 串口句柄
    private int mNativeContext; // 用于存储本地（C++）层的上下文指针

    private volatile boolean receiving = false; // 控制接收线程的标志

    // 打开串口
    public native boolean open(String path, int baudRate, int dataBits, int stopBits, char parity);

    // 关闭串口
    public native void close();

    // 发送数据
    public native void write(byte[] data);

    // 读取数据
    public native byte[] read();

    // 启动接收线程
    public void startReceiving() {
        receiving = true; // 设置接收状态为真
        new Thread(() -> {
            while (receiving) {
                byte[] data = read();
                if (data != null && data.length > 0) {
                    processData(data);
                }
                try {
                    Thread.sleep(100); // 控制读取频率
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt(); // 恢复中断状态
                }
            }
        }).start();
    }

    // 关闭串口并停止接收线程
    public void stopReceiving() {
        receiving = false; // 设置接收状态为假
        close(); // 关闭串口
    }


    // 处理接收到的数据
    private void processData(byte[] data) {
        Log.i("SerialPort", " processData = " + bytesToHex(data));
    }

    public static String bytesToHex(byte[] data) {
        StringBuilder hexString = new StringBuilder();
        for (byte b : data) {
            String hex = Integer.toHexString(b & 0xFF); // 确保为正值
            if (hex.length() == 1) {
                hexString.append('0'); // 补零
            }
            hexString.append(hex);
        }
        return hexString.toString();
    }


}
