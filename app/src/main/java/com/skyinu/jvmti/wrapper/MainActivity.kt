package com.skyinu.jvmti.wrapper

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.skyinu.jvmti.libwrapper.AgentUtil

class MainActivity : AppCompatActivity() {
    companion object {
        init {
            System.loadLibrary("jvmti_wrapper")
            AgentUtil.loadAgent(AppApplication.APPCONTEXT, "jvmti_wrapper", "this is options/**/")
        }
    }


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
    }
}
