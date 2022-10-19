%skeleton "lalr1.cc"
%require  "3.0"
%debug 
%defines 
%define api.namespace {naja::verilog}
%define api.parser.class {VerilogParser}

%code requires{
  namespace naja { namespace verilog {
    class VerilogScanner;
    class VerilogConstructor;
  }}

// The following definitions is missing when %locations isn't used
# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

}

%parse-param { VerilogScanner& scanner }
%parse-param { VerilogConstructor* constructor }

%code{
  #include <iostream>
  #include <cstdlib>
  #include <fstream>
  
  /* include for all driver functions */
  #include "VerilogConstructor.h"
  #include "VerilogScanner.h"

#undef yylex
#define yylex scanner.yylex
}

%define api.value.type variant
%define parse.assert

%token MODULE_KW
%token ENDMODULE_KW
%token INOUT_KW
%token INPUT_KW
%token OUTPUT_KW
%token NEWLINE
%token END

%token <std::string> IDENTIFIER_TK

%locations 
%start source_text

%%

source_text: list_of_descriptions | /* EMPTY */;

list_of_descriptions: description | list_of_descriptions description;

description: module_declaration;

identifier: IDENTIFIER_TK;

list_of_port_identifiers: identifier 
| identifier ',' list_of_port_identifiers
;

inout_declaration: INOUT_KW list_of_port_identifiers;

input_declaration: INPUT_KW list_of_port_identifiers;

output_declaration: OUTPUT_KW list_of_port_identifiers;

port_declaration: inout_declaration | input_declaration | output_declaration ;

list_of_port_declarations: port_declaration
| list_of_port_declarations ',' port_declaration
; 

list_of_port_declarations.opt: '(' list_of_port_declarations ')';

//module_item: ;

module_identifier: IDENTIFIER_TK
{
  constructor->createModule(std::move($1));
}
;

/* A.1.2 */
module_declaration:
  MODULE_KW module_identifier list_of_port_declarations.opt /* module_item */ ENDMODULE_KW
  ;

//%token               END    0     "end of file"
//%token               UPPER
//%token               LOWER
//%token <std::string> WORD
//%token               NEWLINE
//%token               CHAR
//
//%locations
//
//%%
//
//list_option : END | list END;
//
//list
//  : item
//  | list item
//  ;
//
//item
//  : UPPER   { driver.add_upper(); }
//  | LOWER   { driver.add_lower(); }
//  | WORD    { driver.add_word( $1 ); }
//  | NEWLINE { driver.add_newline(); }
//  | CHAR    { driver.add_char(); }
//  ;

%%


void naja::verilog::VerilogParser::error(const location_type& l, const std::string& err_message ) {
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}
