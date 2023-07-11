#include <jni.h>
#include <string>
#include "stdio.h"
#include "unistd.h"
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_nativelib_NativeLib_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    pid_t result = fork();
    char log[50];
    sprintf(log, "Hello from  %d", result);
    return env->NewStringUTF(log);
}