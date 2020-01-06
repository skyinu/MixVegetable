//
// Created by skyinu on 2019/12/30.
//

#include "app_bridge.h"
#include "../applog/app_log.h"
#include "pthread.h"
#include "malloc.h"

#define BRIDGE_CLASS "com/skyinu/jvmti/libwrapper/NativeTiBridge"
#define BRIDGE_LOG_TAG "TAG_WRAPPER"

typedef struct Node {
    short msgType;
    void *msg;
    struct Node *prev;
    struct Node *next;
} Node;

JavaVM *globalVm;
jvmtiEnv *globalJvmtiEnv;
Node *head, *tail;
jclass theBridgeClass = NULL;
jmethodID onThreadStart;
jmethodID onThreadEnd;
jmethodID onClassLoad;

void addNode(Node node) {
    Node *preTail = tail->prev;
    preTail->next = &node;
    node.prev = preTail;
    node.next = tail;
}

Node getNode() {
    if (head->next == tail) {
        return NULL;
    }
    Node *next = head->next;
    head->next = next->next;
    next->next->prev = head;
    return *next;
}

void *handleMessages(void *args) {
    logi(BRIDGE_LOG_TAG, "work thread start");
    JNIEnv *jni_env;
    (*globalVm)->AttachCurrentThread(globalVm, &jni_env, NULL);
    Node current = getNode();
    (*globalVm)->DetachCurrentThread(globalVm);
    pthread_exit(NULL);
}

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
    pthread_t workThread;
    pthread_create(&workThread, NULL, &handleMessages, NULL);
}

void setUpEnv(JavaVM *vm, jvmtiEnv *jvmti_env, JNIEnv *jni_env) {
    globalVm = vm;
    globalJvmtiEnv = jvmti_env;
    head = malloc(sizeof(Node));
    tail = malloc(sizeof(Node));
    head->prev = NULL;
    head->next = tail;
    tail->prev = head;
    tail->next = NULL;
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

