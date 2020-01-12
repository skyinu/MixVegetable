package com.skyinu.jvmti.wrapper

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.skyinu.jvmti.libwrapper.NativeTiBridge
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        Thread({}, "dede").start()
        tvHello.setOnClickListener {
            tvHello.text = "${NativeTiBridge.getObjectSize(this)}"
            try {
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
