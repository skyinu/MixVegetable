package com.skyinu.jvmti.wrapper

import android.app.Application

class AppApplication : Application() {
    companion object{
        lateinit var APPCONTEXT: Application
    }
    override fun onCreate() {
        super.onCreate()
        APPCONTEXT = this
    }
}