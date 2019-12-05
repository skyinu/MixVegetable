//
// Created by skyinu on 2019/12/4.
//

#include <jni.h>
#include"string.h"
#include "applog/app_log.h"

#define LOG_TAG "TAG_WRAPPER"

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    logi(LOG_TAG, "on library load");
    return JNI_VERSION_1_6;
}

