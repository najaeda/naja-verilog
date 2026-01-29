# SPDX-FileCopyrightText: 2023 The Naja verilog authors <https://github.com/najaeda/naja-verilog/blob/main/AUTHORS>
#
# SPDX-License-Identifier: Apache-2.0

include(GoogleTest)

set(tests
  VerilogConstructorTest.cpp
  NajaVerilogTest012.cpp
  NajaVerilogTest3.cpp
  NajaVerilogTest4.cpp
  NajaVerilogTest5.cpp
  NajaVerilogTest6.cpp
  NajaVerilogTest7.cpp
  NajaVerilogTest8.cpp
  NajaVerilogTest9.cpp
  NajaVerilogTest10.cpp
  NajaVerilogTest11.cpp
  NajaVerilogTest12.cpp
  NajaVerilogTest13.cpp
  NajaVerilogTest14.cpp
  NajaVerilogTest15.cpp
  NajaVerilogTest16.cpp
  NajaVerilogTest17.cpp
  NajaVerilogTestSize.cpp
  NajaVerilogTestPreprocess.cpp
  NajaVerilogTestMultipleFiles.cpp
  NajaVerilogTestErrors.cpp
)

add_executable(naja_verilog_tests ${tests})

target_link_libraries(naja_verilog_tests naja_verilog gmock gtest_main)

if(BUILD_EMSCRIPTEN)
  target_compile_options(naja_verilog_tests PRIVATE -fexceptions)
  target_link_options(naja_verilog_tests PRIVATE
  -sEXIT_RUNTIME=1 -sASSERTIONS=0 -sVERBOSE=0
  -sALLOW_MEMORY_GROWTH=1 -sDISABLE_EXCEPTION_CATCHING=0
  --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/benchmarks@
  --verbose)
  target_compile_definitions(naja_verilog_tests PRIVATE NAJA_VERILOG_BENCHMARKS="")
else()
  target_compile_definitions(naja_verilog_tests PRIVATE
    NAJA_VERILOG_BENCHMARKS="${CMAKE_CURRENT_SOURCE_DIR}/benchmarks")
endif()

GTEST_DISCOVER_TESTS(naja_verilog_tests)
