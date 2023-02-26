cmake_minimum_required(VERSION 3.16)
project(Tremble LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/bin/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/bin/")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/bin/")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory("${PROJECT_SOURCE_DIR}/tools/map_builder")

# Third-party
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/glfw/ ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/glfw)

set(TREMBLE_VENDOR_CPP "${PROJECT_SOURCE_DIR}/vendor/loguru/loguru.cpp"
                       "${PROJECT_SOURCE_DIR}/vendor/backward-cpp/backward.cpp")

add_executable(Tremble src/core/main.cpp
                       src/core/engine.cpp
                       src/io/input_manager.cpp
                       src/io/window.cpp
                       src/util/log.cpp
                       src/world/map/map_file_parser.cpp
                       src/rendering/camera.cpp
                       src/rendering/naive_renderer.cpp
                       src/rendering/shader_program.cpp
                       ${TREMBLE_VENDOR_CPP})
target_compile_definitions(Tremble PRIVATE LOGURU_THREADNAME_WIDTH=5
                                           GLFW_INCLUDE_NONE=1)

target_include_directories(Tremble PRIVATE "${PROJECT_SOURCE_DIR}/vendor/loguru/"
                                           "${PROJECT_SOURCE_DIR}/vendor/backward-cpp"
                                           "${PROJECT_SOURCE_DIR}/vendor/glfw/include/"
                                           "${PROJECT_SOURCE_DIR}/vendor/glad"
                                           "${PROJECT_SOURCE_DIR}/vendor/glm"
                                           "${PROJECT_SOURCE_DIR}/tools/map_builder/src/lib/"
                                           "${PROJECT_SOURCE_DIR}/vendor/PhysX/physx/include/")

# TODO Fix
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(PHYX_BUILD_DIR_CONFIG "checked")
endif()
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(PHYX_BUILD_DIR_CONFIG "release")
endif()

find_library(PhysX_64 NAMES PhysX_64 PATHS "${PROJECT_SOURCE_DIR}/vendor/PhysX/physx/bin/*/${PHYX_BUILD_DIR_CONFIG}" NO_DEFAULT_PATHS)
find_library(PhysXCommon_64 NAMES PhysXCommon_64 PATHS "${PROJECT_SOURCE_DIR}/vendor/PhysX/physx/bin/*/${PHYX_BUILD_DIR_CONFIG}" NO_DEFAULT_PATHS)
find_library(PhysXFoundation_64 NAMES PhysXFoundation_64 PATHS "${PROJECT_SOURCE_DIR}/vendor/PhysX/physx/bin/*/${PHYX_BUILD_DIR_CONFIG}" NO_DEFAULT_PATHS)
target_link_libraries(Tremble PUBLIC glfw TrembleMapParserLib ${PhysX_64} ${PhysXCommon_64} ${PhysXFoundation_64})