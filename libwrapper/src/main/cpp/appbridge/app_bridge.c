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
#define BRIDGE_METHOD_COUNT 9
#define BRIDGE_LOG_TAG "TAG_WRAPPER"
#define SLEEP_TIME (500 * 1000)
#define MSG_TYPE_CLASSLODE  0
#define MSG_TYPE_CLASSPREPARE  1
#define MSG_TYPE_METHOD_ENTRY  2
#define MSG_TYPE_GARBAGE_START  3
#define MSG_TYPE_GARBAGE_FINISH  4
#define MSG_TYPE_OBJECT_FREE  5
#define MSG_TYPE_THREAD_START  6
#define MSG_TYPE_THREAD_END  7
#define MSG_TYPE_METHOD_EXIT  8

JavaVM *globalVm;
jvmtiEnv *globalJvmtiEnv;
jclass theBridgeClass = NULL;
jmethodID *bridgeMethods = NULL;

void *handleMessages(void *args) {
    logi(BRIDGE_LOG_TAG, "work thread start");
    JNIEnv *jni_env;
    (*globalVm)->AttachCurrentThread(globalVm, &jni_env, NULL);
    int loop = 1;
    while (loop) {
        if (bridgeMethods == NULL) {
            usleep(SLEEP_TIME);
            continue;
        }
        for (int index = 0; index < BRIDGE_METHOD_COUNT; ++index) {
            if (bridgeMethods[index] == NULL) {
                usleep(SLEEP_TIME);
                break;
            }
        }
        Node *current = getNode();
        if (current == NULL) {
            usleep(SLEEP_TIME);
            continue;
        }
        int msgCount = 0;
        if (current->msg3 != NULL) {
            msgCount++;
        }
        if (current->msg2 != NULL) {
            msgCount++;
        }
        if (current->msg1 != NULL) {
            msgCount++;
        }
        jstring msg1;
        jstring msg2;
        jstring msg3;
        switch (msgCount) {
            case 0:
                (*jni_env)->CallStaticVoidMethod(jni_env,
                                                 theBridgeClass,
                                                 bridgeMethods[current->msgType]);
                break;
            case 1:
                msg1 = (*jni_env)->NewStringUTF(jni_env, current->msg1);
                (*jni_env)->CallStaticVoidMethod(jni_env,
                                                 theBridgeClass,
                                                 bridgeMethods[current->msgType],
                                                 msg1);
                (*jni_env)->DeleteLocalRef(jni_env, msg1);
                break;
            case 2:
                msg1 = (*jni_env)->NewStringUTF(jni_env, current->msg1);
                msg2 = (*jni_env)->NewStringUTF(jni_env, current->msg2);
                (*jni_env)->CallStaticVoidMethod(jni_env,
                                                 theBridgeClass,
                                                 bridgeMethods[current->msgType],
                                                 msg1,
                                                 msg2);
                (*jni_env)->DeleteLocalRef(jni_env, msg1);
                (*jni_env)->DeleteLocalRef(jni_env, msg2);
                break;
            case 3:
                msg1 = (*jni_env)->NewStringUTF(jni_env, current->msg1);
                msg2 = (*jni_env)->NewStringUTF(jni_env, current->msg2);
                msg3 = (*jni_env)->NewStringUTF(jni_env, current->msg3);
                (*jni_env)->CallStaticVoidMethod(jni_env,
                                                 theBridgeClass,
                                                 bridgeMethods[current->msgType],
                                                 msg1,
                                                 msg2,
                                                 msg3);
                (*jni_env)->DeleteLocalRef(jni_env, msg1);
                (*jni_env)->DeleteLocalRef(jni_env, msg2);
                (*jni_env)->DeleteLocalRef(jni_env, msg3);
                break;
            default:
                //this is just to suppress lint, should be never run here
                loop = 0;
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
    bridgeMethods = malloc(sizeof(jmethodID) * BRIDGE_METHOD_COUNT);
    memset(bridgeMethods, NULL, BRIDGE_METHOD_COUNT);
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

}

void setUpEnv(JavaVM *vm, jvmtiEnv *jvmti_env, JNIEnv *jni_env) {
    globalVm = vm;
    globalJvmtiEnv = jvmti_env;
    initBridgeConfig(jni_env);
}


void notifyThreadStart(JNIEnv *jni_env, jthread thread) {
    jvmtiThreadInfo info;
    (*globalJvmtiEnv)->GetThreadInfo(globalJvmtiEnv, thread, &info);
    Node *node = newNode(MSG_TYPE_THREAD_START);
    node->msg1 = info.name;
    node->msg2 = malloc(sizeof(char) * 10);
    if (info.is_daemon) {
        strcpy(node->msg2, "true");
    } else {
        strcpy(node->msg2, "false");
    }
    addNode(node);
}

void notifyThreadEnd(JNIEnv *jni_env, jthread thread) {
    jvmtiThreadInfo info;
    (*globalJvmtiEnv)->GetThreadInfo(globalJvmtiEnv, thread, &info);
    Node *node = newNode(MSG_TYPE_THREAD_END);
    node->msg1 = info.name;
    node->msg2 = malloc(sizeof(char) * 10);
    if (info.is_daemon) {
        strcpy(node->msg2, "true");
    } else {
        strcpy(node->msg2, "false");
    }
    addNode(node);
}

void notifyClassLoad(JNIEnv *jni_env, jthread thread, jclass klass) {
    jvmtiThreadInfo info;
    (*globalJvmtiEnv)->GetThreadInfo(globalJvmtiEnv, thread, &info);
    char *signature = malloc(sizeof(char) * 100);
    (*globalJvmtiEnv)->GetClassSignature(globalJvmtiEnv, klass, &signature, NULL);
    Node *node = newNode(MSG_TYPE_CLASSLODE);
    node->msg1 = info.name;
    node->msg2 = signature;
    addNode(node);
}

void notifyClassPrepare(JNIEnv *jni_env, jthread thread, jclass klass) {
    jvmtiThreadInfo info;
    (*globalJvmtiEnv)->GetThreadInfo(globalJvmtiEnv, thread, &info);
    char *signature = malloc(sizeof(char) * 100);
    (*globalJvmtiEnv)->GetClassSignature(globalJvmtiEnv, klass, &signature, NULL);
    Node *node = newNode(MSG_TYPE_CLASSPREPARE);
    node->msg1 = info.name;
    node->msg2 = signature;
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
        || strncmp(classSignature, "Lkotlin/jvm/internal/", strlen("Lkotlin/jvm/internal/")) == 0
        || strncmp(classSignature, "Landroid/util/", strlen("Landroid/util/")) == 0
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
    Node *node = newNode(MSG_TYPE_METHOD_ENTRY);
    node->msg1 = methodMsg;
    addNode(node);
    free(methodName);
    free(classSignature);
    free(methodSignature);
}

void notifyMethodExit(jthread thread, jmethodID method, jboolean was_popped_by_exception,
                      jvalue return_value) {
    jvmtiThreadInfo info;
    (*globalJvmtiEnv)->GetThreadInfo(globalJvmtiEnv, thread, &info);
    char *classSignature = malloc(sizeof(char) * 150);
    jclass declareClass;
    (*globalJvmtiEnv)->GetMethodDeclaringClass(globalJvmtiEnv, method, &declareClass);
    (*globalJvmtiEnv)->GetClassSignature(globalJvmtiEnv, declareClass, &classSignature, NULL);
    if (strcmp(classSignature, "Lcom/skyinu/jvmti/libwrapper/NativeTiBridge;") == 0
        || strncmp(classSignature, "Lkotlin/jvm/internal/", strlen("Lkotlin/jvm/internal/")) == 0
        || strncmp(classSignature, "Landroid/util/", strlen("Landroid/util/")) == 0
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
    Node *node = newNode(MSG_TYPE_METHOD_EXIT);
    node->msg2 = malloc(sizeof(char) * 10);
    if (was_popped_by_exception) {
        strcpy(node->msg2, "true");
    } else {
        strcpy(node->msg2, "false");
    }
    node->msg1 = methodMsg;
    addNode(node);
    free(methodName);
    free(classSignature);
    free(methodSignature);
}

void notifyGarbageCollectionStart() {
    Node *node = newNode(MSG_TYPE_GARBAGE_START);
    addNode(node);
}

void notifyGarbageCollectionFinish() {
    Node *node = newNode(MSG_TYPE_GARBAGE_FINISH);
    addNode(node);
}

void notifyObjectFree() {
    Node *node = newNode(MSG_TYPE_OBJECT_FREE);
    addNode(node);
}
