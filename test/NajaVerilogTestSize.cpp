// SPDX-FileCopyrightText: 2025 The Naja verilog authors <https://github.com/najaeda/naja-verilog/blob/main/AUTHORS>
//
// SPDX-License-Identifier: Apache-2.0

#include "gtest/gtest.h"

#include "VerilogTypes.h"

TEST(NajaVerilogTestSize, testNumbersSize) {
  {
    naja::verilog::BasedNumber basedNumber("8", true, 'b', "10101010");
    EXPECT_EQ("8'sb10101010", basedNumber.getString());
    EXPECT_EQ(8, basedNumber.getSize());
  }

  {
    naja::verilog::Number number("8", true, 'b', "10101010");
    EXPECT_EQ("8'sb10101010", number.getString());
    EXPECT_EQ(8, number.getSize());
  }

  {
    //without size
    naja::verilog::BasedNumber basedNumber(false, 'b', "10101010");
    EXPECT_EQ("'b10101010", basedNumber.getString());
    EXPECT_EQ(8, basedNumber.getSize());
  }

  {
    //octal number
    naja::verilog::BasedNumber basedNumberOctal("6", true, 'o', "52");
    EXPECT_EQ("6'so52", basedNumberOctal.getString());
    EXPECT_EQ(6, basedNumberOctal.getSize());
  }

  {
    //octal number without size
    naja::verilog::BasedNumber basedNumberOctal(false, 'o', "52");
    EXPECT_EQ("'o52", basedNumberOctal.getString());
    EXPECT_EQ(6, basedNumberOctal.getSize());
  }

  {
    naja::verilog::BasedNumber basedNumberNoSize(true, 'h', "FF");
    EXPECT_EQ("'shFF", basedNumberNoSize.getString());
    EXPECT_EQ(8, basedNumberNoSize.getSize());
  }

  {
    naja::verilog::Number number42("42");
    EXPECT_EQ(42, number42.getInt());
    EXPECT_EQ(32, number42.getSize()); // Assuming int is 32 bits
  }

  {
    naja::verilog::Number number(true, 'd', "42");
    EXPECT_EQ("'sd42", number.getString());
    EXPECT_EQ(31, number.getSize()); // Assuming int is 32 bits
  }

  {
    naja::verilog::BasedNumber number(false, 'd', "42");
    EXPECT_EQ("'d42", number.getString());
    EXPECT_EQ(32, number.getSize()); // Assuming int is 32 bits
  }

  {
    naja::verilog::BasedNumber number("16", false, 'd', "8942");
    EXPECT_EQ("16'd8942", number.getString());
    EXPECT_EQ(16, number.getSize()); // Assuming int is 32 bits
  }
}

TEST(NajaVerilogTestSize, testExpressionsSize) {
  {
    naja::verilog::Expression expr;
    expr.value_ = naja::verilog::Number("42");
    expr.valid_ = true;
    EXPECT_TRUE(expr.supported_);
    EXPECT_EQ(naja::verilog::Expression::Type::NUMBER, expr.value_.index());
    EXPECT_EQ(32, expr.getSize());
  }

  {
    naja::verilog::Expression expr;
    expr.value_ = naja::verilog::Number("8", true, 'b', "10101010");
    expr.valid_ = true;
    EXPECT_TRUE(expr.supported_);
    EXPECT_EQ(naja::verilog::Expression::Type::NUMBER, expr.value_.index());
    EXPECT_EQ(8, expr.getSize()); 
  }

  {
    naja::verilog::Expression expr;
    expr.value_ = naja::verilog::RangeIdentifier(naja::verilog::Identifier("my_signal"), naja::verilog::Range(7, 0));
    expr.valid_ = true;
    EXPECT_TRUE(expr.supported_);
    EXPECT_EQ(naja::verilog::Expression::Type::RANGEIDENTIFIER, expr.value_.index());
    EXPECT_EQ(8, expr.getSize()); // Range 7:0
  }
}