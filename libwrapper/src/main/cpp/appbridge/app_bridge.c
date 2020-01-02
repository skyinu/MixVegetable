//
// Created by skyinu on 2019/12/30.
//

#include "app_bridge.h"

#define BRIDGE_CLASS "com/skyinu/jvmti/libwrapper/NativeTiBridge"
jvmtiEnv *globalJvmtiEnv;
jclass theBridgeClass = NULL;

void setUpEnv(jvmtiEnv *jvmti_env) {
    globalJvmtiEnv = jvmti_env;
}

jclass findBridgeClass(JNIEnv *jni_env) {
    if (theBridgeClass != NULL) {
        return theBridgeClass;
    }
    theBridgeClass = (*jni_env)->FindClass(jni_env, BRIDGE_CLASS);
    return theBridgeClass;
}

void notifyThreadStart(JNIEnv *jni_env, jthread thread) {
    jvmtiThreadInfo info;
    (*globalJvmtiEnv)->GetThreadInfo(globalJvmtiEnv, thread, &info);
    jclass bridgeClass = findBridgeClass(jni_env);
}

