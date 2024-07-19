// SPDX-FileCopyrightText: 2023 The Naja verilog authors <https://github.com/najaeda/naja-verilog/blob/main/AUTHORS>
//
// SPDX-License-Identifier: Apache-2.0

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

size_t portIndex = 0;

static naja::verilog::Number generateNumber(
  bool hasSize,
  const std::string& size,
  const std::string& base,
  const std::string& digits,
  int beginLine,
  int beginColumn,
  int endLine,
  int endColumn) {
  if (base.size() == 2) {
    //LCOV_EXCL_START
    if (not (base[0] == 's' || base[0] == 'S')) {
      //Following should not happen as long as lexer is correct
      //should this be replaced by an assertion ?
      std::ostringstream reason;
      reason << "Parser error: ";
      if (hasSize) {
        reason << "\'" << size << base << digits;
      } else {
        reason << "\'" << base << digits; 
      }
      reason << " is not a valid number: wrong signed character.\n"
        << "  begin at line " << beginLine <<  " col " << beginColumn  << '\n' 
        << "  end   at line " << endLine <<  " col " << endColumn << "\n";
      throw naja::verilog::VerilogException(reason.str());
    }
    //LCOV_EXCL_STOP
    if (hasSize) {
      return naja::verilog::Number(size, true, base[1], digits);
    } else {
      return naja::verilog::Number(true, base[1], digits);
    }
  } else if (base.size() == 1) {
    if (hasSize) {
      return naja::verilog::Number(size, false, base[0], digits);
    } else {
      return naja::verilog::Number(false, base[0], digits);
    }
  } else {
    //LCOV_EXCL_START
    //Same as previously: should not be accessible, as this is filtered by lexer.
    std::ostringstream reason;
    reason << "Parser error: ";
    if (hasSize) {
        reason << "\'" << size << base << digits;
    } else {
        reason << "\'" << base << digits; 
    }
    reason << " is not a valid number\n"
      << "  begin at line " << beginLine <<  " col " << beginColumn  << '\n' 
      << "  end   at line " << endLine <<  " col " << endColumn << "\n";
    throw naja::verilog::VerilogException(reason.str());
    //LCOV_EXCL_STOP
  }
}

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
%token<std::string> STRING_TK
%token<std::string> CONSTVAL_TK BASE_TK BASED_CONSTVAL_TK
%token<std::string> SIGN_TK

%type<naja::verilog::Identifier> identifier;
//no support for XMRs for the moment
%type<naja::verilog::Identifier> hierarchical_identifier;
%type<naja::verilog::Identifier> hierarchical_net_identifier;
%type<naja::verilog::Identifier> module_identifier;
%type<naja::verilog::Identifier> name_of_module_instance;
%type<naja::verilog::Identifier> parameter_identifier;

%type<naja::verilog::Port> port_declaration
%type<naja::verilog::Ports> internal_ports_declaration
%type<naja::verilog::Port::Direction> port_type_io
%type<naja::verilog::Net::Type> net_type;
%type<naja::verilog::Range> range;
%type<naja::verilog::Range> range.opt
%type<naja::verilog::Range> constant_range_expression.opt;
%type<naja::verilog::Identifier> net_identifier;
%type<naja::verilog::Identifiers> list_of_identifiers;
%type<naja::verilog::RangeIdentifiers> net_lvalue;
%type<naja::verilog::RangeIdentifiers> list_of_net_lvalues;
%type<naja::verilog::Identifier> module_instance;
%type<naja::verilog::Identifier> port_identifier;

%type<naja::verilog::Number> number;
%type<naja::verilog::Number> constant_primary;
%type<naja::verilog::Number> constant_expression;
%type<std::string> unary_operator;
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

identifier
  : IDENTIFIER_TK { $$ = naja::verilog::Identifier($1); }
  | ESCAPED_IDENTIFIER_TK {
    std::string escaped = $1.substr(1, $1.size()-2);
    $$ = naja::verilog::Identifier(escaped, true);
  }

constant_primary: number

unary_operator: SIGN_TK; 

constant_expression: constant_primary { 
  $$ = $1;
} 
| unary_operator constant_primary {
  $$ = $2;
  if ($1 == "-") { $$.sign_ = false; }
}

range: '[' constant_expression ':' constant_expression ']' {
  $$ = Range($2.getInt(), $4.getInt());
}

range.opt: %empty { $$.valid_ = false; } | range { $$ = $1; }

port_declaration: port_type_io range.opt identifier {
  $$ = Port($3, $1, $2);
}

internal_ports_declaration: port_type_io range.opt list_of_identifiers {
  constructor->setCurrentLocation(@$.begin.line, @$.begin.column);
  for (auto portIdentifier: $3) {
    constructor->internalModuleImplementationPort(Port(portIdentifier, $1, $2));
  }
}

port_type_io
  : INOUT_KW  { $$ = naja::verilog::Port::Direction::InOut; } 
  | INPUT_KW  { $$ = naja::verilog::Port::Direction::Input; }
  | OUTPUT_KW { $$ = naja::verilog::Port::Direction::Output; }
  ;

non_port_module_item : module_or_generate_item;

hierarchical_net_identifier: identifier;

net_lvalue: hierarchical_net_identifier constant_range_expression.opt {
  $$ = { naja::verilog::RangeIdentifier($1, $2) };
}
| '{' list_of_net_lvalues '}' {
  $$ = $2;
}

//Only one level of list is supported
list_of_net_lvalues: hierarchical_net_identifier constant_range_expression.opt {
  $$ = { naja::verilog::RangeIdentifier($1, $2) };
}
| list_of_net_lvalues ',' hierarchical_net_identifier constant_range_expression.opt {
  $1.push_back(naja::verilog::RangeIdentifier($3, $4));
  $$ = $1;
}

net_assignment: net_lvalue '=' expression {
  constructor->setCurrentLocation(@$.begin.line, @$.begin.column);
  constructor->addAssign($1, $3);
}

list_of_net_assignments: net_assignment | list_of_net_assignments ',' net_assignment;

continuous_assign: ASSIGN_KW list_of_net_assignments ';' 

module_or_generate_item: 
  module_or_generate_item_declaration
| module_instantiation
| continuous_assign
; 

module_or_generate_item_declaration: net_declaration;

net_declaration: net_type range.opt list_of_identifiers ';' {
  for (auto netIdentifier: $3) {
    constructor->setCurrentLocation(@$.begin.line, @$.begin.column);
    constructor->addNet(Net(netIdentifier, $2, $1));
  }
}

list_of_identifiers
: net_identifier {
  $$ = { $1 };
}
| list_of_identifiers ',' net_identifier {
  $1.push_back($3);
  $$ = $1;
}

net_identifier: identifier { $$ = naja::verilog::Identifier($1); }

net_type
: SUPPLY0_KW { $$ = naja::verilog::Net::Type::Supply0; }
| SUPPLY1_KW { $$ = naja::verilog::Net::Type::Supply1; }
| WIRE_KW    { $$ = naja::verilog::Net::Type::Wire; }
;

list_of_module_instances
: module_instance
| list_of_module_instances ',' module_instance
;

number 
: BASE_TK BASED_CONSTVAL_TK {
  $$ = generateNumber(false, "", $1, $2, @$.begin.line, @$.end.line, @$.begin.column, @$.end.column);
} 
| CONSTVAL_TK BASE_TK BASED_CONSTVAL_TK {
  $$ = generateNumber(true, $1, $2, $3, @$.begin.line, @$.end.line, @$.begin.column, @$.end.column);
}
| CONSTVAL_TK {
  $$ = Number($1);
}

//no support for XMRs for the moment
hierarchical_identifier: identifier;

//only numeric values (one bit) [4] or [4:5] are supported
constant_range_expression.opt: %empty { $$.valid_ = false; } 
| '[' constant_expression ']' {
  $$ = Range($2.getInt());
}
| range {
  $$ = $1;
}

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
| hierarchical_identifier constant_range_expression.opt { 
  $$.valid_ = true; $$.value_ = naja::verilog::RangeIdentifier($1, $2); }
| STRING_TK { $$.valid_ = true; $$.value_ = $1.substr(1, $1.size()-2); } 
| concatenation { $$.valid_ = true; $$.value_ = naja::verilog::Concatenation($1); }
;

expression: primary { $$ = $1; }

expression.opt: %empty { $$.valid_ = false; } | expression { $$ = $1; }

ordered_port_connection: expression {
  constructor->setCurrentLocation(@$.begin.line, @$.begin.column);
  constructor->addOrderedInstanceConnection(portIndex++, $1);
}

list_of_ordered_port_connections
: ordered_port_connection
| list_of_ordered_port_connections ',' ordered_port_connection
;

port_identifier: identifier;

named_port_connection: '.' port_identifier '(' expression.opt ')' {
  constructor->setCurrentLocation(@$.begin.line, @$.begin.column);
  constructor->addInstanceConnection($2, $4);
}

list_of_named_port_connections: named_port_connection | list_of_named_port_connections ',' named_port_connection;

list_of_port_connections
: { portIndex = 0; } list_of_ordered_port_connections 
| list_of_named_port_connections
;

list_of_port_connections.opt: %empty | list_of_port_connections;

name_of_module_instance: identifier 

module_instance: name_of_module_instance {
  constructor->setCurrentLocation(@$.begin.line, @$.begin.column);
  constructor->addInstance(std::move($1));
} '(' list_of_port_connections.opt ')'

parameter_identifier: identifier;

mintypmax_expression: expression;

mintypmax_expression.opt: %empty { $$.valid_ = false; } | mintypmax_expression { $$ = $1; }

named_parameter_assignment: '.' parameter_identifier '(' mintypmax_expression.opt ')' {
  constructor->setCurrentLocation(@$.begin.line, @$.begin.column);
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
  constructor->setCurrentLocation(@$.begin.line, @$.begin.column);
  constructor->startInstantiation(std::move($1));
} parameter_value_assignment list_of_module_instances ';' {
  constructor->setCurrentLocation(@$.begin.line, @$.begin.column);
  constructor->endInstantiation();
}

module_identifier: identifier 

port: identifier {
  constructor->setCurrentLocation(@$.begin.line, @$.begin.column);
  constructor->internalModuleInterfaceSimplePort($1);
}

module_item: internal_ports_declaration ';' | non_port_module_item;

list_of_module_items: module_item | list_of_module_items module_item;

list_of_module_items.opt: %empty | list_of_module_items; 

/* A.1.2 */
module_arg
: port_declaration {
  constructor->setCurrentLocation(@$.begin.line, @$.begin.column);
  constructor->internalModuleInterfaceCompletePort($1);
}
| port;

list_of_module_args: module_arg | list_of_module_args ',' module_arg;

list_of_module_args.opt: %empty | '(' ')' | '(' list_of_module_args ')';

module_declaration: MODULE_KW module_identifier {
  constructor->setCurrentLocation(@$.begin.line, @$.begin.column);
  constructor->internalStartModule(std::move($2));
} list_of_module_args.opt ';' list_of_module_items.opt ENDMODULE_KW {
  constructor->setCurrentLocation(@$.begin.line, @$.begin.column);
  constructor->internalEndModule();
}

%%

void naja::verilog::VerilogParser::error(
  const location_type& l,
  const std::string& err_message ) {
  std::ostringstream reason;
  reason << "Parser error: " << err_message  << '\n'
            << "  begin at line " << l.begin.line <<  " col " << l.begin.column  << '\n' 
            << "  end   at line " << l.end.line <<  " col " << l.end.column << "\n";
  throw VerilogException(reason.str());
}