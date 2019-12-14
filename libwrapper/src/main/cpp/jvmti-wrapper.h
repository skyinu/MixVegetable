//
// Created by skyinu on 2019-12-14.
//

#include "jvmti.h"

#ifndef MIXVEGETABLE_MASTER_JVMTI_WRAPPER_H
#define MIXVEGETABLE_MASTER_JVMTI_WRAPPER_H

#endif //MIXVEGETABLE_MASTER_JVMTI_WRAPPER_H

void wrapperMethodEntry(jvmtiEnv *jvmti_env,
                 JNIEnv *jni_env,
                 jthread thread,
                 jmethodID method);
