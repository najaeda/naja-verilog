# SPDX-FileCopyrightText: 2023 The Naja verilog authors <https://github.com/najaeda/naja-verilog/blob/main/AUTHORS>
#
# SPDX-License-Identifier: Apache-2.0

if(PREGENERATED_PARSER_SOURCES)
  message(STATUS "Using pre-generated Flex/Bison sources")

  set(BISON_VerilogParser_OUTPUTS
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/VerilogParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/VerilogParser.hpp
  )

  set(FLEX_VerilogScanner_OUTPUTS
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/VerilogScanner.cpp
  )

  target_include_directories(naja_verilog SYSTEM BEFORE PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/generated)
else()
  BISON_TARGET(VerilogParser VerilogParser.yy
    ${CMAKE_CURRENT_BINARY_DIR}/VerilogParser.cpp
    COMPILE_FLAGS "-d -v -Wconflicts-sr -Wconflicts-rr")

  FLEX_TARGET(VerilogScanner VerilogScanner.ll
    ${CMAKE_CURRENT_BINARY_DIR}/VerilogScanner.cpp)

  ADD_FLEX_BISON_DEPENDENCY(VerilogScanner VerilogParser)
endif()

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
if (BUILD_EMSCRIPTEN)
  target_compile_options(naja_verilog PRIVATE -fexceptions)
endif()
target_link_libraries(naja_verilog PUBLIC naja_verilog_coverage_config naja_verilog_sanitizers_config)

target_include_directories(naja_verilog
  BEFORE PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(naja_verilog
  AFTER PRIVATE ${FLEX_INCLUDE_DIRS})
target_include_directories(naja_verilog
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (WIN32)
  target_include_directories(naja_verilog
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/generated)
endif()

if(PROJECT_IS_TOP_LEVEL)
  set_target_properties(naja_verilog PROPERTIES PUBLIC_HEADER "${HEADERS}")
  install(TARGETS naja_verilog
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  add_executable(naja_verilog_test NajaVerilogSnippet.cpp)
  target_link_libraries(naja_verilog_test naja_verilog)
endif()
