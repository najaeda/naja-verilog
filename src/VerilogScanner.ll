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

%{
/* C++ string header, for string ops below */
#include <string>

/* Implementation of yyFlexScanner */ 
#include "VerilogScanner.h"
#undef  YY_DECL
#define YY_DECL \
  int naja::verilog::VerilogScanner::yylex( \
    naja::verilog::VerilogParser::semantic_type* const lval, \
    naja::verilog::VerilogParser::location_type*loc)

/* typedef to make the returns for the tokens shorter */
using token = naja::verilog::VerilogParser::token;

/* define yyterminate as this instead of NULL */
#define yyterminate() return( token::END )

/* msvc2010 requires that we exclude this header file. */
#define YY_NO_UNISTD_H

/* update location on matching */
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);

%}

%option debug
/* %option nodefault */
%option yyclass="naja::verilog::VerilogScanner"
%option noyywrap
%option nounput
%option never-interactive
%option c++

%x in_comment
%x in_attribute
%x based_const

/* Predefined rules */
NEWLINE       "\n"|"\r\n"
SPACE         " "|"\t"|"\f"
COMMENT_BEGIN "/*"
COMMENT_END   "*/" 
COMMENT_LINE  "//".*\n

ATTRIBUTE_BEGIN "(*"
ATTRIBUTE_END   "*)"

IDENTIFIER [_a-zA-Z][$_a-zA-Z0-9]*
UNSIGNED_NUMBER [0-9][0-9_]*

/* 
  Check the escape rule inside character class 
  https://www.regular-expressions.info/charclass.html
*/
ESCAPED_IDENTIFIER \\[\\^!"#$%&',()*+\-.a-zA-Z0-9/{|}~[\]_:;<=>?@]+[\t\f ]

%%
%{          /** Code executed at the beginning of yylex **/
  yylval = lval;
%}

<*>{SPACE}  { /* ignore any space */ }
{NEWLINE}   {
              // Update line number
              loc->lines();
            }

{COMMENT_LINE}  { loc->lines(); }

{COMMENT_BEGIN} { BEGIN(in_comment); }
<in_comment><<EOF>> {
                       BEGIN(INITIAL);
                       std::cerr << "Unclosed comment at line " << loc->end.line << " col " << loc->end.column << '\n';
                        yyterminate();
                     }
<in_comment>{NEWLINE} { loc->lines(); }
<in_comment>. { /* ignore characters in comment */ }
<in_comment>{COMMENT_END} { BEGIN(INITIAL); }

{ATTRIBUTE_BEGIN} { BEGIN(in_attribute); }
<in_attribute><<EOF>> { 
                      BEGIN(INITIAL);
                      std::cerr << "Unclosed attribute at line " << loc->end.line << " col " << loc->end.column << '\n';
                      yyterminate();
                    }   
<in_attribute>{NEWLINE} { loc->lines(); }
<in_attribute>. { /* ignore characters in comment */ }
<in_attribute>{ATTRIBUTE_END} { BEGIN(INITIAL); }

"."|","|";"|"("|")"|"#"|"["|"]"|":"|"{"|"}"|"=" {
  return yytext[0];
}

module      { return token::MODULE_KW; }
endmodule   { return token::ENDMODULE_KW; }
input       { return token::INPUT_KW; } 
output      { return token::OUTPUT_KW; }
inout       { return token::INOUT_KW; }
wire        { return token::WIRE_KW; }
supply0     { return token::SUPPLY0_KW; }
supply1     { return token::SUPPLY1_KW; }
assign      { return token::ASSIGN_KW; }



{IDENTIFIER}          {  yylval->build<std::string>( yytext ); return token::IDENTIFIER_TK; }
{ESCAPED_IDENTIFIER}  {  yylval->build<std::string>( yytext ); return token::ESCAPED_IDENTIFIER_TK; }

{UNSIGNED_NUMBER} {
	yylval->build<std::string>(yytext);
	return token::CONSTVAL_TK;
}

\'[sS]?[bodhBODH] {
	BEGIN(based_const);
	yylval->build<std::string>(yytext);
	return token::BASE_TK;
}

<based_const>[0-9a-fA-FzxZX?][0-9a-fA-FzxZX?_]* {
	BEGIN(INITIAL);
	yylval->build<std::string>(yytext);
	return token::BASED_CONSTVAL_TK;
}

 /* Last rule catches everything */
.           {
              std::cerr << "Failed to match : " << yytext << '\n';
              yyterminate(); 
            }

%%