//
// Created by skyinu on 2019/12/4.
//

#include <jni.h>
#include"string.h"
#include "applog/app_log.h"
#include "jvmti-wrapper.h"
#include "malloc.h"

#define LOG_TAG "TAG_WRAPPER"
#define MAX_LOG_LENGTH 500
#define GENERAL_LOG_LENGTH 200


jvmtiEnv *createJvmTiEnv(JavaVM *vm, jint version) {
    jvmtiEnv *jvmti;
    jint result = (*vm)->GetEnv(vm, (void **) &jvmti, version);
    if (result != JNI_OK) {
        loge(LOG_TAG, "occur errors when create jvm ti env");
        return NULL;
    }
    return jvmti;
}

jboolean addAndCheckCapabilities(jvmtiEnv *jvmti) {
    jvmtiError error;
    jvmtiCapabilities capa;
    //consider to set t0 0 and set to 1 case by case
    (void) memset(&capa, 1, sizeof(jvmtiCapabilities));
    error = (*jvmti)->AddCapabilities(jvmti, &capa);
    if (error != NULL) {
        char log[GENERAL_LOG_LENGTH];
        sprintf(log, "something not support here, error code is %d", error);
        loge(LOG_TAG, log);
    }
}

void configEvent(jvmtiEnv *jvmti) {
    jint start = JVMTI_MIN_EVENT_TYPE_VAL;
    jint end = JVMTI_MAX_EVENT_TYPE_VAL;
    for (int index = start + 1; index < end; ++index) {
        jvmtiError error = (*jvmti)->SetEventNotificationMode(jvmti, JVMTI_ENABLE,
                                                              (jvmtiEvent) index, NULL);
        if (error != NULL) {
            char log[GENERAL_LOG_LENGTH];
            sprintf(log, "config event %d error, error code is %d", index, error);
            loge(LOG_TAG, log);
        }
    }
}

void configCallback(jvmtiEnv *jvmti) {
    jvmtiEventCallbacks callbacks;
    (void) memset(&callbacks, 0, sizeof(callbacks));
    callbacks.VMInit = &wrappperVMInit;
    callbacks.VMDeath = &wrapperVMDeath;
    callbacks.ThreadStart = &wrapperThreadStart;
    callbacks.ThreadEnd = &wrapperThreadEnd;
    callbacks.ClassFileLoadHook = &wrapperClassFileLoadHook;
    callbacks.ClassLoad = &wrapperClassLoad;
    callbacks.ClassPrepare = &wrapperClassPrepare;
    callbacks.MethodEntry = &wrapperMethodEntry;
    jvmtiError error = (*jvmti)->SetEventCallbacks(jvmti, &callbacks, (jint) sizeof(callbacks));
    if (error != NULL) {
        char log[GENERAL_LOG_LENGTH];
        sprintf(log, "config callback  error, error code is %d", error);
        loge(LOG_TAG, log);
    }
}

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    logi(LOG_TAG, "on library load");
    return JNI_VERSION_1_6;
}

JNIEXPORT jint JNICALL Agent_OnAttach(JavaVM *vm, char *options, void *reserved) {
    char *log = malloc(sizeof(char) * MAX_LOG_LENGTH);
    stpcpy(log, "agent attach with options = ");
    jint maxAppend = MAX_LOG_LENGTH - strlen(log) - 1;
    logi(LOG_TAG, strncat(log, options, (size_t) maxAppend));
    free(log);
    jvmtiEnv *jvmti = createJvmTiEnv(vm, JVMTI_VERSION_1_2);
    addAndCheckCapabilities(jvmti);
    configEvent(jvmti);
    configCallback(jvmti);
    return JNI_OK;
}

//callback area start
void wrapperMethodEntry(jvmtiEnv *jvmti_env,
                        JNIEnv *jni_env,
                        jthread thread,
                        jmethodID method) {
    logi(LOG_TAG, "wrapperMethodEntry");
}

void wrappperVMInit(jvmtiEnv *jvmti_env,
                    JNIEnv *jni_env,
                    jthread thread) {
    logi(LOG_TAG, "wrappperVMInit");
}

void wrapperVMDeath(jvmtiEnv *jvmti_env,
                    JNIEnv *jni_env) {
    logi(LOG_TAG, "wrapperVMDeath");
}

void wrapperThreadStart(jvmtiEnv *jvmti_env,
                        JNIEnv *jni_env,
                        jthread thread) {
    logi(LOG_TAG, "wrapperThreadStart");
}

void wrapperThreadEnd(jvmtiEnv *jvmti_env,
                      JNIEnv *jni_env,
                      jthread thread) {
    logi(LOG_TAG, "wrapperThreadEnd");
}

void wrapperClassFileLoadHook(jvmtiEnv *jvmti_env,
                              JNIEnv *jni_env,
                              jclass class_being_redefined,
                              jobject loader,
                              const char *name,
                              jobject protection_domain,
                              jint class_data_len,
                              const unsigned char *class_data,
                              jint *new_class_data_len,
                              unsigned char **new_class_data) {
    logi(LOG_TAG, "wrapperClassFileLoadHook");
}

void wrapperClassLoad(jvmtiEnv *jvmti_env,
                      JNIEnv *jni_env,
                      jthread thread,
                      jclass klass) {
    logi(LOG_TAG, "wrapperClassLoad");
}

void wrapperClassPrepare(jvmtiEnv *jvmti_env,
                         JNIEnv *jni_env,
                         jthread thread,
                         jclass klass) {
    logi(LOG_TAG, "wrapperClassPrepare");
}
//callback area end

