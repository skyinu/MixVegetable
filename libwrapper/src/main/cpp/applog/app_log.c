//
// Created by skyinu on 2019/12/5.
//

#include "android/log.h"
#include "app_log_config.h"

void logv(char *tag, char *message) {
    if (OPEN_LOG) {
        __android_log_print(ANDROID_LOG_VERBOSE, tag, "%s", message);
    }
}

void logd(char *tag, char *message) {
    if (OPEN_LOG) {
        __android_log_print(ANDROID_LOG_DEBUG, tag, "%s", message);
    }
}

void logi(char *tag, char *message) {
    if (OPEN_LOG) {
        __android_log_print(ANDROID_LOG_INFO, tag, "%s", message);
    }
}

void logw(char *tag, char *message) {
    if (OPEN_LOG) {
        __android_log_print(ANDROID_LOG_WARN, tag, "%s", message);
    }
}

void loge(char *tag, char *message) {
    if (OPEN_LOG) {
        __android_log_print(ANDROID_LOG_ERROR, tag, "%s", message);
    }
}