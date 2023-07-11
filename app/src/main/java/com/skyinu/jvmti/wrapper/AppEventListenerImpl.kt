package com.skyinu.jvmti.wrapper

import android.util.Log
import com.skyinu.jvmti.libwrapper.ApplicationEventListener

class AppEventListenerImpl :ApplicationEventListener{
    companion object {
        const val LOG_TAG = "TAG_WRAPPER"
    }
    private var methodCall = 0L
    override fun onThreadStart(name: String, daemon: String) {
        Log.e(LOG_TAG, "onThreadStart name = ${name}, demaon = $daemon")
    }

    override fun onThreadEnd(name: String, daemon: String) {
        Log.e(LOG_TAG, "onThreadEnd name = ${name}, demaon = $daemon")
    }

    override fun onClassLoad(threadName: String, classSign: String) {
//        Log.e(LOG_TAG, "onClassLoad classSign = $classSign")
    }

    override fun onClassPrepare(threadName: String, classSign: String) {
//        Log.e(LOG_TAG, "onClassPrepare classSign = $classSign")
    }

    override fun onMethodEntry(combinedMsg: String?) {
//        Log.e(LOG_TAG, combinedMsg)
        methodCall++
    }

    override fun onMethodExit(combinedMsg: String?, popByException: String) {
//        Log.e(LOG_TAG, "onMethodExit $combinedMsg, popByException $popByException")
    }

    override fun onCompiledMethodLoad(combinedMsg: String?, codeSize: String) {
        Log.e(LOG_TAG, "onCompiledMethodLoad $combinedMsg, codeSize $codeSize")
    }

    override fun onCompiledMethodUnload(combinedMsg: String?) {
        Log.e(LOG_TAG, "onCompiledMethodUnload $combinedMsg")
    }

    override fun onGarbageCollectionStart() {
        Log.e(LOG_TAG, "onGarbageCollectionStart")
    }

    override fun onGarbageCollectionFinish() {
        Log.e(LOG_TAG, "onGarbageCollectionFinish")
    }

    override fun onObjectFree() {
        Log.e(LOG_TAG, "onObjectFree")
    }

    override fun onMonitorWait(threadName: String?, timeout: String?, monitorObj: Any?) {
        Log.e(
            LOG_TAG,
            "onMonitorWait name = ${threadName}, monitorObj = $monitorObj timeout = $timeout"
        )
    }

    override fun onMonitorWaited(threadName: String?, timeout: String?, monitorObj: Any?) {
        Log.e(
            LOG_TAG,
            "onMonitorWaited name = ${threadName}, monitorObj = $monitorObj timeout = $timeout"
        )
    }

    override fun onMonitorContendedEnter(threadName: String?, monitorObj: Any?) {
        Log.e(
            LOG_TAG,
            "onMonitorContendedEnter name = ${threadName}, monitorObj = $monitorObj"
        )
    }

    override fun onMonitorContendedEntered(threadName: String?, monitorObj: Any?) {
        Log.e(
            LOG_TAG,
            "onMonitorContendedEntered name = ${threadName}, monitorObj = $monitorObj"
        )
    }

    override fun onDataDumpRequest() {
        Log.e(LOG_TAG, "onDataDumpRequest")
    }

    override fun onResourceExhausted() {
        Log.e(LOG_TAG, "onResourceExhausted")
    }

    override fun onDynamicCodeGenerated(name: String?) {
        Log.e(LOG_TAG, "onDynamicCodeGenerated $name")
    }

    override fun onVMObjectAlloc(threadName: String?, name: String?) {
//        Log.e(LOG_TAG, "onVMObjectAlloc threadName = ${threadName}, name $name")
    }
}