//
// Created by skyinu on 2019/12/30.
//

#include "app_bridge.h"
#include "../applog/app_log.h"
#include "pthread.h"
#include "malloc.h"
#include "unistd.h"

#define BRIDGE_CLASS "com/skyinu/jvmti/libwrapper/NativeTiBridge"
#define BRIDGE_LOG_TAG "TAG_WRAPPER"
#define SLEEP_TIME (500 * 1000)
#define MSG_TYPE_CLASSLODE  0

typedef struct Node {
    short msgType;
    char *msg1;
    char *msg2;
    char *msg3;
    struct Node *prev;
    struct Node *next;
} Node;

JavaVM *globalVm;
jvmtiEnv *globalJvmtiEnv;
pthread_mutex_t nodeMutex;
Node *head, *tail;
jclass theBridgeClass = NULL;
jmethodID onThreadStart;
jmethodID onThreadEnd;
jmethodID onClassLoad;

void addNode(Node *node) {
    pthread_mutex_lock(&nodeMutex);
    Node *preTail = tail->prev;
    preTail->next = node;
    node->prev = preTail;
    node->next = tail;
    tail->prev = node;
    pthread_mutex_unlock(&nodeMutex);
}

Node *getNode() {
    if (head->next == tail) {
        return NULL;
    } else {
        pthread_mutex_lock(&nodeMutex);
        Node *current = head->next;
        Node *next = current->next;
        head->next = next;
        next->prev = head;
        pthread_mutex_unlock(&nodeMutex);
        return current;
    }
}

void freeNode(Node *node) {
    free(node->msg1);
    free(node->msg2);
    free(node->msg3);
    free(node);
}

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
            jstring classGeneric = (*jni_env)->NewStringUTF(jni_env, current->msg3);
            (*jni_env)->CallStaticVoidMethod(jni_env, theBridgeClass, onClassLoad, threadName,
                                             classSign, classGeneric);
            (*jni_env)->DeleteLocalRef(jni_env, threadName);
            (*jni_env)->DeleteLocalRef(jni_env, classSign);
            (*jni_env)->DeleteLocalRef(jni_env, classGeneric);
        }
        freeNode(current);
    }
    (*globalVm)->DetachCurrentThread(globalVm);
    pthread_exit(NULL);
}

void initBridgeConfig(JNIEnv *jni_env) {
    pthread_mutex_init(&nodeMutex, NULL);
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
                                                "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
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
    char *signature = malloc(sizeof(char) * 100);
    char *generic = malloc(sizeof(char) * 150);
    (*globalJvmtiEnv)->GetClassSignature(globalJvmtiEnv, klass, &signature, &generic);
    Node *node = malloc(sizeof(Node));
    node->msg1 = info.name;
    node->msg2 = signature;
    node->msg3 = generic;
    node->msgType = MSG_TYPE_CLASSLODE;
    addNode(node);
}

