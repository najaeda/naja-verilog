<<<<<<< HEAD
// SPDX-FileCopyrightText: 2024 The Naja verilog authors <https://github.com/xtofalex/naja-verilog/blob/main/AUTHORS>
=======
// SPDX-FileCopyrightText: 2024 The Naja verilog authors <https://github.com/najaeda/naja-verilog/blob/main/AUTHORS>
>>>>>>> main
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

TEST(NajaVerilogTest8, test) {
  VerilogConstructorTest constructor;
  std::filesystem::path test8Path(
      std::filesystem::path(NAJA_VERILOG_BENCHMARKS)
      / std::filesystem::path("benchmarks")
      / std::filesystem::path("test8.v"));
<<<<<<< HEAD
  constructor.parse(test8Path);
  ASSERT_EQ(1, constructor.modules_.size());
  auto adder = constructor.modules_[0];
  EXPECT_EQ("adder", adder->name_);

  constructor.setFirstPass(false);
  constructor.parse(test8Path);

#if 0
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
#endif
}
=======

  constructor.setFirstPass(true);
  constructor.parse(test8Path);
  ASSERT_EQ(1, constructor.modules_.size());
  auto test = constructor.modules_[0];
  EXPECT_EQ(naja::verilog::Identifier("mod%", true), test->identifier_);
  EXPECT_TRUE(constructor.modules_[0]->instances_.empty());
  EXPECT_TRUE(constructor.modules_[0]->nets_.empty());
  EXPECT_TRUE(constructor.modules_[0]->assigns_.empty());
  ASSERT_EQ(1, constructor.modules_[0]->ports_.size());
  EXPECT_EQ(naja::verilog::Identifier("asqrt", true), constructor.modules_[0]->ports_[0].identifier_);
  EXPECT_EQ(naja::verilog::Port::Direction::Output, constructor.modules_[0]->ports_[0].direction_);
  EXPECT_TRUE(constructor.modules_[0]->ports_[0].isBus());
  EXPECT_EQ(naja::verilog::Range(40, 0), constructor.modules_[0]->ports_[0].range_);
  
  constructor.setFirstPass(false);
  constructor.parse(test8Path);
  ASSERT_EQ(1, constructor.modules_.size());
  ASSERT_EQ(1, constructor.modules_[0]->instances_.size());
  auto instance = constructor.modules_[0]->instances_[0];
  EXPECT_EQ("ins@2", instance.identifier_.name_);
  EXPECT_EQ("$$MOD", instance.model_.name_);
  EXPECT_TRUE(instance.orderedConnections_.empty());
  ASSERT_EQ(5, instance.connections_.size());
  EXPECT_EQ(naja::verilog::Identifier("I0", false), instance.connections_[0].port_);
  EXPECT_EQ(naja::verilog::Identifier("I1", false), instance.connections_[1].port_);
  EXPECT_EQ(naja::verilog::Identifier("I2", false), instance.connections_[2].port_);
  EXPECT_EQ(naja::verilog::Identifier("I3", false), instance.connections_[3].port_);
  EXPECT_EQ(naja::verilog::Identifier("Q", false), instance.connections_[4].port_);
  EXPECT_EQ(naja::verilog::Expression::Type::RANGEIDENTIFIER, instance.connections_[0].expression_.value_.index());
  auto identifier =
    std::get<naja::verilog::Expression::Type::RANGEIDENTIFIER>(instance.connections_[0].expression_.value_);
  EXPECT_EQ(naja::verilog::Identifier("busa+index", true), identifier.identifier_);
  EXPECT_FALSE(identifier.range_.valid_);
  identifier =
    std::get<naja::verilog::Expression::Type::RANGEIDENTIFIER>(instance.connections_[1].expression_.value_);
  EXPECT_EQ(naja::verilog::Identifier("-clock", true), identifier.identifier_);
  EXPECT_FALSE(identifier.range_.valid_);
  identifier =
    std::get<naja::verilog::Expression::Type::RANGEIDENTIFIER>(instance.connections_[2].expression_.value_);
  EXPECT_EQ(naja::verilog::Identifier("asqrt[33]", true), identifier.identifier_);
  EXPECT_FALSE(identifier.range_.valid_);
  identifier =
    std::get<naja::verilog::Expression::Type::RANGEIDENTIFIER>(instance.connections_[3].expression_.value_);
  EXPECT_EQ(naja::verilog::Identifier("asqrt", true), identifier.identifier_);
  EXPECT_TRUE(identifier.range_.valid_);
  EXPECT_EQ(naja::verilog::Range(33), identifier.range_);
  identifier =
    std::get<naja::verilog::Expression::Type::RANGEIDENTIFIER>(instance.connections_[4].expression_.value_);
  EXPECT_EQ(naja::verilog::Identifier("{a,b}", true), identifier.identifier_);
  EXPECT_FALSE(identifier.range_.valid_);
} 
>>>>>>> main
