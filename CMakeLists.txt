cmake_minimum_required(VERSION 3.16)
project(Tremble LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/bin")

set(TREMBLE_VENDOR_CPP "${PROJECT_SOURCE_DIR}/vendor/loguru/loguru.cpp"
                       "${PROJECT_SOURCE_DIR}/vendor/backward-cpp/backward.cpp")

add_executable(Tremble src/core/main.cpp
                       src/core/engine.cpp
                       ${TREMBLE_VENDOR_CPP})
target_compile_definitions(Tremble PRIVATE LOGURU_THREADNAME_WIDTH=5)

target_include_directories(Tremble PRIVATE "${PROJECT_SOURCE_DIR}/vendor/loguru/"
                                           "${PROJECT_SOURCE_DIR}/vendor/backward-cpp")