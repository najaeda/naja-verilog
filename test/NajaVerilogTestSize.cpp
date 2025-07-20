// SPDX-FileCopyrightText: 2025 The Naja verilog authors <https://github.com/najaeda/naja-verilog/blob/main/AUTHORS>
//
// SPDX-License-Identifier: Apache-2.0

#include "gtest/gtest.h"

#include "VerilogTypes.h"

TEST(NajaVerilogTestSize, test) {
  naja::verilog::BasedNumber basedNumber("8", true, 'b', "10101010");
  EXPECT_EQ("8'sb10101010", basedNumber.getString());
  EXPECT_EQ(8, basedNumber.getSize());

  naja::verilog::BasedNumber basedNumberNoSize(true, 'h', "FF");
  EXPECT_EQ("'shFF", basedNumberNoSize.getString());
  EXPECT_EQ(8, basedNumberNoSize.getSize());

  naja::verilog::Number number("42");
  EXPECT_EQ(42, number.getInt());
  EXPECT_EQ(32, number.getSize()); // Assuming int is 32 bits
}