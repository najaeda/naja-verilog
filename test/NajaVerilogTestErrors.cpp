// SPDX-FileCopyrightText: 2023 The Naja verilog authors <https://github.com/najaeda/naja-verilog/blob/main/AUTHORS>
//
// SPDX-License-Identifier: Apache-2.0

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

TEST(NajaVerilogTestErrors, testErrorPath) {
  VerilogConstructorTest constructor;
  std::filesystem::path errorPath("does_not_exist.v");
  EXPECT_THROW(constructor.parse(errorPath), VerilogException);
}

TEST(NajaVerilogTestErrors, test0) {
  std::filesystem::path error0Path(
      std::filesystem::path(NAJA_VERILOG_BENCHMARKS)
      / std::filesystem::path("benchmarks")
      / std::filesystem::path("errors")
      / std::filesystem::path("error0.v"));
  VerilogConstructorTest constructor;
  EXPECT_THROW(constructor.parse(error0Path), VerilogException);
}

TEST(NajaVerilogTestErrors, test1) {
  std::filesystem::path error1Path(
      std::filesystem::path(NAJA_VERILOG_BENCHMARKS)
      / std::filesystem::path("benchmarks")
      / std::filesystem::path("errors")
      / std::filesystem::path("error1.v"));
  VerilogConstructorTest constructor;
  EXPECT_THROW(constructor.parse(error1Path), VerilogException);
}

TEST(NajaVerilogTestErrors, test2) {
  std::filesystem::path error2Path(
      std::filesystem::path(NAJA_VERILOG_BENCHMARKS)
      / std::filesystem::path("benchmarks")
      / std::filesystem::path("errors")
      / std::filesystem::path("error2.v"));
  VerilogConstructorTest constructor;
  EXPECT_THROW(constructor.parse(error2Path), VerilogException);
}