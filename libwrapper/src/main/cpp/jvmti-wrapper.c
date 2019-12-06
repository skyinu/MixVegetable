//
// Created by skyinu on 2019/12/4.
//

#include <jni.h>
#include"string.h"
#include "applog/app_log.h"
#include "jvmti.h"
#include "malloc.h"

#define LOG_TAG "TAG_WRAPPER"
#define MAX_LOG_LENGTH 500

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
    return JNI_OK;
}

