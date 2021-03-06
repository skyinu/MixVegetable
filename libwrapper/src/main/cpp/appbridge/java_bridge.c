//
// Created by skyinu on 2020/1/11.
//

#include <malloc.h>
#include "java_bridge.h"
#include "string.h"

#define FILTER_BRIDGE_PACKAGE  "Lcom/skyinu/jvmti/"
#define FILTER_KT_JVM_INTERNAL "Lkotlin/jvm/internal/"
#define FILTER_ANDROID_UTIL "Landroid/util/"
#define FILTER_JAVA_LANG "Ljava/lang/"

jclass theBridgeClass = NULL;
jmethodID *bridgeMethods = NULL;

//support set package filter at NativeTiBridge in the future
int shouldFilterMethodCall(char *classSign) {
    const size_t bridgeClassLength = strlen(FILTER_BRIDGE_PACKAGE);
    const size_t ktJvmInternalLength = strlen(FILTER_KT_JVM_INTERNAL);
    const size_t androidUtilLength = strlen(FILTER_ANDROID_UTIL);
    const size_t javaLangLength = strlen(FILTER_JAVA_LANG);
    if (strncmp(classSign, FILTER_BRIDGE_PACKAGE, bridgeClassLength) == 0
        || strncmp(classSign, FILTER_KT_JVM_INTERNAL, ktJvmInternalLength) == 0
        || strncmp(classSign, FILTER_ANDROID_UTIL, androidUtilLength) == 0
        || strncmp(classSign, FILTER_JAVA_LANG, javaLangLength) == 0) {
        return 1;
    }
    return 0;
}

int isBizClass(char *classSign) {
    return strcmp(THE_BIZ_CLASS_LOADER_CLASS, classSign) == 0 ||
           strcmp(THE_BIZ_CLASS_LOADER_CLASS_TYPE, classSign) == 0;
}

void initBridgeClass(JNIEnv *jni_env, jobject theBizClassLoader) {
    auto classLoaderClass = (*jni_env)->FindClass(jni_env, "java/lang/ClassLoader");
    auto loadClassMethod = (*jni_env)->GetMethodID(jni_env, classLoaderClass, "loadClass",
                                                   "(Ljava/lang/String;)Ljava/lang/Class;");
    auto bridgeClass = (*jni_env)->CallObjectMethod(jni_env, theBizClassLoader,
                                                    loadClassMethod,
                                                    (*jni_env)->NewStringUTF(jni_env,
                                                                             BRIDGE_CLASS));
    theBridgeClass = (*jni_env)->NewGlobalRef(jni_env, bridgeClass);
}

void initBridgeMethod(JNIEnv *jni_env) {
    bridgeMethods = malloc(sizeof(jmethodID) * BRIDGE_METHOD_COUNT);
    memset(bridgeMethods, 0, BRIDGE_METHOD_COUNT);
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
    bridgeMethods[MSG_TYPE_DYNAMIC_CODE_GENERATED] = (*jni_env)->GetStaticMethodID(
            jni_env,
            theBridgeClass,
            "onDynamicCodeGenerated",
            "(Ljava/lang/String;)V");
    bridgeMethods[MSG_TYPE_VMOBJECT_ALLOC] = (*jni_env)->GetStaticMethodID(
            jni_env,
            theBridgeClass,
            "onVMObjectAlloc",
            "(Ljava/lang/String;Ljava/lang/String;)V");
}