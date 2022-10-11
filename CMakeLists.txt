cmake_minimum_required(VERSION 3.19)

project(naja-verilog LANGUAGES CXX) 
include(CTest) 

set(CMAKE_CXX_STANDARD 20)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

add_subdirectory(src)
