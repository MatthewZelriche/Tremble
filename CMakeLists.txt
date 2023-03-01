cmake_minimum_required(VERSION 3.16)
project(Tremble LANGUAGES CXX)

set(OUTPUT_DIR "${PROJECT_SOURCE_DIR}/build/bin/${CMAKE_BUILD_TYPE}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})

include(ExternalProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(PHYSX_BUILD "checked")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(PHYSX_BUILD "release")
endif()

if (MSVC)
    set(PHYSX_BUILD_EXT ".bat")
    set(PHYSX_PRESET "vc17win64")
    set(PHYSX_CMAKELISTS "compiler/vc17win64/")
    set(ADD_COMMAND "--target ALL_BUILD")
else()
    set(PHYSX_BUILD_EXT ".sh")
    set(PHYSX_PRESET "linux")
    set(PHYSX_CMAKELISTS "compiler/linux-${PHYSX_BUILD}")
endif()

ExternalProject_Add(Physx 
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/PhysX/physx/
    CONFIGURE_COMMAND ${PROJECT_SOURCE_DIR}/vendor/PhysX/physx/generate_projects${PHYSX_BUILD_EXT} ${PHYSX_PRESET}
    BUILD_IN_SOURCE 1
    BUILD_COMMAND cmake --build ${PHYSX_CMAKELISTS} --config ${PHYSX_BUILD} ${ADD_COMMAND}
    INSTALL_COMMAND "") 

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

add_dependencies(Tremble Physx) # Force Physx to be built first
target_compile_definitions(Tremble PRIVATE LOGURU_THREADNAME_WIDTH=5
                                           GLFW_INCLUDE_NONE=1)

target_include_directories(Tremble PRIVATE "${PROJECT_SOURCE_DIR}/vendor/loguru/"
                                           "${PROJECT_SOURCE_DIR}/vendor/backward-cpp"
                                           "${PROJECT_SOURCE_DIR}/vendor/glfw/include/"
                                           "${PROJECT_SOURCE_DIR}/vendor/glad"
                                           "${PROJECT_SOURCE_DIR}/vendor/glm"
                                           "${PROJECT_SOURCE_DIR}/tools/map_builder/src/lib/"
                                           "${PROJECT_SOURCE_DIR}/vendor/PhysX/physx/include/")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Not set automatically on linux like MSVC does...needed for Physx
    add_compile_definitions(_DEBUG)
    set(PHYX_BUILD_DIR_CONFIG "checked")
endif()
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(PHYX_BUILD_DIR_CONFIG "release")
endif()
if (MSVC)
    set(PHYSX_BUILD_DIR "${PROJECT_SOURCE_DIR}/vendor/PhysX/physx/bin/win.x86_64.vc143.mt")
    set(PHYSX_LIB_PREFIX "")
    set(PHYSX_LIB_EXT ".lib")
else()
    set(PHYSX_BUILD_DIR "${PROJECT_SOURCE_DIR}/vendor/PhysX/physx/bin/linux.clang/${PHYX_BUILD_DIR_CONFIG}")
    set(PHYSX_LIB_PREFIX "lib")
    set(PHYSX_LIB_EXT ".so")
endif()

message(${PHYSX_BUILD_DIR})
target_link_libraries(Tremble PUBLIC 
                        glfw 
                        TrembleMapParserLib
                        ${PHYSX_BUILD_DIR}/${PHYSX_LIB_PREFIX}PhysXCommon_64${PHYSX_LIB_EXT}
                        ${PHYSX_BUILD_DIR}/${PHYSX_LIB_PREFIX}PhysX_64${PHYSX_LIB_EXT}
                        ${PHYSX_BUILD_DIR}/${PHYSX_LIB_PREFIX}PhysXFoundation_64${PHYSX_LIB_EXT})

# copy over the built Physx libraries as a post build step
add_custom_command(TARGET Physx POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                    ${PHYSX_BUILD_DIR}/${PHYSX_LIB_PREFIX}PhysXCommon_64${PHYSX_LIB_EXT} 
                    ${OUTPUT_DIR}/${PHYSX_LIB_PREFIX}PhysXCommon_64${PHYSX_LIB_EXT})
add_custom_command(TARGET Physx POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                    ${PHYSX_BUILD_DIR}/${PHYSX_LIB_PREFIX}PhysX_64${PHYSX_LIB_EXT}
                    ${OUTPUT_DIR}/${PHYSX_LIB_PREFIX}PhysX_64${PHYSX_LIB_EXT})
add_custom_command(TARGET Physx POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                    ${PHYSX_BUILD_DIR}/${PHYSX_LIB_PREFIX}PhysXFoundation_64${PHYSX_LIB_EXT}
                    ${OUTPUT_DIR}/${PHYSX_LIB_PREFIX}PhysXFoundation_64${PHYSX_LIB_EXT})

# ugly hack to force this post build step to always occur
add_custom_target(dummy_target ALL
    DEPENDS move_res_folder)
add_custom_command(OUTPUT move_res_folder POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_directory
                        ${PROJECT_SOURCE_DIR}/res ${OUTPUT_DIR}/res)