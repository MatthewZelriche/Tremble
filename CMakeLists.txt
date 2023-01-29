cmake_minimum_required(VERSION 3.16)
project(Tremble LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/bin")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Third-party
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/glfw/ ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/glfw)

set(TREMBLE_VENDOR_CPP "${PROJECT_SOURCE_DIR}/vendor/loguru/loguru.cpp"
                       "${PROJECT_SOURCE_DIR}/vendor/backward-cpp/backward.cpp")

add_executable(Tremble src/core/main.cpp
                       src/core/engine.cpp
                       src/io/input_manager.cpp
                       src/io/window.cpp
                       src/util/log.cpp
                       src/rendering/GLContext.cpp
                       ${TREMBLE_VENDOR_CPP})
target_compile_definitions(Tremble PRIVATE LOGURU_THREADNAME_WIDTH=5
                                           GLFW_INCLUDE_NONE=1)

target_include_directories(Tremble PRIVATE "${PROJECT_SOURCE_DIR}/vendor/loguru/"
                                           "${PROJECT_SOURCE_DIR}/vendor/backward-cpp"
                                           "${PROJECT_SOURCE_DIR}/vendor/glfw/include/"
                                           "${PROJECT_SOURCE_DIR}/vendor/glad")
target_link_libraries(Tremble PRIVATE glfw)