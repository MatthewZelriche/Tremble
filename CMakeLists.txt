cmake_minimum_required(VERSION 3.16)
project(Tremble LANGUAGES CXX)

include(GenerateExportHeader)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Tremble SHARED src/c_api/src/test.cpp)

generate_export_header(Tremble EXPORT_FILE_NAME ${PROJECT_SOURCE_DIR}/include/Tremble/core/tremble_export.hpp)

target_include_directories(Tremble PUBLIC ${PROJECT_SOURCE_DIR}/include/)