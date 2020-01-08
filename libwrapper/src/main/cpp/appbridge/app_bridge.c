//
// Created by skyinu on 2019/12/30.
//

#include <string.h>
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
#define MSG_TYPE_METHOD_ENTRY  2
#define MSG_TYPE_GARBAGE_START  3
#define MSG_TYPE_GARBAGE_FINISH  4
#define MSG_TYPE_OBJECT_FREE  5

JavaVM *globalVm;
jvmtiEnv *globalJvmtiEnv;
jclass theBridgeClass = NULL;
jmethodID onThreadStart;
jmethodID onThreadEnd;
jmethodID onClassLoad = NULL;
jmethodID onClassPrepare = NULL;
jmethodID onMethodEntry = NULL;
jmethodID onGarbageCollectionStart = NULL;
jmethodID onGarbageCollectionFinish = NULL;
jmethodID onObjectFree = NULL;

void *handleMessages(void *args) {
    logi(BRIDGE_LOG_TAG, "work thread start");
    JNIEnv *jni_env;
    (*globalVm)->AttachCurrentThread(globalVm, &jni_env, NULL);
    while (JNI_TRUE) {
        if (onClassLoad == NULL || onClassPrepare == NULL || onMethodEntry == NULL
            || onGarbageCollectionStart == NULL || onGarbageCollectionFinish == NULL
            || onObjectFree == NULL) {
            usleep(SLEEP_TIME);
            continue;
        }
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
        } else if (current->msgType == MSG_TYPE_METHOD_ENTRY) {
            jstring msg = (*jni_env)->NewStringUTF(jni_env, current->msg1);
            (*jni_env)->CallStaticVoidMethod(jni_env, theBridgeClass, onMethodEntry, msg);
            (*jni_env)->DeleteLocalRef(jni_env, msg);
        } else if (current->msgType == MSG_TYPE_GARBAGE_START) {
            (*jni_env)->CallStaticVoidMethod(jni_env, theBridgeClass, onGarbageCollectionStart);
        } else if (current->msgType == MSG_TYPE_GARBAGE_FINISH) {
            (*jni_env)->CallStaticVoidMethod(jni_env, theBridgeClass, onGarbageCollectionFinish);
        } else if (current->msgType == MSG_TYPE_OBJECT_FREE) {
            (*jni_env)->CallStaticVoidMethod(jni_env, theBridgeClass, onObjectFree);
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
    pthread_t workThread;
    pthread_create(&workThread, NULL, &handleMessages, NULL);
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
    onMethodEntry = (*jni_env)->GetStaticMethodID(jni_env,
                                                  theBridgeClass,
                                                  "onMethodEntry",
                                                  "(Ljava/lang/String;)V");
    onGarbageCollectionStart = (*jni_env)->GetStaticMethodID(jni_env,
                                                             theBridgeClass,
                                                             "onGarbageCollectionStart",
                                                             "()V");
    onGarbageCollectionFinish = (*jni_env)->GetStaticMethodID(jni_env,
                                                              theBridgeClass,
                                                              "onGarbageCollectionFinish",
                                                              "()V");
    onObjectFree = (*jni_env)->GetStaticMethodID(jni_env,
                                                 theBridgeClass,
                                                 "onObjectFree",
                                                 "()V");
}

void setUpEnv(JavaVM *vm, jvmtiEnv *jvmti_env, JNIEnv *jni_env) {
    globalVm = vm;
    globalJvmtiEnv = jvmti_env;
    initBridgeConfig(jni_env);
}


void notifyThreadStart(JNIEnv *jni_env, jthread thread) {
    jvmtiThreadInfo info;
    (*globalJvmtiEnv)->GetThreadInfo(globalJvmtiEnv, thread, &info);
    if (onThreadStart == NULL) {
        logi(BRIDGE_LOG_TAG, "thread start callback not ready");
        return;
    }
    jstring threadName = (*jni_env)->NewStringUTF(jni_env, info.name);
    (*jni_env)->CallStaticVoidMethod(jni_env, theBridgeClass, onThreadStart, threadName,
                                     info.is_daemon);
    (*jni_env)->DeleteLocalRef(jni_env, threadName);
}

void notifyThreadEnd(JNIEnv *jni_env, jthread thread) {
    jvmtiThreadInfo info;
    (*globalJvmtiEnv)->GetThreadInfo(globalJvmtiEnv, thread, &info);
    if (onThreadStart == NULL) {
        logi(BRIDGE_LOG_TAG, "thread end callback not ready");
        return;
    }
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

void notifyMethodEntry(jthread thread, jmethodID method) {
    jvmtiThreadInfo info;
    (*globalJvmtiEnv)->GetThreadInfo(globalJvmtiEnv, thread, &info);
    char *classSignature = malloc(sizeof(char) * 150);
    jclass declareClass;
    (*globalJvmtiEnv)->GetMethodDeclaringClass(globalJvmtiEnv, method, &declareClass);
    (*globalJvmtiEnv)->GetClassSignature(globalJvmtiEnv, declareClass, &classSignature, NULL);
    if (strcmp(classSignature, "Lcom/skyinu/jvmti/libwrapper/NativeTiBridge;") == 0
        || strcmp(classSignature, "Landroid/util/Log;") == 0
        || strncmp(classSignature, "Ljava/lang/", strlen("Ljava/lang/")) == 0) {
        free(classSignature);
        return;
    }
    char *methodMsg = malloc(sizeof(char) * 300);
    char *methodName = malloc(sizeof(char) * 100);
    char *methodSignature = malloc(sizeof(char) * 50);
    methodMsg[0] = '\0';
    (*globalJvmtiEnv)->GetMethodName(globalJvmtiEnv, method, &methodName, &methodSignature, NULL);
    strcat(methodMsg, "onMethodEntry ");
    strcat(methodMsg, info.name);
    strcat(methodMsg, " ");
    strcat(methodMsg, classSignature);
    strcat(methodMsg, " ");
    strcat(methodMsg, methodName);
    strcat(methodMsg, " ");
    strcat(methodMsg, methodSignature);
    Node *node = newNode();
    node->msg1 = methodMsg;
    node->msgType = MSG_TYPE_METHOD_ENTRY;
    addNode(node);
    free(methodName);
    free(classSignature);
    free(methodSignature);
}

void notifyGarbageCollectionStart() {
    Node *node = newNode();
    node->msgType = MSG_TYPE_GARBAGE_START;
    addNode(node);
}

void notifyGarbageCollectionFinish() {
    Node *node = newNode();
    node->msgType = MSG_TYPE_GARBAGE_FINISH;
    addNode(node);
}

void notifyObjectFree() {
    Node *node = newNode();
    node->msgType = MSG_TYPE_OBJECT_FREE;
    addNode(node);
}
