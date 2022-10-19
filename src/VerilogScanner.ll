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
%option nodefault
%option yyclass="naja::verilog::VerilogScanner"
%option noyywrap
%option c++

/* Predefined rules */
NEWLINE "\n"|"\r\n"
SPACE   " "|"\t"|"\f"

IDENTIFIER [_a-zA-Z][$_a-zA-Z0-9]
 
%%
%{          /** Code executed at the beginning of yylex **/
  yylval = lval;
%}

<*>{SPACE}  { /* ignore any space */ }
{NEWLINE}   {
              // Update line number
              loc->lines();
              return( token::NEWLINE );
            }

module      { return token::MODULE_KW; }
endmodule   { return token::ENDMODULE_KW; }

{IDENTIFIER} { return token::IDENTIFIER_TK; }

 /* Last rule catches everything */
.           { yyterminate(); }

%%
