%skeleton "lalr1.cc"
%require  "3.0"
%define parse.trace
%defines 
%define api.namespace {naja::verilog}
%define api.parser.class {VerilogParser}

%define parse.error verbose

%code requires{
  #include "VerilogTypes.h"
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
%token ASSIGN_KW
%token END 0 "end of file"

%token<std::string> IDENTIFIER_TK
%token<std::string> ESCAPED_IDENTIFIER_TK
%token<std::string> CONSTVAL_TK BASE_TK BASED_CONSTVAL_TK

%type<std::string> identifier;
%type<std::string> number

%type<naja::verilog::Port> port_declaration
%type<naja::verilog::Port::Direction> port_type_io

%locations 
%start source_text

%%

source_text: list_of_descriptions

list_of_descriptions: description | list_of_descriptions description;

description: module_declaration;

identifier: IDENTIFIER_TK | ESCAPED_IDENTIFIER_TK
  { $$ = $1; }
;

range: '[' CONSTVAL_TK ':' CONSTVAL_TK ']'; 

range.opt: %empty | range;

port_declaration: port_type_io range.opt identifier
  {
    constructor->createPort(std::move($3));
  }
;

port_type_io
  : INOUT_KW  { $$ = naja::verilog::Port::Direction::Inout; } 
  | INPUT_KW  { $$ = naja::verilog::Port::Direction::Input; }
  | OUTPUT_KW { $$ = naja::verilog::Port::Direction::Inout; }
  ;

list_of_port_declarations: port_declaration
| list_of_port_declarations ',' port_declaration
; 

list_of_port_declarations.opt: '(' ')' | %empty | '(' list_of_port_declarations ')';

//optional_comma:
//	',' | %empty;

list_of_non_port_module_items.opt: %empty | list_of_non_port_module_items;

list_of_non_port_module_items: non_port_module_item | list_of_non_port_module_items non_port_module_item 

non_port_module_item : module_or_generate_item;

hierarchical_net_identifier: identifier;

net_lvalue: 
  hierarchical_net_identifier constant_range_expression.opt;
| '{' list_of_net_lvalues '}';
;

list_of_net_lvalues: net_lvalue | list_of_net_lvalues ',' net_lvalue;

net_assignment: net_lvalue '=' expression;

list_of_net_assignments: net_assignment | list_of_net_assignments ',' net_assignment;

continuous_assign: ASSIGN_KW list_of_net_assignments ';' 

module_or_generate_item: 
  module_or_generate_item_declaration
| module_instantiation
| continuous_assign
; 

module_or_generate_item_declaration: net_declaration;

net_declaration:
  net_type list_of_net_identifiers ';'
| net_type range list_of_net_identifiers ';'
;

list_of_net_identifiers: net_identifier | list_of_net_identifiers ',' net_identifier;

net_identifier: identifier;

net_type: SUPPLY0_KW | SUPPLY1_KW | WIRE_KW;

list_of_module_instances: module_instance | list_of_module_instances ',' module_instance;

number:
  CONSTVAL_TK BASE_TK BASED_CONSTVAL_TK;
| CONSTVAL_TK
;

hierarchical_identifier: identifier;

range_expression:
  expression
| CONSTVAL_TK ':' CONSTVAL_TK
;

range_expression.opt: %empty | '[' range_expression ']';

constant_range_expression.opt: range_expression.opt; 

list_of_expressions: expression | list_of_expressions ',' expression;

concatenation: '{' list_of_expressions '}';

primary:
  number
| hierarchical_identifier range_expression.opt
| concatenation
;

expression: primary;

ordered_port_connection: expression;

list_of_ordered_port_connections: ordered_port_connection | list_of_ordered_port_connections ',' ordered_port_connection;

port_identifier: identifier;

named_port_connection: '.' port_identifier '(' expression ')' ;

list_of_named_port_connections: named_port_connection | list_of_named_port_connections ',' named_port_connection;

list_of_port_connections: list_of_ordered_port_connections | list_of_named_port_connections;

list_of_port_connections.opt: %empty | list_of_port_connections;

module_instance_identifier: identifier;

name_of_module_instance: module_instance_identifier;

module_instance: name_of_module_instance '(' list_of_port_connections.opt ')';

parameter_identifier: identifier;

mintypmax_expression: expression;

mintypmax_expression.opt: %empty | mintypmax_expression;

named_parameter_assignment: '.' parameter_identifier '(' mintypmax_expression.opt ')';

list_of_named_parameter_assignments:
  named_parameter_assignment
| list_of_named_parameter_assignments ',' named_parameter_assignment
;

list_of_parameter_assignments: /* list_of_ordered_parameter_assignment | */ list_of_named_parameter_assignments;

parameter_value_assignment: %empty | '#' '(' list_of_parameter_assignments ')'

//(From A.4.1) 
module_instantiation: module_identifier parameter_value_assignment list_of_module_instances ';'

module_identifier: identifier
{
  constructor->createModule(std::move($1));
}
;

port: identifier;

list_of_ports: port | list_of_ports ',' port;

module_item: port_declaration ';' | non_port_module_item;

list_of_module_items: module_item | list_of_module_items module_item;

list_of_module_items.opt: %empty | list_of_module_items; 

/* A.1.2 */
module_declaration:
  MODULE_KW module_identifier '(' list_of_ports ')' ';' list_of_module_items.opt ENDMODULE_KW
| MODULE_KW module_identifier list_of_port_declarations.opt ';' list_of_non_port_module_items.opt ENDMODULE_KW
  ;

%%


void naja::verilog::VerilogParser::error(const location_type& l, const std::string& err_message ) {
  std::cerr << "Parser error: " << err_message  << '\n'
            << "  begin at line " << l.begin.line <<  " col " << l.begin.column  << '\n' 
            << "  end   at line " << l.end.line <<  " col " << l.end.column << "\n";
  std::abort();
}
