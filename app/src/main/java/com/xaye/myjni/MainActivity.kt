package com.xaye.myjni

import android.os.Bundle
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.xaye.myjni.databinding.ActivityMainBinding
import java.util.Arrays

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    private lateinit var jni: JNI

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        jni = JNI()

        binding.btnInt.setOnClickListener {
            val result = jni.passInt(3,4)
            Toast.makeText(this,result.toString(),Toast.LENGTH_SHORT).show()
        }

        binding.btnString.setOnClickListener {
            val result = jni.passString("hello world!")
            Toast.makeText(this,result,Toast.LENGTH_SHORT).show()
        }

        binding.btnArray.setOnClickListener {
            val result = jni.passArray(intArrayOf(1,2,3))
            Toast.makeText(this, result.contentToString(),Toast.LENGTH_SHORT).show()
        }
    }


}