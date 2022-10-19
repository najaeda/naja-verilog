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

%%
%{          /** Code executed at the beginning of yylex **/
  yylval = lval;
%}

module      { return token::MODULE_KW; }
endmodule   { return token::ENDMODULE_KW; }

\n          {
              // Update line number
              loc->lines();
              return( token::NEWLINE );
            }

 /* Last rule catches everything */
.           { yyterminate(); }

%%
