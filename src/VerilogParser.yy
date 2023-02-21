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
//no support for XMRs for the moment
%type<std::string> hierarchical_identifier;
%type<std::string> module_identifier;
%type<std::string> name_of_module_instance;
%type<std::string> parameter_identifier;
//%type<std::string> number

%type<naja::verilog::Port> port_declaration
%type<naja::verilog::Port::Direction> port_type_io
%type<naja::verilog::Net::Type> net_type;
%type<naja::verilog::Range> range;
%type<naja::verilog::Range> range.opt
%type<naja::verilog::Range> range_expression;
%type<naja::verilog::Range> range_expression.opt;
%type<NetIdentifier> net_identifier;
%type<NetIdentifiers> list_of_net_identifiers;
%type<std::string> module_instance;
%type<std::string> port_identifier;

%type<naja::verilog::Number> number;
%type<naja::verilog::Expression> primary;
%type<naja::verilog::Expression> expression;
%type<naja::verilog::Expression> expression.opt;
%type<naja::verilog::Expression> mintypmax_expression;
%type<naja::verilog::Expression> mintypmax_expression.opt;
%type<naja::verilog::Expression::Expressions> concatenation;
%type<naja::verilog::Expression::Expressions> list_of_expressions; 

%locations 
%start source_text

%%

source_text: list_of_descriptions

list_of_descriptions: description | list_of_descriptions description;

description: module_declaration;

identifier: IDENTIFIER_TK | ESCAPED_IDENTIFIER_TK {
  $$ = $1;
}

range: '[' CONSTVAL_TK ':' CONSTVAL_TK ']' {
  $$.valid_ = true;
  $$.msb_ = std::stoi($2);
  $$.lsb_ = std::stoi($4);
}

range.opt: %empty { $$.valid_ = false; } | range { $$ = $1; }

port_declaration: port_type_io range.opt identifier {
  $$ = Port($3, $1, $2);
}

port_type_io
  : INOUT_KW  { $$ = naja::verilog::Port::Direction::InOut; } 
  | INPUT_KW  { $$ = naja::verilog::Port::Direction::Input; }
  | OUTPUT_KW { $$ = naja::verilog::Port::Direction::Output; }
  ;

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

net_declaration: net_type range.opt list_of_net_identifiers ';' {
  for (auto netIdentifier: $3) {
    constructor->addNet(Net(netIdentifier.name_, $2, $1));
  }
}

list_of_net_identifiers
: net_identifier {
  $$ = { $1 };
}
| list_of_net_identifiers ',' net_identifier {
  $1.push_back($3);
  $$ = $1;
}

net_identifier: identifier { $$ = NetIdentifier(std::move($1)); }

net_type
: SUPPLY0_KW { $$ = naja::verilog::Net::Type::Supply0; }
| SUPPLY1_KW { $$ = naja::verilog::Net::Type::Supply1; }
| WIRE_KW    { $$ = naja::verilog::Net::Type::Wire; }
;

list_of_module_instances: module_instance
| list_of_module_instances ',' module_instance;

number 
: CONSTVAL_TK BASE_TK BASED_CONSTVAL_TK {
  $$ = Number($1, $2, $3);
}
| CONSTVAL_TK {
  $$ = Number($1);
}
;

//no support for XMRs for the moment
hierarchical_identifier: identifier;

//only numeric values (one bit) [4] or [4:5] are supported
range_expression: //expression (not supported)
CONSTVAL_TK {
  $$.valid_ = true;
  $$.singleValue_ = true;
  $$.msb_ = std::stoi($1);
}
| CONSTVAL_TK ':' CONSTVAL_TK {
  $$.valid_ = true;
  $$.msb_ = std::stoi($1);
  $$.lsb_ = std::stoi($3);
}

range_expression.opt: %empty { $$.valid_ = false; } | '[' range_expression ']' { $$ = $2; }

constant_range_expression.opt: range_expression.opt; 

list_of_expressions:
expression {
  $$ = { $1 };
}
| list_of_expressions ',' expression {
  $1.push_back($3);
  $$ = $1;
}

concatenation: '{' list_of_expressions '}' { $$ = $2; }

primary
: number {
  $$.valid_ = true; $$.value_ = $1; }
| hierarchical_identifier range_expression.opt { 
  $$.valid_ = true; $$.value_ = naja::verilog::Identifier($1, $2);
}
| concatenation { $$.valid_ = true; $$.supported_ = false; }
;

expression: primary { $$ = $1; }

expression.opt: %empty { $$.valid_ = false; } | expression { $$ = $1; }

ordered_port_connection: expression;

list_of_ordered_port_connections: ordered_port_connection | list_of_ordered_port_connections ',' ordered_port_connection;

port_identifier: identifier;

named_port_connection: '.' port_identifier '(' expression.opt ')' {
  constructor->addInstanceConnection($2, $4);
}

list_of_named_port_connections: named_port_connection | list_of_named_port_connections ',' named_port_connection;

list_of_port_connections: list_of_ordered_port_connections | list_of_named_port_connections;

list_of_port_connections.opt: %empty | list_of_port_connections;

name_of_module_instance: identifier 

module_instance: name_of_module_instance {
  constructor->addInstance(std::move($1));
} '(' list_of_port_connections.opt ')'

parameter_identifier: identifier;

mintypmax_expression: expression;

mintypmax_expression.opt: %empty { $$.valid_ = false; } | mintypmax_expression { $$ = $1; }

named_parameter_assignment: '.' parameter_identifier '(' mintypmax_expression.opt ')' {
  constructor->addParameterAssignment($2, $4);
}

list_of_named_parameter_assignments
: named_parameter_assignment
| list_of_named_parameter_assignments ',' named_parameter_assignment
;

list_of_parameter_assignments: /* list_of_ordered_parameter_assignment | */ list_of_named_parameter_assignments;

parameter_value_assignment: %empty | '#' '(' list_of_parameter_assignments ')'

//(From A.4.1) 
module_instantiation: module_identifier {
  constructor->startInstantiation(std::move($1));
} parameter_value_assignment list_of_module_instances ';' {
  constructor->endInstantiation();
}

module_identifier: identifier 

port: identifier {
  constructor->internalModuleInterfaceSimplePort($1);
}

module_item: port_declaration {
  constructor->internalModuleImplementationPort($1);
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

module_arg
: port_declaration {
  constructor->internalModuleInterfaceCompletePort($1);
}
| port;

list_of_module_args: module_arg | list_of_module_args ',' module_arg;

list_of_module_args.opt: %empty | '(' ')' | '(' list_of_module_args ')';

module_declaration: MODULE_KW module_identifier {
  constructor->internalStartModule(std::move($2));
} list_of_module_args.opt ';' list_of_module_items.opt ENDMODULE_KW {
  constructor->internalEndModule();
}

%%

void naja::verilog::VerilogParser::error(const location_type& l, const std::string& err_message ) {
  std::ostringstream reason;
  reason << "Parser error: " << err_message  << '\n'
            << "  begin at line " << l.begin.line <<  " col " << l.begin.column  << '\n' 
            << "  end   at line " << l.end.line <<  " col " << l.end.column << "\n";
  throw VerilogException(reason.str());
}
