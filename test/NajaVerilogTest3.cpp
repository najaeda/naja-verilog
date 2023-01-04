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
  VerilogConstructorTest constructor;
  std::filesystem::path test3Path(
      std::filesystem::path(NAJA_VERILOG_BENCHMARKS)
      / std::filesystem::path("benchmarks")
      / std::filesystem::path("test3.v"));
  constructor.parse(test3Path);
  ASSERT_EQ(2, constructor.modules_.size());
  ASSERT_TRUE(constructor.modules_.find("mod0") != constructor.modules_.end());
  auto mod0 = constructor.modules_.find("mod0")->second;
  EXPECT_EQ("mod0", mod0->name_);
  ASSERT_EQ(2, mod0->ports_.size());
  EXPECT_EQ("i0", mod0->ports_[0].name_);
  EXPECT_EQ("o0", mod0->ports_[1].name_);
  EXPECT_EQ(naja::verilog::Port::Direction::Input, mod0->ports_[0].direction_);
  EXPECT_EQ(naja::verilog::Port::Direction::Output, mod0->ports_[1].direction_);
  EXPECT_TRUE(mod0->nets_.empty());
  EXPECT_TRUE(mod0->instances_.empty());

  ASSERT_TRUE(constructor.modules_.find("test") != constructor.modules_.end());
  auto test = constructor.modules_.find("test")->second;
  EXPECT_EQ("test", test->name_);
  EXPECT_EQ(3, test->ports_.size());
  EXPECT_EQ("i", test->ports_[0].name_);
  EXPECT_EQ("o", test->ports_[1].name_);
  EXPECT_EQ("io", test->ports_[2].name_);
  EXPECT_EQ(naja::verilog::Port::Direction::Input, test->ports_[0].direction_);
  EXPECT_EQ(naja::verilog::Port::Direction::Output, test->ports_[1].direction_);
  EXPECT_EQ(naja::verilog::Port::Direction::InOut, test->ports_[2].direction_);

  constructor.parse(test3Path);

  ASSERT_EQ(7, test->nets_.size());
  EXPECT_EQ("net0", test->nets_[0].name_);
  EXPECT_EQ("net1", test->nets_[1].name_);
  EXPECT_EQ("net2", test->nets_[2].name_);
  EXPECT_EQ("net3", test->nets_[3].name_);
  EXPECT_EQ("net4", test->nets_[4].name_);
  EXPECT_EQ("constant0", test->nets_[5].name_);
  EXPECT_EQ("constant1", test->nets_[6].name_);
  EXPECT_EQ(naja::verilog::Net::Type::Wire, test->nets_[0].type_);
  EXPECT_EQ(naja::verilog::Net::Type::Wire, test->nets_[1].type_);
  EXPECT_EQ(naja::verilog::Net::Type::Wire, test->nets_[2].type_);
  EXPECT_EQ(naja::verilog::Net::Type::Wire, test->nets_[3].type_);
  EXPECT_EQ(naja::verilog::Net::Type::Wire, test->nets_[4].type_);
  EXPECT_EQ(naja::verilog::Net::Type::Supply0, test->nets_[5].type_);
  EXPECT_EQ(naja::verilog::Net::Type::Supply1, test->nets_[6].type_);
  EXPECT_FALSE(test->nets_[0].range_.valid_);
  EXPECT_FALSE(test->nets_[1].range_.valid_);
  EXPECT_FALSE(test->nets_[2].range_.valid_);
  EXPECT_FALSE(test->nets_[3].range_.valid_);
  EXPECT_TRUE(test->nets_[4].range_.valid_);
  EXPECT_FALSE(test->nets_[5].range_.valid_);
  EXPECT_FALSE(test->nets_[6].range_.valid_);
  EXPECT_EQ(31, test->nets_[4].range_.msb_);
  EXPECT_EQ(0, test->nets_[4].range_.lsb_);

  ASSERT_EQ(1, test->instances_.size());
  EXPECT_EQ("mod0", test->instances_[0].model_);
  EXPECT_EQ("inst0", test->instances_[0].name_);
}

TEST(NajaVerilogTest3, test1) {
  std::filesystem::path test3Path(
      std::filesystem::path(NAJA_VERILOG_BENCHMARKS)
      / std::filesystem::path("benchmarks")
      / std::filesystem::path("test3.v"));
  VerilogConstructorTest constructor;
  {
    constructor.parse(test3Path);

    ASSERT_EQ(2, constructor.modules_.size());
    ASSERT_TRUE(constructor.modules_.find("mod0") != constructor.modules_.end());
    auto mod0 = constructor.modules_.find("mod0")->second;
    EXPECT_EQ("mod0", mod0->name_);
    ASSERT_EQ(2, mod0->ports_.size());
    EXPECT_EQ("i0", mod0->ports_[0].name_);
    EXPECT_EQ("o0", mod0->ports_[1].name_);
    EXPECT_EQ(naja::verilog::Port::Direction::Input, mod0->ports_[0].direction_);
    EXPECT_EQ(naja::verilog::Port::Direction::Output, mod0->ports_[1].direction_);
    EXPECT_TRUE(mod0->nets_.empty());
    EXPECT_TRUE(mod0->instances_.empty());

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
    EXPECT_TRUE(test->instances_.empty());
  }
  {
    constructor.parse(test3Path);

    EXPECT_EQ(2, constructor.modules_.size());
    ASSERT_TRUE(constructor.modules_.find("mod0") != constructor.modules_.end());
    auto mod0 = constructor.modules_.find("mod0")->second;
    EXPECT_EQ("mod0", mod0->name_);
    EXPECT_EQ(2, mod0->ports_.size());
    EXPECT_TRUE(mod0->nets_.empty());
    EXPECT_TRUE(mod0->instances_.empty());

    ASSERT_TRUE(constructor.modules_.find("test") != constructor.modules_.end());
    auto test = constructor.modules_.find("test")->second;
    EXPECT_EQ("test", test->name_);
    EXPECT_EQ(3, test->ports_.size());
    ASSERT_EQ(7, test->nets_.size());
    EXPECT_EQ("net0", test->nets_[0].name_);
    EXPECT_EQ("net1", test->nets_[1].name_);
    EXPECT_EQ("net2", test->nets_[2].name_);
    EXPECT_EQ("net3", test->nets_[3].name_);
    EXPECT_EQ("net4", test->nets_[4].name_);
    EXPECT_EQ("constant0", test->nets_[5].name_);
    EXPECT_EQ("constant1", test->nets_[6].name_);
    EXPECT_EQ(naja::verilog::Net::Type::Wire, test->nets_[0].type_);
    EXPECT_EQ(naja::verilog::Net::Type::Wire, test->nets_[1].type_);
    EXPECT_EQ(naja::verilog::Net::Type::Wire, test->nets_[2].type_);
    EXPECT_EQ(naja::verilog::Net::Type::Wire, test->nets_[3].type_);
    EXPECT_EQ(naja::verilog::Net::Type::Wire, test->nets_[4].type_);
    EXPECT_EQ(naja::verilog::Net::Type::Supply0, test->nets_[5].type_);
    EXPECT_EQ(naja::verilog::Net::Type::Supply1, test->nets_[6].type_);
    EXPECT_FALSE(test->nets_[0].range_.valid_);
    EXPECT_FALSE(test->nets_[1].range_.valid_);
    EXPECT_FALSE(test->nets_[2].range_.valid_);
    EXPECT_FALSE(test->nets_[3].range_.valid_);
    EXPECT_TRUE(test->nets_[4].range_.valid_);
    EXPECT_FALSE(test->nets_[5].range_.valid_);
    EXPECT_FALSE(test->nets_[6].range_.valid_);
    EXPECT_EQ(31, test->nets_[4].range_.msb_);
    EXPECT_EQ(0, test->nets_[4].range_.lsb_);

    ASSERT_EQ(1, test->instances_.size());
    EXPECT_EQ("mod0", test->instances_[0].model_);
    EXPECT_EQ("inst0", test->instances_[0].name_);
  }
}