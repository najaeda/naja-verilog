// SPDX-FileCopyrightText: 2026 The Naja verilog authors <https://github.com/najaeda/naja-verilog/blob/main/AUTHORS>
//
// SPDX-License-Identifier: Apache-2.0

#include "gtest/gtest.h"

#include <filesystem>

#include "VerilogConstructor.h"

using namespace naja::verilog;

#include "VerilogConstructorTest.h"

#ifndef NAJA_VERILOG_BENCHMARKS
#define NAJA_VERILOG_BENCHMARKS "Undefined"
#endif

TEST(NajaVerilogTest18, signedNetDeclarations) {
  VerilogConstructorTest constructor;
  std::filesystem::path test18Path(
      std::filesystem::path(NAJA_VERILOG_BENCHMARKS)
      / std::filesystem::path("test18.v"));

  constructor.parse(test18Path);
  ASSERT_EQ(1, constructor.modules_.size());
  auto top = constructor.modules_[0];
  EXPECT_TRUE(top->nets_.empty());

  constructor.setFirstPass(false);
  constructor.parse(test18Path);

  ASSERT_EQ(4, top->nets_.size());

  EXPECT_EQ("a", top->nets_[0].identifier_.name_);
  EXPECT_EQ(Net::Type::Wire, top->nets_[0].type_);
  EXPECT_TRUE(top->nets_[0].signed_);
  EXPECT_TRUE(top->nets_[0].range_.valid_);
  EXPECT_EQ(7, top->nets_[0].range_.msb_);
  EXPECT_EQ(0, top->nets_[0].range_.lsb_);

  EXPECT_EQ("b", top->nets_[1].identifier_.name_);
  EXPECT_EQ(Net::Type::Wire, top->nets_[1].type_);
  EXPECT_FALSE(top->nets_[1].signed_);
  EXPECT_TRUE(top->nets_[1].range_.valid_);
  EXPECT_EQ(7, top->nets_[1].range_.msb_);
  EXPECT_EQ(0, top->nets_[1].range_.lsb_);

  EXPECT_EQ("c", top->nets_[2].identifier_.name_);
  EXPECT_EQ(Net::Type::Wire, top->nets_[2].type_);
  EXPECT_TRUE(top->nets_[2].signed_);
  EXPECT_FALSE(top->nets_[2].range_.valid_);

  EXPECT_EQ("d", top->nets_[3].identifier_.name_);
  EXPECT_EQ(Net::Type::Wire, top->nets_[3].type_);
  EXPECT_FALSE(top->nets_[3].signed_);
  EXPECT_FALSE(top->nets_[3].range_.valid_);
}
