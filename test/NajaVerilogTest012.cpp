#include "gtest/gtest.h"

#include <filesystem>
#include <fstream>

#include "VerilogConstructor.h"

using namespace naja::verilog;

#include "VerilogConstructorTest.h"

#ifndef NAJA_VERILOG_BENCHMARKS
#define NAJA_VERILOG_BENCHMARKS "Undefined"
#endif

TEST(NajaVerilogTest012, test0) {
  VerilogConstructorTest constructor;
  std::filesystem::path test0Path(
      std::filesystem::path(NAJA_VERILOG_BENCHMARKS)
      / std::filesystem::path("benchmarks")
      / std::filesystem::path("test0.v"));
  constructor.parse(test0Path);
  EXPECT_EQ(1, constructor.modules_.size());
  EXPECT_EQ("test", constructor.modules_[0].name_);
}

TEST(NajaVerilogTest012, test1) {
  VerilogConstructorTest constructor;
  std::filesystem::path test1Path(
      std::filesystem::path(NAJA_VERILOG_BENCHMARKS)
      / std::filesystem::path("benchmarks")
      / std::filesystem::path("test1.v"));
  constructor.parse(test1Path);
  EXPECT_EQ(1, constructor.modules_.size());
  EXPECT_EQ("test", constructor.modules_[0].name_);
}

TEST(NajaVerilogTest012, test2) {
  VerilogConstructorTest constructor;
  std::filesystem::path test1Path(
      std::filesystem::path(NAJA_VERILOG_BENCHMARKS)
      / std::filesystem::path("benchmarks")
      / std::filesystem::path("test2.v"));
  constructor.parse(test1Path);
  EXPECT_EQ(1, constructor.modules_.size());
  EXPECT_EQ("test", constructor.modules_[0].name_);
}