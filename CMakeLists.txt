cmake_minimum_required(VERSION 3.16)
project(Tremble LANGUAGES CXX)

# Generator expr to force correct resolution of this dir for MSVC.
set(OUTPUT_DIR "${PROJECT_SOURCE_DIR}/build/bin/${CMAKE_BUILD_TYPE}/$<0:>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Tremble SHARED src/c_api/test.cpp)