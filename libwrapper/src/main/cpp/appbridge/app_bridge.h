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

void notifyMethodEntry(jthread thread, jmethodID method);

void notifyMethodExit(jthread thread, jmethodID method, jboolean was_popped_by_exception,
                      jvalue return_value);

void notifyGarbageCollectionStart();

void notifyGarbageCollectionFinish();

void notifyObjectFree();

void notifyCompiledMethodLoad(jmethodID method, jint code_size);

void notifyCompiledMethodUnload(jmethodID method);

void notifyMonitorWait(JNIEnv *jni_env, jthread thread, jobject object, jlong timeout);

void notifyMonitorWaited(JNIEnv *jni_env, jthread thread, jobject object, jlong timeout);

void notifyMonitorContendedEnter(JNIEnv *jni_env, jthread thread, jobject object);

void notifyMonitorContendedEntered(JNIEnv *jni_env, jthread thread, jobject object);

void notifyDataDumpRequest();

void notifyResourceExhausted();

void notifyDynamicCodeGenerated(const char *name);

void notifyVMObjectAlloc(JNIEnv *jni_env,
                         jthread thread,
                         jclass object_klass);