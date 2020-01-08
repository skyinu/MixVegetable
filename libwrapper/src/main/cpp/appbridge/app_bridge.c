//
// Created by skyinu on 2019/12/30.
//

#include "app_bridge.h"
#include "../applog/app_log.h"
#include "pthread.h"
#include "malloc.h"
#include "unistd.h"
#include "node.h"

#define BRIDGE_CLASS "com/skyinu/jvmti/libwrapper/NativeTiBridge"
#define BRIDGE_LOG_TAG "TAG_WRAPPER"
#define SLEEP_TIME (500 * 1000)
#define MSG_TYPE_CLASSLODE  0
#define MSG_TYPE_CLASSPREPARE  1

JavaVM *globalVm;
jvmtiEnv *globalJvmtiEnv;
jclass theBridgeClass = NULL;
jmethodID onThreadStart;
jmethodID onThreadEnd;
jmethodID onClassLoad;
jmethodID onClassPrepare;

void *handleMessages(void *args) {
    logi(BRIDGE_LOG_TAG, "work thread start");
    JNIEnv *jni_env;
    (*globalVm)->AttachCurrentThread(globalVm, &jni_env, NULL);
    while (JNI_TRUE) {
        Node *current = getNode();
        if (current == NULL) {
            usleep(SLEEP_TIME);
            continue;
        }
        if (current->msgType == MSG_TYPE_CLASSLODE) {
            jstring threadName = (*jni_env)->NewStringUTF(jni_env, current->msg1);
            jstring classSign = (*jni_env)->NewStringUTF(jni_env, current->msg2);
            (*jni_env)->CallStaticVoidMethod(jni_env, theBridgeClass, onClassLoad, threadName,
                                             classSign);
            (*jni_env)->DeleteLocalRef(jni_env, threadName);
            (*jni_env)->DeleteLocalRef(jni_env, classSign);
        } else if (current->msgType == MSG_TYPE_CLASSPREPARE) {
            jstring threadName = (*jni_env)->NewStringUTF(jni_env, current->msg1);
            jstring classSign = (*jni_env)->NewStringUTF(jni_env, current->msg2);
            (*jni_env)->CallStaticVoidMethod(jni_env, theBridgeClass, onClassPrepare, threadName,
                                             classSign);
            (*jni_env)->DeleteLocalRef(jni_env, threadName);
            (*jni_env)->DeleteLocalRef(jni_env, classSign);
        } else {
            //this is just to suppress lint, should be never run here
            break;
        }
        freeNode(current);
    }
    (*globalVm)->DetachCurrentThread(globalVm);
    pthread_exit(NULL);
}

void initBridgeConfig(JNIEnv *jni_env) {
    init();
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
                                                "(Ljava/lang/String;Ljava/lang/String;)V");
    onClassPrepare = (*jni_env)->GetStaticMethodID(jni_env,
                                                   theBridgeClass,
                                                   "onClassPrepare",
                                                   "(Ljava/lang/String;Ljava/lang/String;)V");
    pthread_t workThread;
    pthread_create(&workThread, NULL, &handleMessages, NULL);
}

void setUpEnv(JavaVM *vm, jvmtiEnv *jvmti_env, JNIEnv *jni_env) {
    globalVm = vm;
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
    char *signature = malloc(sizeof(char) * 100);
    (*globalJvmtiEnv)->GetClassSignature(globalJvmtiEnv, klass, &signature, NULL);
    Node *node = newNode();
    node->msg1 = info.name;
    node->msg2 = signature;
    node->msgType = MSG_TYPE_CLASSLODE;
    addNode(node);
}

void notifyClassPrepare(JNIEnv *jni_env, jthread thread, jclass klass) {
    jvmtiThreadInfo info;
    (*globalJvmtiEnv)->GetThreadInfo(globalJvmtiEnv, thread, &info);
    char *signature = malloc(sizeof(char) * 100);
    (*globalJvmtiEnv)->GetClassSignature(globalJvmtiEnv, klass, &signature, NULL);
    Node *node = newNode();
    node->msg1 = info.name;
    node->msg2 = signature;
    node->msgType = MSG_TYPE_CLASSPREPARE;
    addNode(node);
}

