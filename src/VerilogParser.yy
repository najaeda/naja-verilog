%skeleton "lalr1.cc"

%define parse.trace
%define api.namespace {verilog}
%define api.parser.class {VerilogParser}

%define parse.error verbose

%parse-param { VerilogConstructor* constructor }

%code requires {
  class VerilogConstructor* constructor;
}


%union {
  char* text;
}

%token MODULE_KW
%token ENDMODULE_KW
%token INOUT_KW
%token INPUT_KW
%token OUTPUT_KW

%token <text> IDENTIFIER_TK

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
  constructor_->createModule(std::move($1));
}
;

/* A.1.2 */
module_declaration:
  MODULE_KW module_identifier list_of_port_declarations.opt /* module_item */ ENDMODULE_KW
  ;
