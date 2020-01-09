package com.skyinu.jvmti.wrapper

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        Thread({}, "dede").start()
        tvHello.setOnClickListener {
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
