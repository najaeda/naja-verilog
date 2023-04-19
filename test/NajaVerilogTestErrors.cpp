#include "gtest/gtest.h"

#include <filesystem>
#include <fstream>

#include "VerilogConstructor.h"
#include "VerilogException.h"

using namespace naja::verilog;

#include "VerilogConstructorTest.h"

#ifndef NAJA_VERILOG_BENCHMARKS
#define NAJA_VERILOG_BENCHMARKS "Undefined"
#endif

TEST(NajaVerilogTestErrors, test) {
  VerilogConstructorTest constructor;
  std::filesystem::path errorPath("does_not_exist.v");
  EXPECT_THROW(constructor.parse(errorPath), VerilogException);
}