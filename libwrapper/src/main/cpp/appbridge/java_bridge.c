//
// Created by skyinu on 2020/1/11.
//

#include <malloc.h>
#include "java_bridge.h"
#include "string.h"

jclass theBridgeClass = NULL;
jmethodID *bridgeMethods = NULL;

void initBridgeMethod(JNIEnv *jni_env) {
    theBridgeClass = (*jni_env)->NewGlobalRef(jni_env,
                                              (*jni_env)->FindClass(jni_env, BRIDGE_CLASS));
    bridgeMethods = malloc(sizeof(jmethodID) * BRIDGE_METHOD_COUNT);
    memset(bridgeMethods, NULL, BRIDGE_METHOD_COUNT);
    bridgeMethods[MSG_TYPE_CLASSLODE] = (*jni_env)->GetStaticMethodID(
            jni_env,
            theBridgeClass,
            "onClassLoad",
            "(Ljava/lang/String;Ljava/lang/String;)V");
    bridgeMethods[MSG_TYPE_CLASSPREPARE] = (*jni_env)->GetStaticMethodID(
            jni_env,
            theBridgeClass,
            "onClassPrepare",
            "(Ljava/lang/String;Ljava/lang/String;)V");
    bridgeMethods[MSG_TYPE_METHOD_ENTRY] = (*jni_env)->GetStaticMethodID(
            jni_env,
            theBridgeClass,
            "onMethodEntry",
            "(Ljava/lang/String;)V");
    bridgeMethods[MSG_TYPE_GARBAGE_START] = (*jni_env)->GetStaticMethodID(
            jni_env,
            theBridgeClass,
            "onGarbageCollectionStart",
            "()V");
    bridgeMethods[MSG_TYPE_GARBAGE_FINISH] = (*jni_env)->GetStaticMethodID(
            jni_env,
            theBridgeClass,
            "onGarbageCollectionFinish",
            "()V");
    bridgeMethods[MSG_TYPE_OBJECT_FREE] = (*jni_env)->GetStaticMethodID(
            jni_env,
            theBridgeClass,
            "onObjectFree",
            "()V");
    bridgeMethods[MSG_TYPE_THREAD_START] = (*jni_env)->GetStaticMethodID(
            jni_env,
            theBridgeClass,
            "onThreadStart",
            "(Ljava/lang/String;Ljava/lang/String;)V");
    bridgeMethods[MSG_TYPE_THREAD_END] = (*jni_env)->GetStaticMethodID(
            jni_env,
            theBridgeClass,
            "onThreadEnd",
            "(Ljava/lang/String;Ljava/lang/String;)V");
    bridgeMethods[MSG_TYPE_METHOD_EXIT] = (*jni_env)->GetStaticMethodID(
            jni_env,
            theBridgeClass,
            "onMethodExit",
            "(Ljava/lang/String;Ljava/lang/String;)V");
    bridgeMethods[MSG_TYPE_COMPILED_METHOD_LOAD] = (*jni_env)->GetStaticMethodID(
            jni_env,
            theBridgeClass,
            "onCompiledMethodLoad",
            "(Ljava/lang/String;Ljava/lang/String;)V");

    bridgeMethods[MSG_TYPE_COMPILED_METHOD_UNLOAD] = (*jni_env)->GetStaticMethodID(
            jni_env,
            theBridgeClass,
            "onCompiledMethodUnload",
            "(Ljava/lang/String;)V");
    bridgeMethods[MSG_TYPE_MONITOR_WAIT] = (*jni_env)->GetStaticMethodID(
            jni_env,
            theBridgeClass,
            "onMonitorWait",
            "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/Object;)V");
    bridgeMethods[MSG_TYPE_MONITOR_WAITED] = (*jni_env)->GetStaticMethodID(
            jni_env,
            theBridgeClass,
            "onMonitorWaited",
            "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/Object;)V");
    bridgeMethods[MSG_TYPE_MONITOR_CONTENDED_ENTER] = (*jni_env)->GetStaticMethodID(
            jni_env,
            theBridgeClass,
            "onMonitorContendedEnter",
            "(Ljava/lang/String;Ljava/lang/Object;)V");
    bridgeMethods[MSG_TYPE_MONITOR_CONTENDED_ENTERED] = (*jni_env)->GetStaticMethodID(
            jni_env,
            theBridgeClass,
            "onMonitorContendedEntered",
            "(Ljava/lang/String;Ljava/lang/Object;)V");
    bridgeMethods[MSG_TYPE_DATA_DUMP_REQUEST] = (*jni_env)->GetStaticMethodID(
            jni_env,
            theBridgeClass,
            "onDataDumpRequest",
            "()V");
    bridgeMethods[MSG_TYPE_RESOURCE_EXHAUSTED] = (*jni_env)->GetStaticMethodID(
            jni_env,
            theBridgeClass,
            "onResourceExhausted",
            "()V");
}