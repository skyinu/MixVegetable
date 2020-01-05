//
// Created by skyinu on 2019/12/30.
//

#include "app_bridge.h"

#define BRIDGE_CLASS "com/skyinu/jvmti/libwrapper/NativeTiBridge"
jvmtiEnv *globalJvmtiEnv;
jclass theBridgeClass = NULL;
jmethodID onThreadStart;

void initBridgeConfig(JNIEnv *jni_env) {
    theBridgeClass = (*jni_env)->NewGlobalRef(jni_env,
                                              (*jni_env)->FindClass(jni_env, BRIDGE_CLASS));
    onThreadStart = (*jni_env)->GetStaticMethodID(jni_env, theBridgeClass, "onThreadStart",
                                                  "(Ljava/lang/String;)V");
}

void setUpEnv(jvmtiEnv *jvmti_env) {
    globalJvmtiEnv = jvmti_env;
}

void setUpJNIEnv(JNIEnv *jni_env) {
    initBridgeConfig(jni_env);
}


void notifyThreadStart(JNIEnv *jni_env, jthread thread) {
    jvmtiThreadInfo info;
    (*globalJvmtiEnv)->GetThreadInfo(globalJvmtiEnv, thread, &info);
    if(theBridgeClass == NULL){
        return;
    }
    (*jni_env)->CallStaticVoidMethod(jni_env, theBridgeClass, onThreadStart, info.name);
}

