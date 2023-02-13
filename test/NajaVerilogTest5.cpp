#include "gtest/gtest.h"

#include <filesystem>
#include <fstream>

#include "VerilogConstructor.h"

using namespace naja::verilog;

#include "VerilogConstructorTest.h"

#ifndef NAJA_VERILOG_BENCHMARKS
#define NAJA_VERILOG_BENCHMARKS "Undefined"
#endif

TEST(NajaVerilogTest5, test) {
  VerilogConstructorTest constructor;
  std::filesystem::path test5Path(
      std::filesystem::path(NAJA_VERILOG_BENCHMARKS)
      / std::filesystem::path("benchmarks")
      / std::filesystem::path("test5.v"));
  constructor.parse(test5Path);
  ASSERT_EQ(1, constructor.modules_.size());
  auto test = constructor.modules_[0];
  EXPECT_EQ("test", test->name_);

  ASSERT_EQ(1, test->ports_.size());
  EXPECT_EQ("\\asqrt[33] ", test->ports_[0].name_);
  EXPECT_EQ(naja::verilog::Port::Direction::Output, test->ports_[0].direction_);

  EXPECT_TRUE(test->nets_.empty());
  EXPECT_TRUE(test->instances_.empty());

  constructor.setFirstPass(false);
  constructor.parse(test5Path);
  ASSERT_EQ(4, test->nets_.size());
  EXPECT_EQ("\\asqrt[33] ", test->nets_[0].name_);
  EXPECT_EQ("_0746_", test->nets_[1].name_);
  EXPECT_EQ("_0747_", test->nets_[2].name_);
  EXPECT_EQ("_0748_", test->nets_[3].name_);
  EXPECT_EQ(naja::verilog::Net::Type::Wire, test->nets_[0].type_);
  EXPECT_EQ(naja::verilog::Net::Type::Wire, test->nets_[1].type_);
  EXPECT_EQ(naja::verilog::Net::Type::Wire, test->nets_[2].type_);
  EXPECT_EQ(naja::verilog::Net::Type::Wire, test->nets_[3].type_);
  EXPECT_FALSE(test->nets_[0].range_.valid_);
  EXPECT_FALSE(test->nets_[1].range_.valid_);
  EXPECT_FALSE(test->nets_[2].range_.valid_);
  EXPECT_FALSE(test->nets_[3].range_.valid_);

  EXPECT_EQ(1, test->instances_.size());
  const VerilogConstructorTest::Instance& instance = test->instances_[0];
  EXPECT_EQ("_4370_", instance.name_);
  EXPECT_EQ("LUT4", instance.model_);
  EXPECT_EQ(5, instance.connections_.size());
  EXPECT_EQ("I0", instance.connections_[0].port_);
  EXPECT_EQ("I1", instance.connections_[1].port_);
  EXPECT_EQ("I2", instance.connections_[2].port_);
  EXPECT_EQ("I3", instance.connections_[3].port_);
  EXPECT_EQ("Q", instance.connections_[4].port_);
}
