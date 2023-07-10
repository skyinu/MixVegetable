package com.skyinu.jvmti.libwrapper

import androidx.annotation.Keep

@Keep
object NativeTiBridge {
    @Keep
    var applicationEventListener: ApplicationEventListener? = null

    @JvmStatic
    fun onThreadStart(name: String, daemon: String) {
        applicationEventListener?.onThreadStart(name, daemon)
    }

    @JvmStatic
    fun onThreadEnd(name: String, daemon: String) {
        applicationEventListener?.onThreadEnd(name, daemon)
    }

    @JvmStatic
    fun onClassLoad(threadName: String, classSign: String) {
        applicationEventListener?.onClassLoad(threadName, classSign)
    }

    @JvmStatic
    fun onClassPrepare(threadName: String, classSign: String) {
        applicationEventListener?.onClassPrepare(threadName, classSign)
    }

    @JvmStatic
    fun onMethodEntry(combinedMsg: String?) {
        applicationEventListener?.onMethodEntry(combinedMsg)
    }

    @JvmStatic
    fun onMethodExit(combinedMsg: String?, popByException: String) {
        applicationEventListener?.onMethodExit(combinedMsg, popByException)
    }

    @JvmStatic
    fun onCompiledMethodLoad(combinedMsg: String?, codeSize: String) {
        applicationEventListener?.onCompiledMethodLoad(combinedMsg, codeSize)
    }

    @JvmStatic
    fun onCompiledMethodUnload(combinedMsg: String?) {
        applicationEventListener?.onCompiledMethodUnload(combinedMsg)
    }

    @JvmStatic
    fun onGarbageCollectionStart() {
        applicationEventListener?.onGarbageCollectionStart()
    }

    @JvmStatic
    fun onGarbageCollectionFinish() {
        applicationEventListener?.onGarbageCollectionFinish()
    }

    @JvmStatic
    fun onObjectFree() {
        applicationEventListener?.onObjectFree()
    }

    @JvmStatic
    fun onMonitorWait(threadName: String?, timeout: String?, monitorObj: Any?) {
        applicationEventListener?.onMonitorWait(threadName, timeout, monitorObj)
    }

    @JvmStatic
    fun onMonitorWaited(threadName: String?, timeout: String?, monitorObj: Any?) {
        applicationEventListener?.onMonitorWaited(threadName, timeout, monitorObj)
    }

    @JvmStatic
    fun onMonitorContendedEnter(threadName: String?, monitorObj: Any?) {
        applicationEventListener?.onMonitorContendedEnter(threadName, monitorObj)
    }

    @JvmStatic
    fun onMonitorContendedEntered(threadName: String?, monitorObj: Any?) {
        applicationEventListener?.onMonitorContendedEntered(threadName, monitorObj)
    }

    @JvmStatic
    fun onDataDumpRequest() {
        applicationEventListener?.onDataDumpRequest()
    }

    @JvmStatic
    fun onResourceExhausted() {
        applicationEventListener?.onResourceExhausted()
    }

    @JvmStatic
    fun onDynamicCodeGenerated(name: String?) {
        applicationEventListener?.onDynamicCodeGenerated(name)
    }

    @JvmStatic
    fun onVMObjectAlloc(threadName: String?, name: String?) {
        applicationEventListener?.onVMObjectAlloc(threadName, name)
    }

    fun getObjectSize(obj: Any): Long {
        return NativeBridge.getObjSize(obj)
    }
}