package com.xaye.myjni

import android.app.Application
import com.xaye.myjni.jni.SignatureUtils

/**
 * 作者: xaye
 * 创建时间: 2025/7/10 22:49
 * 描述:
 */
class App : Application(){
    override fun onCreate() {
        super.onCreate()
        SignatureUtils.signatureVerify(this)
    }
}