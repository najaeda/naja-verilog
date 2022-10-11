%{

#include "VerilogParser.hpp"

%}

%option noyywrap
%option c++

NAME    [_a-zA-Z][$_a-zA-Z0-9]*{0,1023}

%%

module  { return TK_MODULE; }

 /* Last rule catches everything */
. { yyterminate(); }

%%
