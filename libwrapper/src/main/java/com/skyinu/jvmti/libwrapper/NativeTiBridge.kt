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
//        Log.e(LOG_TAG, "onClassLoad classSign = $classSign")
    }

    @JvmStatic
    fun onClassPrepare(threadName: String, classSign: String) {
//        Log.e(LOG_TAG, "onClassPrepare classSign = $classSign")
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
    fun onCompiledMethodLoad(combinedMsg: String?, codeSize: String) {
        Log.e(LOG_TAG, "onCompiledMethodLoad $combinedMsg, codeSize $codeSize")
    }

    @JvmStatic
    fun onCompiledMethodUnload(combinedMsg: String?) {
        Log.e(LOG_TAG, "onCompiledMethodUnload $combinedMsg")
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

    @JvmStatic
    fun onMonitorWait(threadName: String?, timeout: String?, monitorObj: Any?) {
        Log.e(
            LOG_TAG,
            "onMonitorWait name = ${threadName}, monitorObj = $monitorObj timeout = $timeout"
        )
    }

    @JvmStatic
    fun onMonitorWaited(threadName: String?, timeout: String?, monitorObj: Any?) {
        Log.e(
            LOG_TAG,
            "onMonitorWaited name = ${threadName}, monitorObj = $monitorObj timeout = $timeout"
        )
    }

    @JvmStatic
    fun onMonitorContendedEnter(threadName: String?, monitorObj: Any?) {
        Log.e(
            LOG_TAG,
            "onMonitorContendedEnter name = ${threadName}, monitorObj = $monitorObj"
        )
    }

    @JvmStatic
    fun onMonitorContendedEntered(threadName: String?, monitorObj: Any?) {
        Log.e(
            LOG_TAG,
            "onMonitorContendedEntered name = ${threadName}, monitorObj = $monitorObj"
        )
    }

    @JvmStatic
    fun onDataDumpRequest() {
        Log.e(LOG_TAG, "onDataDumpRequest")
    }

    @JvmStatic
    fun onResourceExhausted() {
        Log.e(LOG_TAG, "onResourceExhausted")
    }

    @JvmStatic
    fun onDynamicCodeGenerated(name: String?) {
        Log.e(LOG_TAG, "onDynamicCodeGenerated $name")
    }

    @JvmStatic
    fun onVMObjectAlloc(threadName: String?, name: String?) {
        Log.e(LOG_TAG, "onVMObjectAlloc threadName = ${threadName}, name $name")
    }
}