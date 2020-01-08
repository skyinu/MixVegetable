//
// Created by skyinu on 2019/12/30.
//

#ifndef JVMTIWRAPPER_ANDROID_APP_BRIDGE_H
#define JVMTIWRAPPER_ANDROID_APP_BRIDGE_H

#endif //JVMTIWRAPPER_ANDROID_APP_BRIDGE_H

#include "../jvmti.h"

void setUpEnv(JavaVM *vm, jvmtiEnv *jvmti_env, JNIEnv *jni_env);

void notifyThreadStart(JNIEnv *jni_env, jthread thread);

void notifyThreadEnd(JNIEnv *jni_env, jthread thread);

void notifyClassLoad(JNIEnv *jni_env, jthread thread, jclass klass);

void notifyClassPrepare(JNIEnv *jni_env, jthread thread, jclass klass);