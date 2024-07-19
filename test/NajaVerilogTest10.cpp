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

TEST(NajaVerilogTest10, test) {
  VerilogConstructorTest constructor;
  std::filesystem::path test10Path(
      std::filesystem::path(NAJA_VERILOG_BENCHMARKS)
      / std::filesystem::path("benchmarks")
      / std::filesystem::path("test10.v"));

  constructor.setFirstPass(true);
  constructor.parse(test10Path);
  ASSERT_EQ(1, constructor.modules_.size());
  auto test = constructor.modules_[0];
  EXPECT_TRUE(test->ports_.empty());
  EXPECT_TRUE(test->nets_.empty());
  EXPECT_TRUE(test->assigns_.empty());
  EXPECT_TRUE(test->instances_.empty());
  constructor.setFirstPass(false);
  constructor.parse(test10Path);

  ASSERT_EQ(1, constructor.modules_.size());
  EXPECT_TRUE(test->ports_.empty());
  EXPECT_TRUE(test->nets_.empty());
  EXPECT_TRUE(test->assigns_.empty());
  EXPECT_EQ(1, test->instances_.size());
  auto instance = test->instances_[0];
  EXPECT_EQ("ins", instance.identifier_.name_);
  EXPECT_EQ("MOD", instance.model_.name_);
  EXPECT_EQ(3, instance.parameterAssignments_.size());
  using Parameters = std::vector<std::pair<std::string, std::string>>;
  Parameters parameters;
  for (const auto& [parameter, value]: instance.parameterAssignments_) {
    parameters.push_back({parameter, value});
  }
  EXPECT_EQ("elem10", parameters[0].first);
  EXPECT_EQ("'d0", parameters[0].second);
  EXPECT_EQ("elem11", parameters[1].first);
  EXPECT_EQ("10'sd0", parameters[1].second);
  EXPECT_EQ("elem12", parameters[2].first);
  EXPECT_EQ("'sd0", parameters[2].second);
}