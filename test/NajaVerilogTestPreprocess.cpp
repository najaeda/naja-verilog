// SPDX-FileCopyrightText: 2026 The Naja verilog authors
//
// SPDX-License-Identifier: Apache-2.0

#include "gtest/gtest.h"

#include <filesystem>

#include "VerilogConstructor.h"
#include "VerilogConstructorTest.h"

#ifndef NAJA_VERILOG_BENCHMARKS
#define NAJA_VERILOG_BENCHMARKS "Undefined"
#endif

TEST(NajaVerilogTestPreprocess, includeDefineIfdef) {
  std::filesystem::path testPath(
    std::filesystem::path(NAJA_VERILOG_BENCHMARKS)
    / std::filesystem::path("preprocess_top.v"));
  VerilogConstructorTest constructor;
  constructor.parse(testPath);

  ASSERT_EQ(2, constructor.modules_.size());
  EXPECT_EQ("inc_mod", constructor.modules_[0]->identifier_.name_);
  EXPECT_EQ("top", constructor.modules_[1]->identifier_.name_);
}

TEST(NajaVerilogTestPreprocess, undefActiveAndInactive) {
  std::filesystem::path testPath(
    std::filesystem::path(NAJA_VERILOG_BENCHMARKS)
    / std::filesystem::path("preprocess_undef.v"));
  VerilogConstructorTest constructor;
  constructor.parse(testPath);

  ASSERT_EQ(1, constructor.modules_.size());
  EXPECT_EQ("undef_mod", constructor.modules_[0]->identifier_.name_);
}

TEST(NajaVerilogTestPreprocess, elsifElseBranches) {
  std::filesystem::path testPath(
    std::filesystem::path(NAJA_VERILOG_BENCHMARKS)
    / std::filesystem::path("preprocess_elsif.v"));
  VerilogConstructorTest constructor;
  constructor.parse(testPath);

  ASSERT_EQ(2, constructor.modules_.size());
  EXPECT_EQ("elsif_mod", constructor.modules_[0]->identifier_.name_);
  EXPECT_EQ("else_taken", constructor.modules_[1]->identifier_.name_);
}
