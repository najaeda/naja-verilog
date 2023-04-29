cmake_minimum_required(VERSION 3.18)

project(naja-verilog LANGUAGES CXX) 

set(CMAKE_CXX_STANDARD 20)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

add_library(naja_verilog_coverage_config INTERFACE)
option(CODE_COVERAGE "Enable coverage reporting" OFF)
if(CODE_COVERAGE)
  # Add required flags (GCC & LLVM/Clang)
  target_compile_options(naja_verilog_coverage_config INTERFACE
    -O0        # no optimization
    -g         # generate debug info
    --coverage # sets all required flags
  )
  target_link_options(naja_verilog_coverage_config INTERFACE --coverage)
endif(CODE_COVERAGE)

add_library(naja_verilog_sanitizers_config INTERFACE)
option(ENABLE_SANITIZERS "Enable sanitizers in compilation" OFF)
if(ENABLE_SANITIZERS)
  target_compile_options(naja_verilog_sanitizers_config INTERFACE
    -fsanitize=address
    -fno-omit-frame-pointer
    #-fsanitize=thread
  )
  target_link_options(naja_verilog_sanitizers_config INTERFACE
    -fsanitize=address
    #-fsanitize=thread
  )
endif(ENABLE_SANITIZERS)

add_subdirectory(src)
add_subdirectory(cmake)

#Build tests if googletest submodule has been added
if(EXISTS ${PROJECT_SOURCE_DIR}/thirdparty/googletest/CMakeLists.txt)
  add_subdirectory(thirdparty)
  include(CTest)
  enable_testing()
  add_subdirectory(test)
endif(EXISTS ${PROJECT_SOURCE_DIR}/thirdparty/googletest/CMakeLists.txt)
