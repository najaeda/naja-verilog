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

TEST(NajaVerilogTest12, test) {
  VerilogConstructorTest constructor;
  std::filesystem::path test12Path(
      std::filesystem::path(NAJA_VERILOG_BENCHMARKS)
      / std::filesystem::path("benchmarks")
      / std::filesystem::path("test12.v"));

  constructor.setFirstPass(true);
  constructor.parse(test12Path);
  ASSERT_EQ(1, constructor.modules_.size());
  auto test = constructor.modules_[0];
  EXPECT_TRUE(test->ports_.empty());
  EXPECT_TRUE(test->nets_.empty());
  EXPECT_TRUE(test->assigns_.empty());
  EXPECT_TRUE(test->instances_.empty());

  constructor.setFirstPass(false);
  constructor.parse(test12Path);
  ASSERT_EQ(1, constructor.modules_.size());
  EXPECT_TRUE(test->ports_.empty());
  EXPECT_TRUE(test->assigns_.empty());
  ASSERT_EQ(2, test->nets_.size());
  ASSERT_EQ(3, test->instances_.size());

  auto ins1 = test->instances_[0];
  EXPECT_EQ("ins1", ins1.identifier_.name_);
  EXPECT_FALSE(ins1.identifier_.escaped_);
  EXPECT_EQ("CFG1", ins1.model_.name_);
  EXPECT_TRUE(ins1.parameterAssignments_.empty());

  auto mem = test->instances_[1];
  EXPECT_EQ("mem_regfile_mem_regfile_0_0", mem.identifier_.name_);
  EXPECT_FALSE(mem.identifier_.escaped_);
  EXPECT_EQ("RAM64x18", mem.model_.name_);
  EXPECT_TRUE(mem.parameterAssignments_.empty());

  auto ins2 = test->instances_[2];
  EXPECT_EQ("$$ins2@@", ins2.identifier_.name_);
  EXPECT_FALSE(ins2.identifier_.escaped_);
  EXPECT_EQ("CFG1", ins2.model_.name_);
  EXPECT_TRUE(ins2.parameterAssignments_.empty());

  EXPECT_EQ(3, test->defParameterAssignments_.size());
  auto def0Path = test->defParameterAssignments_[0].first;
  auto def0Value = test->defParameterAssignments_[0].second;
  EXPECT_EQ(2, def0Path.size());
  EXPECT_EQ("ins1", def0Path[0].name_);
  EXPECT_FALSE(def0Path[0].escaped_);
  EXPECT_EQ("INIT", def0Path[1].name_);
  EXPECT_FALSE(def0Path[1].escaped_);
  EXPECT_EQ("2'h1", def0Value.getString());
  EXPECT_EQ(Expression::Type::NUMBER ,def0Value.value_.index());

  auto def1Path = test->defParameterAssignments_[1].first;
  auto def1Value = test->defParameterAssignments_[1].second;
  EXPECT_EQ(2, def1Path.size());
  EXPECT_EQ("mem_regfile_mem_regfile_0_0", def1Path[0].name_);
  EXPECT_FALSE(def1Path[0].escaped_);
  EXPECT_EQ("RAMINDEX", def1Path[1].name_);
  EXPECT_FALSE(def1Path[1].escaped_);
  EXPECT_EQ(Expression::Type::STRING ,def1Value.value_.index());
  EXPECT_EQ("mem_regfile[7:0]%32%8%SPEED%0%0%MICRO_RAM", def1Value.getString());

  auto def2Path = test->defParameterAssignments_[2].first;
  auto def2Value = test->defParameterAssignments_[2].second;
  EXPECT_EQ(2, def2Path.size());
  EXPECT_EQ("$$ins2@@", def2Path[0].name_);
  EXPECT_TRUE(def2Path[0].escaped_);
  EXPECT_EQ("INIT", def2Path[1].name_);
  EXPECT_FALSE(def2Path[1].escaped_);
  EXPECT_EQ(Expression::Type::NUMBER ,def2Value.value_.index());
  EXPECT_EQ("2'h2", def2Value.getString());
}