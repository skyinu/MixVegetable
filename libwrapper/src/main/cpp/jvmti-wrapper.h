//
// Created by skyinu on 2019-12-14.
//

#include "jvmti.h"

#ifndef MIXVEGETABLE_MASTER_JVMTI_WRAPPER_H
#define MIXVEGETABLE_MASTER_JVMTI_WRAPPER_H

#endif //MIXVEGETABLE_MASTER_JVMTI_WRAPPER_H

void wrappperVMInit(jvmtiEnv *jvmti_env,
                    JNIEnv *jni_env,
                    jthread thread);

void wrapperVMDeath(jvmtiEnv *jvmti_env,
                    JNIEnv *jni_env);

void wrapperThreadStart(jvmtiEnv *jvmti_env,
                        JNIEnv *jni_env,
                        jthread thread);

void wrapperThreadEnd(jvmtiEnv *jvmti_env,
                      JNIEnv *jni_env,
                      jthread thread);

void wrapperClassFileLoadHook(jvmtiEnv *jvmti_env,
                              JNIEnv *jni_env,
                              jclass class_being_redefined,
                              jobject loader,
                              const char *name,
                              jobject protection_domain,
                              jint class_data_len,
                              const unsigned char *class_data,
                              jint *new_class_data_len,
                              unsigned char **new_class_data);

void wrapperClassLoad(jvmtiEnv *jvmti_env,
                      JNIEnv *jni_env,
                      jthread thread,
                      jclass klass);

void wrapperClassPrepare(jvmtiEnv *jvmti_env,
                         JNIEnv *jni_env,
                         jthread thread,
                         jclass klass);

void wrapperMethodEntry(jvmtiEnv *jvmti_env,
                        JNIEnv *jni_env,
                        jthread thread,
                        jmethodID method);

void wrapperMethodExit(jvmtiEnv *jvmti_env,
                       JNIEnv *jni_env,
                       jthread thread,
                       jmethodID method,
                       jboolean was_popped_by_exception,
                       jvalue return_value);

void wrapperNativeMethodBind(jvmtiEnv *jvmti_env,
                             JNIEnv *jni_env,
                             jthread thread,
                             jmethodID method,
                             void *address,
                             void **new_address_ptr);

void wrapperCompiledMethodLoad(jvmtiEnv *jvmti_env,
                               jmethodID method,
                               jint code_size,
                               const void *code_addr,
                               jint map_length,
                               const jvmtiAddrLocationMap *map,
                               const void *compile_info);

void wrapperCompiledMethodUnload(jvmtiEnv *jvmti_env,
                                 jmethodID method,
                                 const void *code_addr);

void wrapperDynamicCodeGenerated(jvmtiEnv *jvmti_env,
                                 const char *name,
                                 const void *address,
                                 jint length);

void wrapperDataDumpRequest(jvmtiEnv *jvmti_env);

void wrapperMonitorWait(jvmtiEnv *jvmti_env,
                        JNIEnv *jni_env,
                        jthread thread,
                        jobject object,
                        jlong timeout);

void wrapperMonitorWaited(jvmtiEnv *jvmti_env,
                          JNIEnv *jni_env,
                          jthread thread,
                          jobject object,
                          jboolean timed_out);

void wrapperMonitorContendedEnter(jvmtiEnv *jvmti_env,
                                  JNIEnv *jni_env,
                                  jthread thread,
                                  jobject object);

void wrapperMonitorContendedEntered(jvmtiEnv *jvmti_env,
                                    JNIEnv *jni_env,
                                    jthread thread,
                                    jobject object);

void wrapperResourceExhausted(jvmtiEnv *jvmti_env,
                              JNIEnv *jni_env,
                              jint flags,
                              const void *reserved,
                              const char *description);

void wrapperGarbageCollectionStart(jvmtiEnv *jvmti_env);

void wrapperGarbageCollectionFinish(jvmtiEnv *jvmti_env);

void wrapperObjectFree(jvmtiEnv *jvmti_env, jlong tag);

void wrapperVMObjectAlloc(jvmtiEnv *jvmti_env,
                          JNIEnv *jni_env,
                          jthread thread,
                          jobject object,
                          jclass object_klass,
                          jlong size);
