cmake_minimum_required(VERSION 3.16)
project(TrembleMapParser LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/tools")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}/tools")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}/tools")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(TrembleMapParserLib STATIC src/lib/file_parser.cpp
                                       src/lib/geometry_constructor.cpp
                                       src/lib/math.cpp
                                       src/lib/serialization.cpp)

add_executable(TrembleMapParser src/main.cpp)
target_include_directories(TrembleMapParserLib PUBLIC "${CMAKE_SOURCE_DIR}/vendor/glm"
                                                    "${CMAKE_SOURCE_DIR}/vendor/stb_image"
                                                    "${CMAKE_SOURCE_DIR}/vendor/cereal/include/")
target_link_libraries(TrembleMapParser PUBLIC TrembleMapParserLib)