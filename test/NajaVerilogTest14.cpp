// SPDX-FileCopyrightText: 2024 The Naja verilog authors <https://github.com/najaeda/naja-verilog/blob/main/AUTHORS>
//
// SPDX-License-Identifier: Apache-2.0

#include "gtest/gtest.h"

#include <filesystem>
#include <fstream>

#include "VerilogConstructor.h"

using namespace naja::verilog;

#include "VerilogConstructorTest.h"

#ifndef NAJA_VERILOG_BENCHMARKS
#define NAJA_VERILOG_BENCHMARKS "Undefined"
#endif

TEST(NajaVerilogTest14, test) {
  VerilogConstructorTest constructor;
  std::filesystem::path test14Path(
      std::filesystem::path(NAJA_VERILOG_BENCHMARKS)
      / std::filesystem::path("benchmarks")
      / std::filesystem::path("test14.v"));
  constructor.parse(test14Path);
  ASSERT_EQ(1, constructor.modules_.size());
  auto adder = constructor.modules_[0];
  EXPECT_EQ("adder", adder->identifier_.getString());

  constructor.setFirstPass(false);
  constructor.parse(test14Path);
}