package com.xaye.myjni.jni;

import android.content.Context;

/**
 * 作者: xaye
 * 创建时间: 2025/7/2 22:36
 * 修改时间: 2025/7/2 22:36
 * 描述:
 */
public class SignatureUtils {
    static {
        System.loadLibrary("otherlib");
    }

    public static native String signatureParams(String params);

    /**
     * 签名验证
     */
    public static native String signatureVerify(Context context);
}
