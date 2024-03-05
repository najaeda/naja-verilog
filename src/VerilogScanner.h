// SPDX-FileCopyrightText: 2023 The Naja verilog authors <https://github.com/najaeda/naja-verilog/blob/main/AUTHORS>
//
// SPDX-License-Identifier: Apache-2.0

#ifndef __VERILOG_SCANNER_H_
#define __VERILOG_SCANNER_H_

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "VerilogParser.hpp"
#include "location.hh"

namespace naja { namespace verilog {

class VerilogScanner: public yyFlexLexer {
  public:
    VerilogScanner(std::istream *in): yyFlexLexer(in) {};
    virtual ~VerilogScanner() {};

    //get rid of override virtual function warning
    using FlexLexer::yylex;

    int yylex( 
      naja::verilog::VerilogParser::semantic_type* const lval, 
      naja::verilog::VerilogParser::location_type* location);
      // YY_DECL defined in VerilogScanner.ll
      // Method body created by flex in mc_lexer.yy.cc

  private:
    /* yyval ptr */
    naja::verilog::VerilogParser::semantic_type* yylval = nullptr;
};

}} // namespace verilog //namespace naja

#endif /* __VERILOG_SCANNER_H_ */