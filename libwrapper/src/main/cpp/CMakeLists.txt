cmake_minimum_required(VERSION 3.10)
add_subdirectory(applog)
add_subdirectory(appbridge)
add_library(jvmti_wrapper SHARED
        jvmti-wrapper.c)
target_link_libraries(jvmti_wrapper
        android
        log
        applog
        appbridge)
target_include_directories(jvmti_wrapper PUBLIC ${PROJECT_SOURCE_DIR}/applog ${PROJECT_SOURCE_DIR}/appbridge)
