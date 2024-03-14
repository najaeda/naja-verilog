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
  NajaVerilogTestMultipleFiles.cpp
  NajaVerilogTestErrors.cpp
)

add_executable(naja_verilog_tests ${tests})
target_compile_definitions(naja_verilog_tests PRIVATE
  NAJA_VERILOG_BENCHMARKS="${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(naja_verilog_tests naja_verilog gmock gtest_main)

GTEST_DISCOVER_TESTS(naja_verilog_tests)