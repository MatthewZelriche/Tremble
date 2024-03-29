cmake_minimum_required(VERSION 3.16)
project(Tremble LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(GenerateExportHeader)

set(TR_OUTPUT_DIR "${CMAKE_BUILD_TYPE}/$<0:>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${TR_OUTPUT_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${TR_OUTPUT_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${TR_OUTPUT_DIR}")

add_subdirectory("vendor/fmt")
add_subdirectory("vendor/glfw")

add_library(Tremble SHARED  src/core/engine.cpp
                            src/io/config.cpp
                            src/io/window.cpp
                            src/c_api/core/engine.cpp)

if (MSVC)
    target_compile_options(Tremble PRIVATE /Zc:preprocessor)
endif()

target_link_libraries(Tremble fmt::fmt glfw)

generate_export_header(Tremble EXPORT_FILE_NAME ${PROJECT_SOURCE_DIR}/include/Tremble/core/tremble_export.hpp)

target_include_directories(Tremble PUBLIC ${PROJECT_SOURCE_DIR}/include/)
target_include_directories(Tremble PUBLIC ${PROJECT_SOURCE_DIR}/vendor/optional/include/
                                          ${PROJECT_SOURCE_DIR}/vendor/expected/include/
                                          ${PROJECT_SOURCE_DIR}/vendor/tomlplusplus/include/
                                          ${PROJECT_SOURCE_DIR}/vendor/glfw/include/)