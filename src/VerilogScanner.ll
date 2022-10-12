%{

#include "VerilogParser.hpp"

%}

%option noyywrap
%option c++

IDENTIFIER_TK [a-zA-Z_][a-zA-Z0-9_$]*

%%

module      { return MODULE_KW; }
endmodule   { return ENDMODULE_KW; }

 /* Last rule catches everything */
. { yyterminate(); }

%%
