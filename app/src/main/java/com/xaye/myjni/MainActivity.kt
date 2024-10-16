package com.xaye.myjni

import android.os.Bundle
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.xaye.myjni.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    private lateinit var jni: JNI

    private lateinit var jni2: JNI2

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        jni = JNI()

        jni2 = JNI2(applicationContext)
        //传递int
        binding.btnInt.setOnClickListener {
            val result = jni.passInt(3,4)
            Toast.makeText(this,result.toString(),Toast.LENGTH_SHORT).show()
        }

        //传递String
        binding.btnString.setOnClickListener {
            val result = jni.passString("hello world!")
            Toast.makeText(this,result,Toast.LENGTH_SHORT).show()
        }

        //传递数组
        binding.btnArray.setOnClickListener {
            val result = jni.passArray(intArrayOf(1,2,3))
            Toast.makeText(this, result.contentToString(),Toast.LENGTH_SHORT).show()
        }

        //-------------------------------------------------------
        //调用Java的void方法
        binding.btnFunVoid.setOnClickListener {
            jni2.cCallJavaVoidMethod()
        }

        //调用Java的int方法
        binding.btnFunInt.setOnClickListener {
             jni2.cCallJavaIntMethod()
        }

        //调用Java的String方法
        binding.btnFunString.setOnClickListener {
            jni2.cCallJavaStringMethod()
        }

        //-------------------------------------------------------

        //看具体代码，就是简单的地址转换，了解就行，因为这种会出现非法地址问题
        //解密
        binding.btnDecode.setOnClickListener {
            var psd = binding.etView.text.toString()
            psd = jni2.decode(psd)
            binding.etView.setText(psd)
        }
        //加密
        binding.btnEncode.setOnClickListener {
            var psd = binding.etView.text.toString()
            psd = jni2.encode(psd)
            binding.etView.setText(psd)
        }

        //-------------------------------------------------------

        binding.btnStart.setOnClickListener {
            Thread{
                startMonitor()
            }.start()
        }

        binding.btnStop.setOnClickListener {
            stopMonitor()
        }

        //-------------------------------------------------------

        binding.btnCreateThread.setOnClickListener {
            jni2.startCThread()
        }
    }

    private external fun startMonitor()

    private external fun stopMonitor()

    public fun updatePressure(value : Int) {
        runOnUiThread {
            binding.pb.progress = value
        }
    }




}