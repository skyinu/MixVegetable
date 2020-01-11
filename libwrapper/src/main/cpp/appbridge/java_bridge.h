//
// Created by skyinu on 2020/1/11.
//

#ifndef JVMTIWRAPPER_ANDROID_JAVA_BRIDGE_H
#define JVMTIWRAPPER_ANDROID_JAVA_BRIDGE_H

#endif //JVMTIWRAPPER_ANDROID_JAVA_BRIDGE_H

#include "jni.h"

#define BRIDGE_CLASS "com/skyinu/jvmti/libwrapper/NativeTiBridge"
#define THE_BIZ_CLASS_LOADER_CLASS_TYPE  "[Landroid/view/View;"
#define THE_BIZ_CLASS_LOADER_CLASS  "Landroid/view/View;"
#define BRIDGE_METHOD_COUNT 19
#define MSG_TYPE_CLASSLODE  0
#define MSG_TYPE_CLASSPREPARE  1
#define MSG_TYPE_METHOD_ENTRY  2
#define MSG_TYPE_GARBAGE_START  3
#define MSG_TYPE_GARBAGE_FINISH  4
#define MSG_TYPE_OBJECT_FREE  5
#define MSG_TYPE_THREAD_START  6
#define MSG_TYPE_THREAD_END  7
#define MSG_TYPE_METHOD_EXIT  8
#define MSG_TYPE_COMPILED_METHOD_LOAD 9
#define MSG_TYPE_COMPILED_METHOD_UNLOAD 10
#define MSG_TYPE_MONITOR_WAIT  11
#define MSG_TYPE_MONITOR_WAITED  12
#define MSG_TYPE_MONITOR_CONTENDED_ENTER 13
#define MSG_TYPE_MONITOR_CONTENDED_ENTERED 14
#define MSG_TYPE_DATA_DUMP_REQUEST 15
#define MSG_TYPE_RESOURCE_EXHAUSTED 16
#define MSG_TYPE_DYNAMIC_CODE_GENERATED 17
#define MSG_TYPE_VMOBJECT_ALLOC 18

int shouldFilterMethodCall(char *classSign);

int isBizClass(char *classSign);

void initBridgeClass(JNIEnv *jni_env, jobject theBizClassLoader);

void initBridgeMethod(JNIEnv *jni_env);