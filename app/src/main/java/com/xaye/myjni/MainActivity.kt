package com.xaye.myjni

import android.os.Bundle
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.xaye.myjni.databinding.ActivityMainBinding
import com.xaye.myjni.jni.JNI
import com.xaye.myjni.jni.JNI2
import com.xaye.myjni.jni.JNI3
import com.xaye.myjni.jni.SerialPort
import com.xaye.myjni.util.HexUtil.hexToByteArray

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    private lateinit var jni: JNI

    private lateinit var jni2: JNI2

    private lateinit var jnI3: JNI3

    private lateinit var serialPort: SerialPort


    private var producerConsumerPtr: Long = 0

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        jni = JNI()

        jni2 = JNI2(applicationContext)

        jnI3 = JNI3()

        serialPort = SerialPort()
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

        //-------------------------------------------------------

        binding.btnPcStart.setOnClickListener {
            producerConsumerPtr = jnI3.startProducerConsumer()
        }

        binding.btnPcStop.setOnClickListener {
            jnI3.stopProducerConsumer(producerConsumerPtr)
        }

        //-------------------------------------------------------

        binding.btnPortOpen.setOnClickListener {
           val result = serialPort.open("/dev/ttyS3", 9600, 8, 1, 'N')

            Toast.makeText(this, "打开: $result", Toast.LENGTH_SHORT).show()

            serialPort.startReceiving()

        }

        binding.btnPortSend.setOnClickListener {
            val bytes = hexToByteArray("737461728A0101119b656E646F")
            serialPort.write(bytes)
        }

        binding.btnPortStop.setOnClickListener {
            serialPort.stopReceiving()
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