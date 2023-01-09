#include "gtest/gtest.h"

#include <filesystem>
#include <fstream>

#include "VerilogConstructor.h"

using namespace naja::verilog;

#include "VerilogConstructorTest.h"

#ifndef NAJA_VERILOG_BENCHMARKS
#define NAJA_VERILOG_BENCHMARKS "Undefined"
#endif

TEST(NajaVerilogTest4, test) {
  VerilogConstructorTest constructor;
  std::filesystem::path test4Path(
      std::filesystem::path(NAJA_VERILOG_BENCHMARKS)
      / std::filesystem::path("benchmarks")
      / std::filesystem::path("test4.v"));
  constructor.parse(test4Path);
  ASSERT_EQ(2, constructor.modules_.size());

  ASSERT_TRUE(constructor.modules_.find("test") != constructor.modules_.end());
  auto test = constructor.modules_.find("test")->second;
  EXPECT_EQ("test", test->name_);
  ASSERT_EQ(3, test->ports_.size());
  EXPECT_EQ("i", test->ports_[0].name_);
  EXPECT_EQ("o", test->ports_[1].name_);
  EXPECT_EQ("io", test->ports_[2].name_);

  EXPECT_EQ(naja::verilog::Port::Direction::Input, test->ports_[0].direction_);
  EXPECT_EQ(naja::verilog::Port::Direction::Output, test->ports_[1].direction_);
  EXPECT_EQ(naja::verilog::Port::Direction::InOut, test->ports_[2].direction_);

  EXPECT_TRUE(test->nets_.empty());

  constructor.setFirstPass(false);
  constructor.parse(test4Path);
  ASSERT_EQ(5, test->nets_.size());
  EXPECT_EQ("_i0_", test->nets_[0].name_);
  EXPECT_EQ("_i1_", test->nets_[1].name_);
  EXPECT_EQ("i", test->nets_[2].name_);
  EXPECT_EQ("o", test->nets_[3].name_);
  EXPECT_EQ("io", test->nets_[4].name_);
  EXPECT_EQ(naja::verilog::Net::Type::Wire, test->nets_[0].type_);
  EXPECT_EQ(naja::verilog::Net::Type::Wire, test->nets_[1].type_);
  EXPECT_EQ(naja::verilog::Net::Type::Wire, test->nets_[2].type_);
  EXPECT_EQ(naja::verilog::Net::Type::Wire, test->nets_[3].type_);
  EXPECT_EQ(naja::verilog::Net::Type::Wire, test->nets_[4].type_);
  EXPECT_FALSE(test->nets_[0].range_.valid_);
  EXPECT_FALSE(test->nets_[1].range_.valid_);
  EXPECT_FALSE(test->nets_[2].range_.valid_);
  EXPECT_TRUE(test->nets_[3].range_.valid_);
  EXPECT_EQ(3, test->nets_[3].range_.msb_);
  EXPECT_EQ(0, test->nets_[3].range_.lsb_);
  EXPECT_FALSE(test->nets_[4].range_.valid_);
}