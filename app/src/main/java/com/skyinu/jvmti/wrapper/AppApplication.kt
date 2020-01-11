package com.skyinu.jvmti.wrapper

import android.app.Application
import android.content.Context
import com.skyinu.jvmti.libwrapper.AgentUtil
import com.skyinu.jvmti.libwrapper.NativeTiBridge

class AppApplication : Application() {
    companion object {
        lateinit var APPCONTEXT: Application
    }

    override fun attachBaseContext(base: Context?) {
        super.attachBaseContext(base)
        AgentUtil.loadAgent(this, "jvmti_wrapper")
        NativeTiBridge.applicationEventListener = AppEventListenerImpl()
    }

    override fun onCreate() {
        super.onCreate()
        APPCONTEXT = this
    }
}