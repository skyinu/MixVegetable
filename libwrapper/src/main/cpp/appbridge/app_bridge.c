//
// Created by skyinu on 2019/12/30.
//

#include "app_bridge.h"
#include "../applog/app_log.h"

#define BRIDGE_CLASS "com/skyinu/jvmti/libwrapper/NativeTiBridge"
#define BRIDGE_LOG_TAG "TAG_WRAPPER"
jvmtiEnv *globalJvmtiEnv;
jclass theBridgeClass = NULL;
jmethodID onThreadStart;
jmethodID onThreadEnd;
jmethodID onClassLoad;

void initBridgeConfig(JNIEnv *jni_env) {
    theBridgeClass = (*jni_env)->NewGlobalRef(jni_env,
                                              (*jni_env)->FindClass(jni_env, BRIDGE_CLASS));
    onThreadStart = (*jni_env)->GetStaticMethodID(jni_env,
                                                  theBridgeClass,
                                                  "onThreadStart",
                                                  "(Ljava/lang/String;Z)V");
    onThreadEnd = (*jni_env)->GetStaticMethodID(jni_env,
                                                theBridgeClass,
                                                "onThreadEnd",
                                                "(Ljava/lang/String;Z)V");
    onClassLoad = (*jni_env)->GetStaticMethodID(jni_env,
                                                theBridgeClass,
                                                "onClassLoad",
                                                "(Ljava/lang/String;Ljava/lang/Class;)V");
}

void setUpEnv(jvmtiEnv *jvmti_env, JNIEnv *jni_env) {
    globalJvmtiEnv = jvmti_env;
    initBridgeConfig(jni_env);
}


void notifyThreadStart(JNIEnv *jni_env, jthread thread) {
    jvmtiThreadInfo info;
    (*globalJvmtiEnv)->GetThreadInfo(globalJvmtiEnv, thread, &info);
    jstring threadName = (*jni_env)->NewStringUTF(jni_env, info.name);
    (*jni_env)->CallStaticVoidMethod(jni_env, theBridgeClass, onThreadStart, threadName,
                                     info.is_daemon);
    (*jni_env)->DeleteLocalRef(jni_env, threadName);
}

void notifyThreadEnd(JNIEnv *jni_env, jthread thread) {
    jvmtiThreadInfo info;
    (*globalJvmtiEnv)->GetThreadInfo(globalJvmtiEnv, thread, &info);
    jstring threadName = (*jni_env)->NewStringUTF(jni_env, info.name);
    (*jni_env)->CallStaticVoidMethod(jni_env, theBridgeClass, onThreadEnd, threadName,
                                     info.is_daemon);
    (*jni_env)->DeleteLocalRef(jni_env, threadName);
}

void notifyClassLoad(JNIEnv *jni_env, jthread thread, jclass klass) {
    jvmtiThreadInfo info;
    (*globalJvmtiEnv)->GetThreadInfo(globalJvmtiEnv, thread, &info);
    jstring threadName = (*jni_env)->NewStringUTF(jni_env, info.name);
    (*jni_env)->DeleteLocalRef(jni_env, threadName);
}

