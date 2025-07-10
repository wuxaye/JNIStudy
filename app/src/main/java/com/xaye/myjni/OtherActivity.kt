package com.xaye.myjni

import android.content.pm.PackageManager
import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import com.xaye.myjni.databinding.ActivityOtherBinding
import com.xaye.myjni.jni.SignatureUtils

class OtherActivity : AppCompatActivity() {

    private lateinit var mBinding: ActivityOtherBinding
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        mBinding = ActivityOtherBinding.inflate(layoutInflater)
        setContentView(mBinding.root)

        // 参数加密，应该是在运行时架构中 HttpUtils,
        // 1. 对参数字典进行排序
        // 2. 生成待加密链接 userName=user&password=123456

        val signature = SignatureUtils.signatureParams("userName=240336124&userPwd=123456")

        // 作为参数传递给服务端， 服务端也会生成同样的密文，会对加密的字符串进行比较
        // 但是 加密方法被别人调试得到，so 被别的应用拿去使用，就被破解了，因为jni方法是不能被混淆的。
        mBinding.sampleText.text = signature

        val packageInfo = getPackageManager().getPackageInfo(packageName, PackageManager.GET_SIGNATURES);
        val signatures = packageInfo.signatures
        signatures[0].toCharsString();
        Log.i("OtherActivity", "signatures: "+signatures[0].toCharsString())
    }
}