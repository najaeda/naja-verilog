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

TEST(NajaVerilogTest17, test) {
  VerilogConstructorTest constructor;
  std::filesystem::path test17Path(
      std::filesystem::path(NAJA_VERILOG_BENCHMARKS)
      / std::filesystem::path("benchmarks")
      / std::filesystem::path("test17.v"));
  constructor.parse(test17Path);
  ASSERT_EQ(1, constructor.modules_.size());
  auto test17 = constructor.modules_[0];
  EXPECT_EQ("test17", test17->identifier_.getString());
  EXPECT_TRUE(test17->instances_.empty());

  constructor.setFirstPass(false);
  constructor.parse(test17Path);

  EXPECT_EQ(3, test17->instances_.size());
  //auto and0 = dynamic_cast<VerilogConstructorTest::GateInstance*>(test17->instances_[0]);
  //ASSERT_NE(nullptr, and0);
  //EXPECT_FALSE(and0->identifier_.empty());
  //EXPECT_FALSE(and0->isAnonymous());
  //EXPECT_EQ("and0", and0->identifier_.getString());
  //EXPECT_EQ(naja::verilog::GateType::And, and0->type_);
  //EXPECT_EQ(3, and0->orderedConnections_.size());
  //EXPECT_EQ(0, and0->orderedConnections_[0].portIndex_);
  //EXPECT_EQ(
  //  VerilogConstructorTest::OrderedInstanceConnection::Type::RANGEIDENTIFIERS,
  //  and0->orderedConnections_[0].value_.index()
  //);
  //EXPECT_EQ("a", and0->orderedConnections_[0].expression_.getString());
  //EXPECT_EQ(1, and0->orderedConnections_[1].portIndex_);
  //EXPECT_EQ("b", and0->orderedConnections_[1].expression_.getString());
}