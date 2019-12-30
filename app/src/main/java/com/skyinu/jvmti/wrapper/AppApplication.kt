package com.skyinu.jvmti.wrapper

import android.app.Application
import android.content.Context
import com.skyinu.jvmti.libwrapper.AgentUtil

class AppApplication : Application() {
    companion object{
        lateinit var APPCONTEXT: Application
    }

    override fun attachBaseContext(base: Context?) {
        super.attachBaseContext(base)
        System.loadLibrary("jvmti_wrapper")
        AgentUtil.loadAgent(this, "jvmti_wrapper", "this is options/**/")
    }

    override fun onCreate() {
        super.onCreate()
        APPCONTEXT = this
    }
}