#include "gtest/gtest.h"

#include <filesystem>
#include <fstream>

#include "VerilogConstructor.h"

using namespace naja::verilog;

#include "VerilogConstructorTest.h"

#ifndef NAJA_VERILOG_BENCHMARKS
#define NAJA_VERILOG_BENCHMARKS "Undefined"
#endif

TEST(NajaVerilogTestMultipleFiles, test) {
  VerilogConstructorTest constructor;
  std::filesystem::path multipleFilesPath(
      std::filesystem::path(NAJA_VERILOG_BENCHMARKS)
      / std::filesystem::path("benchmarks")
      / std::filesystem::path("multifiles"));
  VerilogConstructor::Paths paths;
  paths.push_back(multipleFilesPath / std::filesystem::path("mod0.v"));
  paths.push_back(multipleFilesPath / std::filesystem::path("mod1.v"));
  paths.push_back(multipleFilesPath / std::filesystem::path("top.v"));
  constructor.parse(paths);
  ASSERT_EQ(3, constructor.modules_.size());
  EXPECT_EQ("mod0", constructor.modules_[0]->name_);
  EXPECT_EQ("mod1", constructor.modules_[1]->name_);
  EXPECT_EQ("top", constructor.modules_[2]->name_);
  ASSERT_EQ(2, constructor.modules_[0]->ports_.size());
  ASSERT_EQ(2, constructor.modules_[1]->ports_.size());
  ASSERT_EQ(2, constructor.modules_[2]->ports_.size());

  constructor.setFirstPass(false);
  constructor.parse(paths);


  EXPECT_TRUE(constructor.modules_[0]->instances_.empty());
  EXPECT_TRUE(constructor.modules_[1]->instances_.empty());
  ASSERT_EQ(2, constructor.modules_[2]->instances_.size());
  auto ins0 = constructor.modules_[2]->instances_[0];
  EXPECT_EQ("ins0", ins0.name_);
  EXPECT_EQ("mod0", ins0.model_);
  auto ins1 = constructor.modules_[2]->instances_[1];
  EXPECT_EQ("ins1", ins1.name_);
  EXPECT_EQ("mod1", ins1.model_);
}
