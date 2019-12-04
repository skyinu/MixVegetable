cmake_minimum_required(VERSION 3.10)
add_library(jvmti_wrapper SHARED
            jvmti-wrapper.c)
target_link_libraries(jvmti_wrapper
                      android
                      log)