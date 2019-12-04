//
// Created by skyinu on 2019/12/4.
//

#include <jni.h>
#include"string.h"
#include "android/log.h"

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    __android_log_print(ANDROID_LOG_INFO, "wrapper", "on library load");
    return JNI_VERSION_1_6;
}

