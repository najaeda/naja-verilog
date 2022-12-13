/*
 * Copyright 2022 The Naja Authors.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
