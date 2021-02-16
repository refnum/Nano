cmake_minimum_required(VERSION 3.19)

project(Nano VERSION 4.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/Library/Support/CMake")
include(NanoSetup)

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Library/Project")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/NanoTest/Project")
