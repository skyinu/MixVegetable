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
