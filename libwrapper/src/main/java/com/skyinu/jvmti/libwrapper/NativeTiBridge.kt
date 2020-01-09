package com.skyinu.jvmti.libwrapper

import android.util.Log

import androidx.annotation.Keep

@Keep
object NativeTiBridge {
    const val LOG_TAG = "TAG_WRAPPER"
    private var methodCall = 0L
    @JvmStatic
    fun onThreadStart(name: String, daemon: String) {
        Log.e(LOG_TAG, "onThreadStart name = ${name}, demaon = $daemon")
    }

    @JvmStatic
    fun onThreadEnd(name: String, daemon: String) {
        Log.e(LOG_TAG, "onThreadEnd name = ${name}, demaon = $daemon")
    }

    @JvmStatic
    fun onClassLoad(threadName: String, classSign: String) {
        Log.e(LOG_TAG, "onClassLoad classSign = $classSign")
    }

    @JvmStatic
    fun onClassPrepare(threadName: String, classSign: String) {
        Log.e(LOG_TAG, "onClassPrepare classSign = $classSign")
    }

    @JvmStatic
    fun onMethodEntry(combinedMsg: String?) {
//        Log.e(LOG_TAG, combinedMsg)
        methodCall++
    }

    @JvmStatic
    fun onMethodExit(combinedMsg: String?, popByException: String) {
//        Log.e(LOG_TAG, "onMethodExit $combinedMsg, popByException $popByException")
    }

    @JvmStatic
    fun onGarbageCollectionStart() {
        Log.e(LOG_TAG, "onGarbageCollectionStart")
    }

    @JvmStatic
    fun onGarbageCollectionFinish() {
        Log.e(LOG_TAG, "onGarbageCollectionFinish")
    }

    @JvmStatic
    fun onObjectFree() {
        Log.e(LOG_TAG, "onObjectFree")
    }
}