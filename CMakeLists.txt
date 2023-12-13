cmake_minimum_required(VERSION 3.16)
project(Tremble LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Tremble SHARED src/c_api/test.cpp)