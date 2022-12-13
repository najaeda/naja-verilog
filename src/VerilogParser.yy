//
// Copyright 2022 The Naja Authors.
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

%skeleton "lalr1.cc"
%require  "3.0"
%define parse.trace
%defines 
%define api.namespace {naja::verilog}
%define api.parser.class {VerilogParser}

%define parse.error verbose

%code requires {

  #include "VerilogTypes.h"
  namespace naja { namespace verilog {
    class VerilogScanner;
    class VerilogConstructor;
  }}

  namespace {
  
    struct NetIdentifier {
      NetIdentifier() = default;
      NetIdentifier(const NetIdentifier&) = default;
      NetIdentifier(std::string&& name): name_(name) {}
      NetIdentifier(std::string&& name, naja::verilog::Range&& range):
        name_(name), range_(range) {}
      std::string           name_   {};
      naja::verilog::Range  range_  {};
    };
  
    using NetIdentifiers = std::vector<NetIdentifier>;
    using Instances = std::vector<std::string>;
  }

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
#include <sstream>

#include "VerilogException.h"

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
%type<std::string> module_identifier;
%type<std::string> name_of_module_instance;
%type<std::string> module_instance_identifier;
%type<std::string> number

%type<naja::verilog::Port> port_declaration
%type<naja::verilog::Port::Direction> port_type_io
%type<naja::verilog::Net::Type> net_type;
%type<naja::verilog::Range> range
%type<naja::verilog::Range> range.opt
%type<NetIdentifier> net_identifier;
%type<NetIdentifiers> list_of_net_identifiers;
%type<std::string> module_instance;
%type<Instances> list_of_module_instances;
%type<std::string> port_identifier;

%locations 
%start source_text

%%

source_text: list_of_descriptions

list_of_descriptions: description | list_of_descriptions description;

description: module_declaration;

identifier: IDENTIFIER_TK | ESCAPED_IDENTIFIER_TK
  { $$ = $1; }
;

range: '[' CONSTVAL_TK ':' CONSTVAL_TK ']' {
  $$.valid_ = true;
  $$.msb_ = std::stoi($2);
  $$.lsb_ = std::stoi($4);
}
;

range.opt: %empty { $$.valid_ = false; } | range { $$ = $1; }

port_declaration: port_type_io range.opt identifier {
  if (constructor->inFirstPass() or constructor->inFullPass()) {
    if ($2.valid_) {
      constructor->moduleInterfaceCompletePort(
        Port(std::move($3), $1, std::move($2))
      );
    } else {
      constructor->moduleInterfaceCompletePort(Port(std::move($3), $1));
    }
  }
}

port_type_io
  : INOUT_KW  { $$ = naja::verilog::Port::Direction::InOut; } 
  | INPUT_KW  { $$ = naja::verilog::Port::Direction::Input; }
  | OUTPUT_KW { $$ = naja::verilog::Port::Direction::Output; }
  ;

list_of_port_declarations: port_declaration
| list_of_port_declarations ',' port_declaration
; 

//optional_comma:
//	',' | %empty;

//list_of_non_port_module_items.opt: %empty | list_of_non_port_module_items;

//list_of_non_port_module_items: non_port_module_item | list_of_non_port_module_items non_port_module_item 

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

net_declaration: net_type list_of_net_identifiers ';' {
  if (not constructor->inFirstPass()) {
    for (auto netIdentifier: $2) {
      constructor->addNet(Net(netIdentifier.name_, netIdentifier.range_, $1));
    }
  }
}

list_of_net_identifiers
: net_identifier {
  if (not constructor->inFirstPass()) {
    $$ = { $1 };
  }
}
| list_of_net_identifiers ',' net_identifier {
  if (not constructor->inFirstPass()) {
    $1.push_back($3);
    $$ = $1;
  }
}

net_identifier: identifier range.opt {
  if (not constructor->inFirstPass()) {
    if ($2.valid_) {
      $$ = NetIdentifier(std::move($1), std::move($2));
    } else {
      $$ = NetIdentifier(std::move($1));
    }
  }
}

net_type
: SUPPLY0_KW { $$ = naja::verilog::Net::Type::Supply0; }
| SUPPLY1_KW { $$ = naja::verilog::Net::Type::Supply1; }
| WIRE_KW    { $$ = naja::verilog::Net::Type::Wire; }
;

list_of_module_instances: module_instance
| list_of_module_instances ',' module_instance;

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

primary
: number
| hierarchical_identifier range_expression.opt
| concatenation
;

expression: primary;

expression.opt: %empty | expression;

ordered_port_connection: expression;

list_of_ordered_port_connections: ordered_port_connection | list_of_ordered_port_connections ',' ordered_port_connection;

port_identifier: identifier;

named_port_connection: '.' port_identifier '(' expression.opt ')' {
  if (not constructor->inFirstPass()) {
    constructor->addInstanceConnection(std::move($2));
  }
}

list_of_named_port_connections: named_port_connection | list_of_named_port_connections ',' named_port_connection;

list_of_port_connections: list_of_ordered_port_connections | list_of_named_port_connections;

list_of_port_connections.opt: %empty | list_of_port_connections;

name_of_module_instance: identifier;

module_instance: name_of_module_instance '(' list_of_port_connections.opt ')' {
  if (not constructor->inFirstPass()) {
    constructor->addInstance(std::move($1));
  }
}

parameter_identifier: identifier;

mintypmax_expression: expression;

mintypmax_expression.opt: %empty | mintypmax_expression;

named_parameter_assignment: '.' parameter_identifier '(' mintypmax_expression.opt ')';

list_of_named_parameter_assignments
: named_parameter_assignment
| list_of_named_parameter_assignments ',' named_parameter_assignment
;

list_of_parameter_assignments: /* list_of_ordered_parameter_assignment | */ list_of_named_parameter_assignments;

parameter_value_assignment: %empty | '#' '(' list_of_parameter_assignments ')'

//(From A.4.1) 
module_instantiation: module_identifier {
  if (not constructor->inFirstPass()) {
    constructor->startInstantiation(std::move($1));
  }
} parameter_value_assignment list_of_module_instances ';' {
  if (not constructor->inFirstPass()) {
    constructor->endInstantiation();
  }
}

module_identifier: identifier;

port: identifier {
  constructor->moduleInterfacePort(std::move($1));
}

list_of_ports: port | list_of_ports ',' port;

module_item
: port_declaration {
    //constructor->moduleImplementationPort(std::move($1));
  } ';'
| non_port_module_item;

list_of_module_items: module_item | list_of_module_items module_item;

list_of_module_items.opt: %empty | list_of_module_items; 

/* A.1.2 */
//module_declaration:
//  MODULE_KW module_identifier {
//    constructor->startModule(std::move($2));
//  } '(' list_of_ports ')' ';' list_of_module_items.opt ENDMODULE_KW {
//    constructor->endModule();
//  }
//| MODULE_KW module_identifier list_of_port_declarations.opt ';' ENDMODULE_KW
//;

module_args.opt: '(' ')' | %empty | '(' list_of_port_declarations ')';

module_declaration:
  MODULE_KW module_identifier {
    constructor->startModule(std::move($2));
  } module_args.opt ';' list_of_module_items.opt ENDMODULE_KW
  {
    constructor->endModule();
  }

%%

void naja::verilog::VerilogParser::error(const location_type& l, const std::string& err_message ) {
  std::ostringstream reason;
  reason << "Parser error: " << err_message  << '\n'
            << "  begin at line " << l.begin.line <<  " col " << l.begin.column  << '\n' 
            << "  end   at line " << l.end.line <<  " col " << l.end.column << "\n";
  throw VerilogException(reason.str());
}
