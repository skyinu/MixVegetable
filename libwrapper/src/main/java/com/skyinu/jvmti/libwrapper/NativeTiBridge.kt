package com.skyinu.jvmti.libwrapper

import android.util.Log

import androidx.annotation.Keep

@Keep
object NativeTiBridge {
    @JvmStatic
    fun onThreadStart(name: String) {
        Log.e("TAG1", "onThreadStart")
    }
}