#include "gtest/gtest.h"

#include <filesystem>
#include <fstream>

#include "VerilogConstructor.h"

using namespace naja::verilog;

#include "VerilogConstructorTest.h"

#ifndef NAJA_VERILOG_BENCHMARKS
#define NAJA_VERILOG_BENCHMARKS "Undefined"
#endif

TEST(NajaVerilogTest7, test) {
  VerilogConstructorTest constructor;
  std::filesystem::path test7Path(
      std::filesystem::path(NAJA_VERILOG_BENCHMARKS)
      / std::filesystem::path("benchmarks")
      / std::filesystem::path("test7.v"));
  constructor.parse(test7Path);
  ASSERT_EQ(1, constructor.modules_.size());
  auto test = constructor.modules_[0];
  EXPECT_EQ("test", test->name_);

  constructor.setFirstPass(false);
  constructor.parse(test7Path);

  ASSERT_EQ(1, constructor.modules_[0]->instances_.size());
  auto instance = constructor.modules_[0]->instances_[0];
  EXPECT_EQ("ins", instance.name_);
  EXPECT_EQ("mod", instance.model_);
  ASSERT_EQ(3, instance.parameterAssignments_.size());
  auto paramIt = instance.parameterAssignments_.find("PARAM0");
  ASSERT_TRUE(paramIt != instance.parameterAssignments_.end());
  EXPECT_TRUE(paramIt->second.empty());
  paramIt = instance.parameterAssignments_.find("PARAM1");
  ASSERT_TRUE(paramIt != instance.parameterAssignments_.end());
  EXPECT_EQ("A", paramIt->second);
  paramIt = instance.parameterAssignments_.find("PARAM2");
  ASSERT_TRUE(paramIt != instance.parameterAssignments_.end());
  EXPECT_EQ("VALUE", paramIt->second);
}