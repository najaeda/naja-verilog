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

TEST(NajaVerilogTest6, test) {
  VerilogConstructorTest constructor;
  std::filesystem::path test6Path(
      std::filesystem::path(NAJA_VERILOG_BENCHMARKS)
      / std::filesystem::path("test6.v"));
  constructor.parse(test6Path);
  ASSERT_EQ(1, constructor.modules_.size());
  auto test = constructor.modules_[0];
  EXPECT_EQ("test", test->identifier_.name_);
  EXPECT_EQ(1, constructor.modules_.size());
  {
    auto mod = constructor.modules_[0];
    EXPECT_TRUE(mod->assigns_.empty());
  }

  constructor.setFirstPass(false);
  constructor.parse(test6Path);
  EXPECT_EQ(1, constructor.modules_.size());
  auto mod = constructor.modules_[0];
  EXPECT_FALSE(mod->assigns_.empty());
  EXPECT_EQ(4, mod->assigns_.size());
  
  {
    auto assign = mod->assigns_[0];
    EXPECT_EQ(1, assign.identifiers_.size());
    auto identifier = assign.identifiers_[0];
    EXPECT_EQ("n0", identifier.identifier_.name_);
    EXPECT_FALSE(identifier.range_.valid_);
    auto expression = assign.expression_;
    EXPECT_TRUE(expression.valid_);
    EXPECT_EQ(naja::verilog::Expression::Type::RANGEIDENTIFIER, expression.value_.index());
    auto leftId = std::get<naja::verilog::Expression::Type::RANGEIDENTIFIER>(expression.value_);
    EXPECT_EQ("n1", leftId.identifier_.name_);
    EXPECT_FALSE(leftId.range_.valid_);
  }

  {
    auto assign = mod->assigns_[1];
    EXPECT_EQ(1, assign.identifiers_.size());
    auto identifier = assign.identifiers_[0];
    EXPECT_EQ("n1", identifier.identifier_.name_);
    EXPECT_FALSE(identifier.range_.valid_);
    auto expression = assign.expression_;
    EXPECT_TRUE(expression.valid_);
    EXPECT_EQ(naja::verilog::Expression::Type::NUMBER, expression.value_.index());
    auto leftId = std::get<naja::verilog::Expression::Type::NUMBER>(expression.value_);
    EXPECT_TRUE(leftId.sign_);
    EXPECT_EQ(naja::verilog::Number::BASED, leftId.value_.index());
    auto leftBaseNumber = std::get<naja::verilog::Number::BASED>(leftId.value_);
    EXPECT_FALSE(leftBaseNumber.signed_);
    EXPECT_EQ(naja::verilog::BasedNumber::BINARY, leftBaseNumber.base_);
    EXPECT_EQ(1, leftBaseNumber.size_);
    EXPECT_EQ("0", leftBaseNumber.digits_);
  }

  {
    auto assign = mod->assigns_[2];
    EXPECT_EQ(2, assign.identifiers_.size());
    auto id0 = assign.identifiers_[0];
    EXPECT_EQ("n2", id0.identifier_.name_);
    EXPECT_TRUE(id0.range_.valid_);
    EXPECT_EQ(3, id0.range_.msb_);
    EXPECT_EQ(2, id0.range_.lsb_);
    auto id1 = assign.identifiers_[1];
    EXPECT_EQ("n2", id1.identifier_.name_);
    EXPECT_TRUE(id1.range_.valid_);
    EXPECT_EQ(1, id1.range_.msb_);
    EXPECT_EQ(0, id1.range_.lsb_);
    auto expression = assign.expression_;
    EXPECT_TRUE(expression.valid_);
    EXPECT_EQ(naja::verilog::Expression::Type::CONCATENATION, expression.value_.index());
    auto concatenation = std::get<naja::verilog::Expression::Type::CONCATENATION>(expression.value_);
    EXPECT_EQ(3, concatenation.expressions_.size());
    EXPECT_TRUE(concatenation.expressions_[2].valid_);
    auto constant = concatenation.expressions_[2];
    EXPECT_EQ(naja::verilog::Expression::NUMBER, constant.value_.index());
    auto constantValue = std::get<naja::verilog::Expression::NUMBER>(constant.value_); 
    EXPECT_TRUE(constantValue.sign_);
    EXPECT_EQ(naja::verilog::Number::BASED, constantValue.value_.index());
    auto constantNumber = std::get<naja::verilog::Number::BASED>(constantValue.value_);
    EXPECT_FALSE(constantNumber.signed_);
    EXPECT_EQ(naja::verilog::BasedNumber::HEX, constantNumber.base_);
    EXPECT_EQ(2, constantNumber.size_);
    EXPECT_EQ("2", constantNumber.digits_);
  }

  {
    auto assign = mod->assigns_[3];
    EXPECT_EQ(1, assign.identifiers_.size());
    auto identifier = assign.identifiers_[0];
    EXPECT_EQ("n3", identifier.identifier_.name_);
    EXPECT_FALSE(identifier.range_.valid_);
    auto expression = assign.expression_;
    EXPECT_TRUE(expression.valid_);
    EXPECT_EQ(naja::verilog::Expression::Type::NUMBER, expression.value_.index());
    auto leftId = std::get<naja::verilog::Expression::Type::NUMBER>(expression.value_);
    EXPECT_TRUE(leftId.sign_);
    EXPECT_EQ(naja::verilog::Number::BASED, leftId.value_.index());
    auto leftBaseNumber = std::get<naja::verilog::Number::BASED>(leftId.value_);
    EXPECT_TRUE(leftBaseNumber.signed_);
    EXPECT_EQ(naja::verilog::BasedNumber::HEX, leftBaseNumber.base_);
    EXPECT_EQ(4, leftBaseNumber.size_);
    EXPECT_EQ("F", leftBaseNumber.digits_);
  }
}