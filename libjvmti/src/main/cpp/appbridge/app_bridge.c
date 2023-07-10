//
// Created by skyinu on 2019/12/30.
//

#include <string.h>
#include "stdlib.h"
#include "app_bridge.h"
#include "../applog/app_log.h"
#include "pthread.h"
#include "malloc.h"
#include "unistd.h"
#include "node.h"
#include "java_bridge.h"
#include "stdlib.h"

#define BRIDGE_LOG_TAG "TAG_WRAPPER"
#define SLEEP_TIME (500 * 1000)

JavaVM *globalVm;
jvmtiEnv *globalJvmtiEnv;
extern jclass theBridgeClass;
extern jmethodID *bridgeMethods;
jobject theBizClassLoader = NULL;

void *handleMessages(void *args) {
    logi(BRIDGE_LOG_TAG, "work thread start");
    JNIEnv *jni_env;
    (*globalVm)->AttachCurrentThread(globalVm, &jni_env, NULL);
    int loop = 1;
    while (JNI_TRUE) {
        if (theBizClassLoader == NULL) {
            usleep(SLEEP_TIME);
            continue;
        }
        break;
    }
    initBridgeClass(jni_env, theBizClassLoader);
    (*jni_env)->DeleteGlobalRef(jni_env, theBizClassLoader);
    theBizClassLoader = NULL;
    initBridgeMethod(jni_env);
    while (loop) {
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
                if (current->obj == NULL) {
                    (*jni_env)->CallStaticVoidMethod(jni_env,
                                                     theBridgeClass,
                                                     bridgeMethods[current->msgType],
                                                     msg1);
                } else {
                    (*jni_env)->CallStaticVoidMethod(jni_env,
                                                     theBridgeClass,
                                                     bridgeMethods[current->msgType],
                                                     msg1,
                                                     current->obj);
                }
                (*jni_env)->DeleteLocalRef(jni_env, msg1);
                break;
            case 2:
                msg1 = (*jni_env)->NewStringUTF(jni_env, current->msg1);
                msg2 = (*jni_env)->NewStringUTF(jni_env, current->msg2);
                if (current->obj == NULL) {
                    (*jni_env)->CallStaticVoidMethod(jni_env,
                                                     theBridgeClass,
                                                     bridgeMethods[current->msgType],
                                                     msg1,
                                                     msg2);
                } else {
                    (*jni_env)->CallStaticVoidMethod(jni_env,
                                                     theBridgeClass,
                                                     bridgeMethods[current->msgType],
                                                     msg1,
                                                     msg2,
                                                     current->obj);
                }
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
        freeNode(jni_env, current);
    }
    (*globalVm)->DetachCurrentThread(globalVm);
    pthread_exit(NULL);
}

void initBridgeConfig(JNIEnv *jni_env) {
    init();
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
    if (isBizClass(signature) || theBridgeClass == NULL) {
        jobject classloader;
        (*globalJvmtiEnv)->GetClassLoader(globalJvmtiEnv, klass, &classloader);
        theBizClassLoader = (*jni_env)->NewGlobalRef(jni_env, classloader);;
    }
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
    if (shouldFilterMethodCall(classSignature)) {
        free(classSignature);
        return;
    }
    char *methodMsg = malloc(sizeof(char) * 500);
    char *methodName = malloc(sizeof(char) * 200);
    char *methodSignature = malloc(sizeof(char) * 100);
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
    char *classSignature = malloc(sizeof(char) * 200);
    jclass declareClass;
    (*globalJvmtiEnv)->GetMethodDeclaringClass(globalJvmtiEnv, method, &declareClass);
    (*globalJvmtiEnv)->GetClassSignature(globalJvmtiEnv, declareClass, &classSignature, NULL);
    if (shouldFilterMethodCall(classSignature)) {
        free(classSignature);
        return;
    }
    char *methodMsg = malloc(sizeof(char) * 500);
    char *methodName = malloc(sizeof(char) * 200);
    char *methodSignature = malloc(sizeof(char) * 100);
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

void notifyCompiledMethodLoad(jmethodID method, jint code_size) {
    char *classSignature = malloc(sizeof(char) * 150);
    jclass declareClass;
    (*globalJvmtiEnv)->GetMethodDeclaringClass(globalJvmtiEnv, method, &declareClass);
    (*globalJvmtiEnv)->GetClassSignature(globalJvmtiEnv, declareClass, &classSignature, NULL);
    char *methodMsg = malloc(sizeof(char) * 300);
    char *methodName = malloc(sizeof(char) * 100);
    char *methodSignature = malloc(sizeof(char) * 50);
    methodMsg[0] = '\0';
    (*globalJvmtiEnv)->GetMethodName(globalJvmtiEnv, method, &methodName, &methodSignature, NULL);
    strcat(methodMsg, "onMethodEntry ");
    strcat(methodMsg, classSignature);
    strcat(methodMsg, " ");
    strcat(methodMsg, methodName);
    strcat(methodMsg, " ");
    strcat(methodMsg, methodSignature);
    Node *node = newNode(MSG_TYPE_COMPILED_METHOD_LOAD);
    node->msg2 = malloc(sizeof(char) * 10);
    node->msg1 = methodMsg;
    sprintf(node->msg2, "%d\0", code_size);
    addNode(node);
    free(methodName);
    free(classSignature);
    free(methodSignature);
}

void notifyCompiledMethodUnload(jmethodID method) {
    char *classSignature = malloc(sizeof(char) * 150);
    jclass declareClass;
    (*globalJvmtiEnv)->GetMethodDeclaringClass(globalJvmtiEnv, method, &declareClass);
    (*globalJvmtiEnv)->GetClassSignature(globalJvmtiEnv, declareClass, &classSignature, NULL);
    char *methodMsg = malloc(sizeof(char) * 300);
    char *methodName = malloc(sizeof(char) * 100);
    char *methodSignature = malloc(sizeof(char) * 50);
    methodMsg[0] = '\0';
    (*globalJvmtiEnv)->GetMethodName(globalJvmtiEnv, method, &methodName, &methodSignature, NULL);
    strcat(methodMsg, "onMethodEntry ");
    strcat(methodMsg, classSignature);
    strcat(methodMsg, " ");
    strcat(methodMsg, methodName);
    strcat(methodMsg, " ");
    strcat(methodMsg, methodSignature);
    Node *node = newNode(MSG_TYPE_COMPILED_METHOD_UNLOAD);
    node->msg1 = methodMsg;
    addNode(node);
    free(methodName);
    free(classSignature);
    free(methodSignature);
}

void notifyMonitorWait(JNIEnv *jni_env, jthread thread, jobject object, jlong timeout) {
    jvmtiThreadInfo info;
    (*globalJvmtiEnv)->GetThreadInfo(globalJvmtiEnv, thread, &info);
    Node *node = newNode(MSG_TYPE_MONITOR_WAIT);
    node->msg1 = info.name;
    node->msg2 = malloc(sizeof(char) * 30);
    if (timeout != NULL) {
        sprintf(node->msg2, "%ld", timeout);
    } else {
        strcpy(node->msg2, "");
    }
    node->obj = (*jni_env)->NewGlobalRef(jni_env, object);
    addNode(node);
}

void notifyMonitorWaited(JNIEnv *jni_env, jthread thread, jobject object, jlong timeout) {
    jvmtiThreadInfo info;
    (*globalJvmtiEnv)->GetThreadInfo(globalJvmtiEnv, thread, &info);
    Node *node = newNode(MSG_TYPE_MONITOR_WAITED);
    node->msg1 = info.name;
    node->msg2 = malloc(sizeof(char) * 30);
    if (timeout != NULL) {
        sprintf(node->msg2, "%ld", timeout);
    } else {
        strcpy(node->msg2, "");
    }
    node->obj = (*jni_env)->NewGlobalRef(jni_env, object);
    addNode(node);
}

void notifyMonitorContendedEnter(JNIEnv *jni_env, jthread thread, jobject object) {
    jvmtiThreadInfo info;
    (*globalJvmtiEnv)->GetThreadInfo(globalJvmtiEnv, thread, &info);
    Node *node = newNode(MSG_TYPE_MONITOR_CONTENDED_ENTER);
    node->msg1 = info.name;
    node->obj = (*jni_env)->NewGlobalRef(jni_env, object);
    addNode(node);
}

void notifyMonitorContendedEntered(JNIEnv *jni_env, jthread thread, jobject object) {
    jvmtiThreadInfo info;
    (*globalJvmtiEnv)->GetThreadInfo(globalJvmtiEnv, thread, &info);
    Node *node = newNode(MSG_TYPE_MONITOR_CONTENDED_ENTERED);
    node->msg1 = info.name;
    node->obj = (*jni_env)->NewGlobalRef(jni_env, object);
    addNode(node);
}

void notifyDataDumpRequest() {
    Node *node = newNode(MSG_TYPE_DATA_DUMP_REQUEST);
    addNode(node);
}

void notifyResourceExhausted() {
    Node *node = newNode(MSG_TYPE_RESOURCE_EXHAUSTED);
    addNode(node);
}

void notifyDynamicCodeGenerated(const char *name) {
    Node *node = newNode(MSG_TYPE_DYNAMIC_CODE_GENERATED);
    node->msg1 = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(node->msg1, name);
    addNode(node);
}

void notifyVMObjectAlloc(JNIEnv *jni_env,
                         jthread thread,
                         jclass object_klass) {
    jvmtiThreadInfo info;
    (*globalJvmtiEnv)->GetThreadInfo(globalJvmtiEnv, thread, &info);
    Node *node = newNode(MSG_TYPE_VMOBJECT_ALLOC);
    node->msg1 = info.name;
    char *classSignature = malloc(sizeof(char) * 150);
    (*globalJvmtiEnv)->GetClassSignature(globalJvmtiEnv, object_klass, &classSignature, NULL);
    node->msg2 = classSignature;
    addNode(node);
}
