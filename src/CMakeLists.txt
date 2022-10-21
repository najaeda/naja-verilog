BISON_TARGET(VerilogParser VerilogParser.yy
  ${CMAKE_CURRENT_BINARY_DIR}/VerilogParser.cpp
  COMPILE_FLAGS "-Wconflicts-sr -Wcounterexamples -d -v")
FLEX_TARGET(VerilogScanner VerilogScanner.ll
  ${CMAKE_CURRENT_BINARY_DIR}/VerilogScanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(VerilogScanner VerilogParser)

set(verilog_sources
  ${BISON_VerilogParser_OUTPUTS}
  ${FLEX_VerilogScanner_OUTPUTS}
  VerilogConstructor.cpp
)

add_library(naja_verilog STATIC ${verilog_sources})
target_link_libraries(naja_verilog PUBLIC coverage_config sanitizers_config)
target_include_directories(naja_verilog
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(naja_verilog_test NajaVerilogSnippet.cpp)
target_link_libraries(naja_verilog_test naja_verilog)
