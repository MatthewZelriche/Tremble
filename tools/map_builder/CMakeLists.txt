cmake_minimum_required(VERSION 3.16)
project(TrembleMapParser LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/bin")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(TrembleMapParser src/main.cpp
                                src/file_parser.cpp
                                src/geometry_constructor.cpp)
target_include_directories(TrembleMapParser PRIVATE "${CMAKE_SOURCE_DIR}/vendor/glm"
                                                    "${CMAKE_SOURCE_DIR}/vendor/stb_image")