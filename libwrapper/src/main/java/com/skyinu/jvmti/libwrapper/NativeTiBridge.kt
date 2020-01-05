package com.skyinu.jvmti.libwrapper

import android.util.Log

import androidx.annotation.Keep

@Keep
object NativeTiBridge {
    const val LOG_TAG = "TAG_WRAPPER"
    @JvmStatic
    fun onThreadStart(name: String, daemon: Boolean) {
        Log.e(LOG_TAG, "onThreadStart name = ${name}, demaon = $daemon")
    }

    @JvmStatic
    fun onThreadEnd(name: String, daemon: Boolean) {
        Log.e(LOG_TAG, "onThreadEnd name = ${name}, demaon = $daemon")
    }

    @JvmStatic
    fun onClassLoad(threadName: String, loadClass: Class<*>) {
    }
}