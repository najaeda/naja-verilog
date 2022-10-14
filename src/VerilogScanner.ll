%{

#include "VerilogParser.hpp"
 
using token = verilog::VerilogParser::token;

%}

%option noyywrap
%option c++

IDENTIFIER_TK [a-zA-Z_][a-zA-Z0-9_$]*

%%

module      { return token::MODULE_KW; }
endmodule   { return token::ENDMODULE_KW; }

 /* Last rule catches everything */
. { yyterminate(); }

%%
