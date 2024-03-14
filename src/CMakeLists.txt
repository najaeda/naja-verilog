# SPDX-FileCopyrightText: 2023 The Naja verilog authors <https://github.com/najaeda/naja-verilog/blob/main/AUTHORS>
#
# SPDX-License-Identifier: Apache-2.0

BISON_TARGET(VerilogParser VerilogParser.yy
  ${CMAKE_CURRENT_BINARY_DIR}/VerilogParser.cpp
  COMPILE_FLAGS "-Wconflicts-sr -d -v")
FLEX_TARGET(VerilogScanner VerilogScanner.ll
  ${CMAKE_CURRENT_BINARY_DIR}/VerilogScanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(VerilogScanner VerilogParser)

set(SOURCES
  ${BISON_VerilogParser_OUTPUTS}
  ${FLEX_VerilogScanner_OUTPUTS}
  VerilogTypes.cpp
  VerilogConstructor.cpp
)

set(HEADERS
  VerilogTypes.h
  VerilogConstructor.h
  VerilogException.h
)

add_library(naja_verilog STATIC ${SOURCES})
set_property(TARGET naja_verilog PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(naja_verilog PUBLIC naja_verilog_coverage_config naja_verilog_sanitizers_config)
target_include_directories(naja_verilog
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ${FLEX_INCLUDE_DIRS}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(naja_verilog PROPERTIES PUBLIC_HEADER "${HEADERS}")
install(TARGETS naja_verilog LIBRARY DESTINATION lib PUBLIC_HEADER DESTINATION include) 

add_executable(naja_verilog_test NajaVerilogSnippet.cpp)
target_link_libraries(naja_verilog_test naja_verilog)