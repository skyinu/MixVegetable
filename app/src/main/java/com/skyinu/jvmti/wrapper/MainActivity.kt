package com.skyinu.jvmti.wrapper

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Process
import android.util.Log
import com.example.nativelib.NativeLib
import com.skyinu.jvmti.libwrapper.NativeTiBridge
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        Thread({
            while (true){
                Thread.sleep(10000)
                Log.e("TAG_WRAPPER", "TAG:" + Process.myPid() + " vs " + Any())
            }
        }, "dede").start()
        tvHello.setOnClickListener {
            tvHello.text = "${NativeTiBridge.getObjectSize(this)}"
            try {
                val result = NativeLib().stringFromJNI()
                Log.e("TAG_WRAPPER", "lib fork $result " + Process.myPid())
                clickHello()
            } catch (ignore: Exception) {
            }
        }
    }

    private fun clickHello() {
        hitException()
    }

    private fun hitException() {
        throw Exception()
    }
}
