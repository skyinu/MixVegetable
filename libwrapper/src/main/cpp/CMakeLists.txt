cmake_minimum_required(VERSION 3.10)
add_subdirectory(applog)
add_library(jvmti_wrapper SHARED
        jvmti-wrapper.c)
target_link_libraries(jvmti_wrapper
        android
        log
        applog)
target_include_directories(jvmti_wrapper PUBLIC applog)