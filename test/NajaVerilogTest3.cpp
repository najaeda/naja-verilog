// SPDX-FileCopyrightText: 2023 The Naja verilog authors <https://github.com/najaeda/naja-verilog/blob/main/AUTHORS>
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

TEST(NajaVerilogTest3, test0) {
  std::filesystem::path test3Path(
      std::filesystem::path(NAJA_VERILOG_BENCHMARKS)
      / std::filesystem::path("benchmarks")
      / std::filesystem::path("test3.v"));
  VerilogConstructorTest constructor;
  {
    constructor.parse(test3Path);

    ASSERT_EQ(3, constructor.modules_.size());
    auto mod0 = constructor.modules_[0];
    EXPECT_EQ("mod0", mod0->identifier_.name_);
    ASSERT_EQ(2, mod0->ports_.size());
    EXPECT_EQ("i0", mod0->ports_[0].identifier_.name_);
    EXPECT_EQ("o0", mod0->ports_[1].identifier_.name_);
    EXPECT_EQ(naja::verilog::Port::Direction::Input, mod0->ports_[0].direction_);
    EXPECT_EQ(naja::verilog::Port::Direction::Output, mod0->ports_[1].direction_);
    EXPECT_TRUE(mod0->nets_.empty());
    EXPECT_TRUE(mod0->instances_.empty());

    auto mod1 = constructor.modules_[1];
    EXPECT_EQ("mod1", mod1->identifier_.name_);
    ASSERT_EQ(2, mod1->ports_.size());
    EXPECT_EQ("i0", mod1->ports_[0].identifier_.name_);
    EXPECT_EQ("o0", mod1->ports_[1].identifier_.name_);
    EXPECT_EQ(naja::verilog::Port::Direction::Input, mod1->ports_[0].direction_);
    EXPECT_EQ(naja::verilog::Port::Direction::Output, mod1->ports_[1].direction_);
    EXPECT_TRUE(mod1->nets_.empty());
    EXPECT_TRUE(mod1->instances_.empty());

    auto test = constructor.modules_[2];
    EXPECT_EQ("test", test->identifier_.name_);
    EXPECT_FALSE(test->identifier_.escaped_);
    ASSERT_EQ(3, test->ports_.size());
    EXPECT_EQ("i", test->ports_[0].identifier_.name_);
    EXPECT_EQ("o", test->ports_[1].identifier_.name_);
    EXPECT_EQ("io", test->ports_[2].identifier_.name_);
    EXPECT_EQ(naja::verilog::Port::Direction::Input, test->ports_[0].direction_);
    EXPECT_EQ(naja::verilog::Port::Direction::Output, test->ports_[1].direction_);
    EXPECT_EQ(naja::verilog::Port::Direction::InOut, test->ports_[2].direction_);
    EXPECT_TRUE(test->nets_.empty());
    EXPECT_TRUE(test->instances_.empty());
  }
  {
    constructor.setFirstPass(false);
    constructor.parse(test3Path);

    EXPECT_EQ(3, constructor.modules_.size());
    auto mod0 = constructor.modules_[0];
    EXPECT_EQ("mod0", mod0->identifier_.name_);
    EXPECT_EQ(2, mod0->ports_.size());
    EXPECT_TRUE(mod0->nets_.empty());
    EXPECT_TRUE(mod0->instances_.empty());

    auto mod1 = constructor.modules_[1];
    EXPECT_EQ("mod1", mod1->identifier_.name_);
    EXPECT_EQ(2, mod1->ports_.size());
    EXPECT_TRUE(mod1->nets_.empty());
    EXPECT_TRUE(mod1->instances_.empty());

    auto test = constructor.modules_[2];
    EXPECT_EQ("test", test->identifier_.name_);
    EXPECT_EQ(3, test->ports_.size());
    ASSERT_EQ(8, test->nets_.size());
    EXPECT_EQ("net0", test->nets_[0].identifier_.name_);
    EXPECT_EQ("net1", test->nets_[1].identifier_.name_);
    EXPECT_EQ("net2", test->nets_[2].identifier_.name_);
    EXPECT_EQ("net3", test->nets_[3].identifier_.name_);
    EXPECT_EQ("net4", test->nets_[4].identifier_.name_);
    EXPECT_EQ("net5", test->nets_[5].identifier_.name_);
    EXPECT_EQ("constant0", test->nets_[6].identifier_.name_);
    EXPECT_EQ("constant1", test->nets_[7].identifier_.name_);
    EXPECT_EQ(naja::verilog::Net::Type::Wire, test->nets_[0].type_);
    EXPECT_EQ(naja::verilog::Net::Type::Wire, test->nets_[1].type_);
    EXPECT_EQ(naja::verilog::Net::Type::Wire, test->nets_[2].type_);
    EXPECT_EQ(naja::verilog::Net::Type::Wire, test->nets_[3].type_);
    EXPECT_EQ(naja::verilog::Net::Type::Wire, test->nets_[4].type_);
    EXPECT_EQ(naja::verilog::Net::Type::Wire, test->nets_[5].type_);
    EXPECT_EQ(naja::verilog::Net::Type::Supply0, test->nets_[6].type_);
    EXPECT_EQ(naja::verilog::Net::Type::Supply1, test->nets_[7].type_);
    EXPECT_FALSE(test->nets_[0].range_.valid_);
    EXPECT_FALSE(test->nets_[1].range_.valid_);
    EXPECT_FALSE(test->nets_[2].range_.valid_);
    EXPECT_FALSE(test->nets_[3].range_.valid_);
    EXPECT_TRUE(test->nets_[4].range_.valid_);
    EXPECT_TRUE(test->nets_[5].range_.valid_);
    EXPECT_FALSE(test->nets_[6].range_.valid_);
    EXPECT_FALSE(test->nets_[7].range_.valid_);
    EXPECT_EQ(31, test->nets_[4].range_.msb_);
    EXPECT_EQ(0, test->nets_[4].range_.lsb_);
    EXPECT_EQ(-2, test->nets_[5].range_.msb_);
    EXPECT_EQ(1, test->nets_[5].range_.lsb_);

    ASSERT_EQ(6, test->instances_.size());
    EXPECT_EQ("mod0", test->instances_[0].model_.name_);
    EXPECT_EQ("inst0", test->instances_[0].identifier_.name_);
    EXPECT_EQ("mod0", test->instances_[1].model_.name_);
    EXPECT_EQ("inst1", test->instances_[1].identifier_.name_);
    EXPECT_EQ("mod1", test->instances_[2].model_.name_);
    EXPECT_EQ("inst2", test->instances_[2].identifier_.name_);
    EXPECT_EQ("mod1", test->instances_[3].model_.name_);
    EXPECT_EQ("inst3", test->instances_[3].identifier_.name_);
    EXPECT_EQ("mod1", test->instances_[4].model_.name_);
    EXPECT_EQ("inst4", test->instances_[4].identifier_.name_);
    EXPECT_EQ("mod1", test->instances_[5].model_.name_);
    EXPECT_EQ("inst5", test->instances_[5].identifier_.name_);

    auto inst0 = test->instances_[0];
    ASSERT_EQ(2, inst0.connections_.size());
    EXPECT_EQ("i0", inst0.connections_[0].port_.name_);
    EXPECT_TRUE(inst0.connections_[0].expression_.valid_);
    EXPECT_EQ(naja::verilog::Expression::Type::RANGEIDENTIFIER,
      inst0.connections_[0].expression_.value_.index());
    auto identifier =
      std::get<naja::verilog::Expression::Type::RANGEIDENTIFIER>(inst0.connections_[0].expression_.value_);
    EXPECT_FALSE(identifier.range_.valid_);
    EXPECT_EQ("net0", identifier.identifier_.name_);
    EXPECT_FALSE(identifier.identifier_.escaped_);
    EXPECT_EQ("o0", inst0.connections_[1].port_.name_);
    EXPECT_FALSE(inst0.connections_[1].expression_.valid_);

    auto inst1 = test->instances_[1];
    ASSERT_EQ(2, inst1.connections_.size());
    EXPECT_EQ("i0", inst1.connections_[0].port_.name_);
    EXPECT_TRUE(inst1.connections_[0].expression_.valid_);
    EXPECT_EQ(naja::verilog::Expression::Type::RANGEIDENTIFIER,
      inst1.connections_[0].expression_.value_.index());
    identifier =
      std::get<naja::verilog::Expression::Type::RANGEIDENTIFIER>(inst1.connections_[0].expression_.value_);
    EXPECT_TRUE(identifier.range_.valid_);
    EXPECT_EQ("net4", identifier.identifier_.name_);
    EXPECT_TRUE(identifier.range_.singleValue_);
    EXPECT_EQ(21, identifier.range_.msb_);

    EXPECT_EQ("o0", inst1.connections_[1].port_.name_);
    EXPECT_TRUE(inst1.connections_[1].expression_.valid_);
    EXPECT_EQ(naja::verilog::Expression::Type::RANGEIDENTIFIER,
      inst1.connections_[1].expression_.value_.index());
    identifier =
      std::get<naja::verilog::Expression::Type::RANGEIDENTIFIER>(inst1.connections_[1].expression_.value_);
    EXPECT_TRUE(identifier.range_.valid_);
    EXPECT_EQ("net4", identifier.identifier_.name_);
    EXPECT_TRUE(identifier.range_.singleValue_);
    EXPECT_EQ(5, identifier.range_.msb_);

    auto inst2 = test->instances_[2];
    ASSERT_EQ(2, inst2.connections_.size());
    EXPECT_EQ("i0", inst2.connections_[0].port_.name_);
    EXPECT_TRUE(inst2.connections_[0].expression_.valid_);
    EXPECT_EQ(naja::verilog::Expression::Type::RANGEIDENTIFIER,
      inst2.connections_[0].expression_.value_.index());
    identifier =
      std::get<naja::verilog::Expression::Type::RANGEIDENTIFIER>(inst2.connections_[0].expression_.value_);
    EXPECT_TRUE(identifier.range_.valid_);
    EXPECT_EQ("net4", identifier.identifier_.name_);
    EXPECT_FALSE(identifier.range_.singleValue_);
    EXPECT_EQ(3, identifier.range_.msb_);
    EXPECT_EQ(6, identifier.range_.lsb_);

    EXPECT_EQ("o0", inst2.connections_[1].port_.name_);
    EXPECT_TRUE(inst2.connections_[1].expression_.valid_);
    EXPECT_EQ(naja::verilog::Expression::Type::RANGEIDENTIFIER,
      inst2.connections_[1].expression_.value_.index());
    identifier =
      std::get<naja::verilog::Expression::Type::RANGEIDENTIFIER>(inst2.connections_[1].expression_.value_);
    EXPECT_FALSE(identifier.range_.valid_);
    EXPECT_EQ("net5", identifier.identifier_.name_);

    auto inst3 = test->instances_[3];
    ASSERT_EQ(2, inst3.connections_.size());
    EXPECT_EQ("i0", inst3.connections_[0].port_.name_);
    EXPECT_TRUE(inst3.connections_[0].expression_.valid_);
    ASSERT_EQ(naja::verilog::Expression::Type::CONCATENATION,
      inst3.connections_[0].expression_.value_.index());
    auto concatenation =
      std::get<naja::verilog::Expression::Type::CONCATENATION>(inst3.connections_[0].expression_.value_);
    ASSERT_EQ(4, concatenation.expressions_.size());
    EXPECT_EQ(naja::verilog::Expression::Type::RANGEIDENTIFIER,
      concatenation.expressions_[0].value_.index());
    identifier =
      std::get<naja::verilog::Expression::Type::RANGEIDENTIFIER>(concatenation.expressions_[0].value_);
    EXPECT_FALSE(identifier.range_.valid_);
    EXPECT_EQ("net0", identifier.identifier_.name_);

    EXPECT_EQ(naja::verilog::Expression::Type::RANGEIDENTIFIER,
      concatenation.expressions_[1].value_.index());
    identifier =
      std::get<naja::verilog::Expression::Type::RANGEIDENTIFIER>(concatenation.expressions_[1].value_);
    EXPECT_FALSE(identifier.range_.valid_);
    EXPECT_EQ("net1", identifier.identifier_.name_);

    
    EXPECT_EQ(naja::verilog::Expression::Type::RANGEIDENTIFIER,
      concatenation.expressions_[2].value_.index());
    identifier =
      std::get<naja::verilog::Expression::Type::RANGEIDENTIFIER>(concatenation.expressions_[2].value_);
    EXPECT_FALSE(identifier.range_.valid_);
    EXPECT_EQ("net2", identifier.identifier_.name_);
    EXPECT_FALSE(identifier.identifier_.escaped_);

    EXPECT_EQ(naja::verilog::Expression::Type::RANGEIDENTIFIER,
      concatenation.expressions_[3].value_.index());
    identifier =
      std::get<naja::verilog::Expression::Type::RANGEIDENTIFIER>(concatenation.expressions_[3].value_);
    EXPECT_TRUE(identifier.range_.valid_);
    EXPECT_EQ("net5", identifier.identifier_.name_);
    EXPECT_TRUE(identifier.range_.singleValue_);
    EXPECT_EQ(-2, identifier.range_.msb_);

    auto inst4 = test->instances_[4];
    ASSERT_EQ(2, inst4.orderedConnections_.size());
    EXPECT_EQ(0, inst4.orderedConnections_[0].portIndex_);
    ASSERT_TRUE(inst4.orderedConnections_[0].expression_.valid_);
    EXPECT_EQ(naja::verilog::Expression::Type::RANGEIDENTIFIER,
      inst4.orderedConnections_[0].expression_.value_.index());
    identifier =
      std::get<naja::verilog::Expression::Type::RANGEIDENTIFIER>(inst4.orderedConnections_[0].expression_.value_);
    EXPECT_TRUE(identifier.range_.valid_);
    EXPECT_EQ("net4", identifier.identifier_.name_);
    EXPECT_FALSE(identifier.range_.singleValue_);
    EXPECT_EQ(7, identifier.range_.msb_);
    EXPECT_EQ(10, identifier.range_.lsb_);

    EXPECT_EQ(1, inst4.orderedConnections_[1].portIndex_);
    ASSERT_TRUE(inst4.orderedConnections_[1].expression_.valid_);
    EXPECT_EQ(naja::verilog::Expression::Type::CONCATENATION,
      inst4.orderedConnections_[1].expression_.value_.index());
    concatenation =
      std::get<naja::verilog::Expression::Type::CONCATENATION>(inst4.orderedConnections_[1].expression_.value_);
    ASSERT_EQ(4, concatenation.expressions_.size());

    auto inst5 = test->instances_[5];
    ASSERT_EQ(1, inst5.orderedConnections_.size());
    EXPECT_EQ(0, inst5.orderedConnections_[0].portIndex_);
    ASSERT_TRUE(inst5.orderedConnections_[0].expression_.valid_);
    EXPECT_EQ(naja::verilog::Expression::Type::RANGEIDENTIFIER,
      inst5.orderedConnections_[0].expression_.value_.index());
    identifier =
      std::get<naja::verilog::Expression::Type::RANGEIDENTIFIER>(inst5.orderedConnections_[0].expression_.value_);
    EXPECT_TRUE(identifier.range_.valid_);
    EXPECT_EQ("net4", identifier.identifier_.name_);
    EXPECT_FALSE(identifier.range_.singleValue_);
    EXPECT_EQ(7, identifier.range_.msb_);
    EXPECT_EQ(10, identifier.range_.lsb_);
  }
}