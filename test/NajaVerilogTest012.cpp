#include "gtest/gtest.h"

#include <filesystem>
#include <fstream>

#include "VerilogConstructor.h"

using namespace naja::verilog;

#include "VerilogConstructorTest.h"

#ifndef NAJA_VERILOG_BENCHMARKS
#define NAJA_VERILOG_BENCHMARKS "Undefined"
#endif

TEST(NajaVerilogTest012, test0) {
  VerilogConstructorTest constructor;
  std::filesystem::path test0Path(
      std::filesystem::path(NAJA_VERILOG_BENCHMARKS)
      / std::filesystem::path("benchmarks")
      / std::filesystem::path("test0.v"));
  constructor.parse(test0Path);
  ASSERT_EQ(5, constructor.modules_.size());
  auto test0 = constructor.modules_[0];
  EXPECT_EQ("test0", test0->name_);
  EXPECT_TRUE(test0->ports_.empty());
  auto test1 = constructor.modules_[1];
  EXPECT_EQ("test1", test1->name_);
  EXPECT_TRUE(test1->ports_.empty());
  auto test2 = constructor.modules_[2];
  EXPECT_EQ("test2", test2->name_);
  EXPECT_EQ(5, test2->ports_.size());
  EXPECT_EQ("i0", test2->ports_[0].name_);
  EXPECT_EQ(Port::Direction::Input, test2->ports_[0].direction_);
  EXPECT_EQ("i1", test2->ports_[1].name_);
  EXPECT_EQ(Port::Direction::Input, test2->ports_[1].direction_);
  EXPECT_EQ("o0", test2->ports_[2].name_);
  EXPECT_EQ(Port::Direction::Output, test2->ports_[2].direction_);
  EXPECT_EQ("o1", test2->ports_[3].name_);
  EXPECT_EQ(Port::Direction::Output, test2->ports_[3].direction_);
  EXPECT_EQ("io", test2->ports_[4].name_);
  EXPECT_EQ(Port::Direction::InOut, test2->ports_[4].direction_);

  auto test3 = constructor.modules_[3];
  EXPECT_EQ("test3", test3->name_);
  EXPECT_EQ(5, test3->ports_.size());
  EXPECT_EQ("i0", test3->ports_[0].name_);
  EXPECT_EQ(Port::Direction::Input, test3->ports_[0].direction_);
  EXPECT_EQ("i1", test3->ports_[1].name_);
  EXPECT_EQ(Port::Direction::Input, test3->ports_[1].direction_);
  EXPECT_EQ("o0", test3->ports_[2].name_);
  EXPECT_EQ(Port::Direction::Output, test3->ports_[2].direction_);
  EXPECT_EQ("o1", test3->ports_[3].name_);
  EXPECT_EQ(Port::Direction::Output, test3->ports_[3].direction_);
  EXPECT_EQ("io", test3->ports_[4].name_);
  EXPECT_EQ(Port::Direction::InOut, test3->ports_[4].direction_);

  auto test4 = constructor.modules_[4];
  EXPECT_EQ("test4", test4->name_);
  EXPECT_EQ(5, test4->ports_.size());
}

TEST(NajaVerilogTest012, test1) {
  VerilogConstructorTest constructor;
  std::filesystem::path test1Path(
      std::filesystem::path(NAJA_VERILOG_BENCHMARKS)
      / std::filesystem::path("benchmarks")
      / std::filesystem::path("test1.v"));
  constructor.parse(test1Path);
  ASSERT_EQ(1, constructor.modules_.size());
  auto module = constructor.modules_[0];
  EXPECT_EQ("test", module->name_);
}

TEST(NajaVerilogTest012, test2) {
  VerilogConstructorTest constructor;
  std::filesystem::path test1Path(
      std::filesystem::path(NAJA_VERILOG_BENCHMARKS)
      / std::filesystem::path("benchmarks")
      / std::filesystem::path("test2.v"));
  constructor.parse(test1Path);
  ASSERT_EQ(1, constructor.modules_.size());
  auto module = constructor.modules_[0];
  EXPECT_EQ("test", module->name_);
}