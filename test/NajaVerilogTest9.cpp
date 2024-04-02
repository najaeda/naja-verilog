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

TEST(NajaVerilogTest9, test) {
  VerilogConstructorTest constructor;
  std::filesystem::path test9Path(
      std::filesystem::path(NAJA_VERILOG_BENCHMARKS)
      / std::filesystem::path("benchmarks")
      / std::filesystem::path("test9.v"));

  constructor.setFirstPass(true);
  constructor.parse(test9Path);
  ASSERT_EQ(1, constructor.modules_.size());
  EXPECT_TRUE(constructor.modules_[0]->nets_.empty());
  EXPECT_TRUE(constructor.modules_[0]->assigns_.empty());
  EXPECT_TRUE(constructor.modules_[0]->instances_.empty());
  auto test = constructor.modules_[0];
  const auto& ports = constructor.modules_[0]->ports_;
  ASSERT_EQ(7, constructor.modules_[0]->ports_.size());
  EXPECT_EQ(naja::verilog::Identifier("i1", false), ports[0].identifier_);
  EXPECT_EQ(naja::verilog::Identifier("i2%", true), ports[1].identifier_);
  EXPECT_EQ(naja::verilog::Identifier("i3", false), ports[2].identifier_);
  EXPECT_EQ(naja::verilog::Identifier("o1", false), ports[3].identifier_);
  EXPECT_EQ(naja::verilog::Identifier("o2", false), ports[4].identifier_);
  EXPECT_EQ(naja::verilog::Identifier("io1", false), ports[5].identifier_);
  EXPECT_EQ(naja::verilog::Identifier("io2", false), ports[6].identifier_);

  EXPECT_EQ(naja::verilog::Port::Direction::Input, ports[0].direction_);
  EXPECT_EQ(naja::verilog::Port::Direction::Input, ports[1].direction_);
  EXPECT_EQ(naja::verilog::Port::Direction::Input, ports[2].direction_);
  EXPECT_EQ(naja::verilog::Port::Direction::Output, ports[3].direction_);
  EXPECT_EQ(naja::verilog::Port::Direction::Output, ports[4].direction_);
  EXPECT_EQ(naja::verilog::Port::Direction::InOut, ports[5].direction_);
  EXPECT_EQ(naja::verilog::Port::Direction::InOut, ports[6].direction_);

  EXPECT_FALSE(ports[0].range_.valid_);
  EXPECT_FALSE(ports[1].range_.valid_);
  EXPECT_FALSE(ports[2].range_.valid_);
  EXPECT_TRUE(ports[3].range_.valid_);
  EXPECT_EQ(3, ports[3].range_.msb_);
  EXPECT_EQ(0, ports[3].range_.lsb_);
  EXPECT_FALSE(ports[4].range_.valid_);
  EXPECT_FALSE(ports[5].range_.valid_);
  EXPECT_FALSE(ports[6].range_.valid_);
  
  constructor.setFirstPass(false);
  constructor.parse(test9Path);
  ASSERT_EQ(1, constructor.modules_.size());
  EXPECT_TRUE(constructor.modules_[0]->nets_.empty());
  EXPECT_TRUE(constructor.modules_[0]->assigns_.empty());
  EXPECT_TRUE(constructor.modules_[0]->instances_.empty());
}