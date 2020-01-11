//
// Created by skyinu on 2019/12/4.
//

#include <jni.h>
#include"string.h"
#include "applog/app_log.h"
#include "jvmti-wrapper.h"
#include "malloc.h"
#include "appbridge/app_bridge.h"
#include "jvmti.h"

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
    capa.can_generate_method_entry_events = 1;
    capa.can_generate_method_exit_events = 1;
    capa.can_generate_compiled_method_load_events = 1;
    capa.can_generate_monitor_events = 1;
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
    callbacks.MethodExit = &wrapperMethodExit;
    callbacks.NativeMethodBind = &wrapperNativeMethodBind;
    callbacks.CompiledMethodLoad = &wrapperCompiledMethodLoad;
    callbacks.CompiledMethodUnload = &wrapperCompiledMethodUnload;
    callbacks.DynamicCodeGenerated = &wrapperDynamicCodeGenerated;
    callbacks.DataDumpRequest = &wrapperDataDumpRequest;
    callbacks.MonitorWait = &wrapperMonitorWait;
    callbacks.MonitorWaited = &wrapperMonitorWaited;
    callbacks.MonitorContendedEnter = &wrapperMonitorContendedEnter;
    callbacks.MonitorContendedEntered = &wrapperMonitorContendedEntered;
    callbacks.ResourceExhausted = &wrapperResourceExhausted;
    callbacks.GarbageCollectionStart = &wrapperGarbageCollectionStart;
    callbacks.GarbageCollectionFinish = &wrapperGarbageCollectionFinish;
    callbacks.ObjectFree = &wrapperObjectFree;
    callbacks.VMObjectAlloc = &wrapperVMObjectAlloc;
    jvmtiError error = (*jvmti)->SetEventCallbacks(jvmti, &callbacks, (jint) sizeof(callbacks));
    if (error != NULL) {
        char log[GENERAL_LOG_LENGTH];
        sprintf(log, "config callback  error, error code is %d", error);
        loge(LOG_TAG, log);
    }
}

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    logi(LOG_TAG, "on library load");
    JNIEnv *env;
    if ((*vm)->GetEnv(vm, (void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
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
    JNIEnv *env;
    if ((*vm)->GetEnv(vm, (void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    (*jvmti)->AddToBootstrapClassLoaderSearch(jvmti, options);
    setUpEnv(vm, jvmti, env);
    return JNI_OK;
}

//callback area start

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
    notifyThreadStart(jni_env, thread);
}

void wrapperThreadEnd(jvmtiEnv *jvmti_env,
                      JNIEnv *jni_env,
                      jthread thread) {
    notifyThreadEnd(jni_env, thread);
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
//    logi(LOG_TAG, "wrapperClassFileLoadHook");
}

void wrapperClassLoad(jvmtiEnv *jvmti_env,
                      JNIEnv *jni_env,
                      jthread thread,
                      jclass klass) {
    notifyClassLoad(jni_env, thread, klass);
}

void wrapperClassPrepare(jvmtiEnv *jvmti_env,
                         JNIEnv *jni_env,
                         jthread thread,
                         jclass klass) {
    notifyClassPrepare(jni_env, thread, klass);
}

void wrapperMethodEntry(jvmtiEnv *jvmti_env,
                        JNIEnv *jni_env,
                        jthread thread,
                        jmethodID method) {
    notifyMethodEntry(thread, method);
}

void wrapperMethodExit(jvmtiEnv *jvmti_env,
                       JNIEnv *jni_env,
                       jthread thread,
                       jmethodID method,
                       jboolean was_popped_by_exception,
                       jvalue return_value) {
    notifyMethodExit(thread, method, was_popped_by_exception, return_value);
}

void wrapperNativeMethodBind(jvmtiEnv *jvmti_env,
                             JNIEnv *jni_env,
                             jthread thread,
                             jmethodID method,
                             void *address,
                             void **new_address_ptr) {
    logi(LOG_TAG, "wrapperNativeMethodBind");
}

void wrapperCompiledMethodLoad(jvmtiEnv *jvmti_env,
                               jmethodID method,
                               jint code_size,
                               const void *code_addr,
                               jint map_length,
                               const jvmtiAddrLocationMap *map,
                               const void *compile_info) {
    notifyCompiledMethodLoad(method, code_size);
}

void wrapperCompiledMethodUnload(jvmtiEnv *jvmti_env,
                                 jmethodID method,
                                 const void *code_addr) {
    notifyCompiledMethodUnload(method);
}

void wrapperDynamicCodeGenerated(jvmtiEnv *jvmti_env,
                                 const char *name,
                                 const void *address,
                                 jint length) {
    logi(LOG_TAG, "wrapperDynamicCodeGenerated");
}

void wrapperDataDumpRequest(jvmtiEnv *jvmti_env) {
    notifyDataDumpRequest();
}

void wrapperMonitorWait(jvmtiEnv *jvmti_env,
                        JNIEnv *jni_env,
                        jthread thread,
                        jobject object,
                        jlong timeout) {
    notifyMonitorWait(jni_env, thread, object, timeout);
}

void wrapperMonitorWaited(jvmtiEnv *jvmti_env,
                          JNIEnv *jni_env,
                          jthread thread,
                          jobject object,
                          jboolean timed_out) {
    notifyMonitorWaited(jni_env, thread, object, timed_out);
}

void wrapperMonitorContendedEnter(jvmtiEnv *jvmti_env,
                                  JNIEnv *jni_env,
                                  jthread thread,
                                  jobject object) {
    notifyMonitorContendedEnter(jni_env, thread, object);
}

void wrapperMonitorContendedEntered(jvmtiEnv *jvmti_env,
                                    JNIEnv *jni_env,
                                    jthread thread,
                                    jobject object) {
    notifyMonitorContendedEntered(jni_env, thread, object);
}

void wrapperResourceExhausted(jvmtiEnv *jvmti_env,
                              JNIEnv *jni_env,
                              jint flags,
                              const void *reserved,
                              const char *description) {
    notifyResourceExhausted();
}

void wrapperGarbageCollectionStart(jvmtiEnv *jvmti_env) {
    notifyGarbageCollectionStart();
}

void wrapperGarbageCollectionFinish(jvmtiEnv *jvmti_env) {
    notifyGarbageCollectionFinish();
}

void wrapperObjectFree(jvmtiEnv *jvmti_env, jlong tag) {
    notifyObjectFree();
}

void wrapperVMObjectAlloc(jvmtiEnv *jvmti_env,
                          JNIEnv *jni_env,
                          jthread thread,
                          jobject object,
                          jclass object_klass,
                          jlong size) {
    logi(LOG_TAG, "wrapperVMObjectAlloc");
}
//callback area end

