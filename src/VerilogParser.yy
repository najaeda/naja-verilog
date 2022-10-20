%skeleton "lalr1.cc"
%require  "3.0"
%define parse.trace
%defines 
%define api.namespace {naja::verilog}
%define api.parser.class {VerilogParser}

%define parse.error verbose

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
%token SUPPLY0_KW
%token SUPPLY1_KW
%token WIRE_KW
%token END 0 "end of file"


%token<std::string> IDENTIFIER_TK

%locations 
%start source_text

%%

source_text: list_of_descriptions

list_of_descriptions: description | list_of_descriptions description;

description: module_declaration;

identifier: IDENTIFIER_TK;

port_declaration: port_type_io identifier ;

port_type_io: INOUT_KW | INPUT_KW | OUTPUT_KW;

list_of_port_declarations: port_declaration
| list_of_port_declarations ',' port_declaration
; 

list_of_port_declarations.opt: '(' ')' | %empty | '(' list_of_port_declarations ')';

//optional_comma:
//	',' | %empty;


list_of_non_port_module_items.opt: %empty | list_of_non_port_module_items;

list_of_non_port_module_items: non_port_module_item | list_of_non_port_module_items non_port_module_item 

non_port_module_item : module_or_generate_item;

module_or_generate_item: module_or_generate_item_declaration;

module_or_generate_item_declaration: net_declaration | module_instantiation;

net_declaration: net_type list_of_net_identifiers ';' ;

list_of_net_identifiers: net_identifier | list_of_net_identifiers ',' net_identifier;

net_identifier: IDENTIFIER_TK;

net_type: SUPPLY0_KW | SUPPLY1_KW | WIRE_KW;

list_of_module_instances: module_instance | list_of_module_instances ',' module_instance;

expression: IDENTIFIER_TK;

ordered_port_connection: expression;

list_of_ordered_port_connections: ordered_port_connection | list_of_ordered_port_connections ',' ordered_port_connection;

port_identifier: IDENTIFIER_TK;

named_port_connection: '.' port_identifier '(' ')' ;

list_of_named_port_connections: named_port_connection | list_of_named_port_connections ',' named_port_connection;

list_of_port_connections: list_of_ordered_port_connections | list_of_named_port_connections;

list_of_port_connections.opt: %empty | list_of_port_connections;

module_instance_identifier: IDENTIFIER_TK;

name_of_module_instance: module_instance_identifier;

module_instance: name_of_module_instance '(' list_of_port_connections.opt ')';

//(From A.4.1) 
module_instantiation: module_identifier list_of_module_instances ';'

module_identifier: IDENTIFIER_TK
{
  constructor->createModule(std::move($1));
}
;



/* A.1.2 */
module_declaration:
  MODULE_KW module_identifier list_of_port_declarations.opt ';' list_of_non_port_module_items.opt ENDMODULE_KW
  ;

%%


void naja::verilog::VerilogParser::error(const location_type& l, const std::string& err_message ) {
  std::cerr << "Parser error: " << err_message  << '\n'
            << "  begin at line " << l.begin.line <<  " col " << l.begin.column  << '\n' 
            << "  end   at line " << l.end.line <<  " col " << l.end.column << "\n";
  std::abort();
}
