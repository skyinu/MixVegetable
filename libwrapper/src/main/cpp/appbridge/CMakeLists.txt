cmake_minimum_required(VERSION 3.10)
message("----------bridge binary path-------------")
message("${CMAKE_CURRENT_BINARY_DIR}")
add_library(appbridge STATIC app_bridge.c node.c java_bridge.c)
include_directories(appbridge ${CMAKE_CURRENT_BINARY_DIR})
