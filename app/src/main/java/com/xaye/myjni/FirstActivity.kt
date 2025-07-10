package com.xaye.myjni

import android.content.Intent
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.xaye.myjni.databinding.ActivityFirstBinding


class FirstActivity : AppCompatActivity() {

    private lateinit var mBinding: ActivityFirstBinding
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        mBinding = ActivityFirstBinding.inflate(layoutInflater)
        setContentView(mBinding.root)

        mBinding.btnMain.setOnClickListener {
            startActivity(Intent(this, MainActivity::class.java))
        }

        mBinding.btnOther.setOnClickListener {
            startActivity(Intent(this, OtherActivity::class.java))
        }
    }
}