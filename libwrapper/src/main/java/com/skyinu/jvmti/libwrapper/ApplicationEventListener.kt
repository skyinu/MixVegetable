package com.skyinu.jvmti.libwrapper

interface ApplicationEventListener {
    fun onThreadStart(name: String, daemon: String)

    fun onThreadEnd(name: String, daemon: String)

    fun onClassLoad(threadName: String, classSign: String)

    fun onClassPrepare(threadName: String, classSign: String)

    fun onMethodEntry(combinedMsg: String?)

    fun onMethodExit(combinedMsg: String?, popByException: String)

    fun onCompiledMethodLoad(combinedMsg: String?, codeSize: String)

    fun onCompiledMethodUnload(combinedMsg: String?)

    fun onGarbageCollectionStart()

    fun onGarbageCollectionFinish()

    fun onObjectFree()

    fun onMonitorWait(threadName: String?, timeout: String?, monitorObj: Any?)

    fun onMonitorWaited(threadName: String?, timeout: String?, monitorObj: Any?)

    fun onMonitorContendedEnter(threadName: String?, monitorObj: Any?)

    fun onMonitorContendedEntered(threadName: String?, monitorObj: Any?)

    fun onDataDumpRequest()

    fun onResourceExhausted()

    fun onDynamicCodeGenerated(name: String?)

    fun onVMObjectAlloc(threadName: String?, name: String?)
}