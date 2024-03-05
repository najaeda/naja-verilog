# naja-verilog

![build](https://github.com/najaeda/naja-verilog/actions/workflows/build.yml/badge.svg)
[![codecov](https://codecov.io/gh/najaeda/naja-verilog/branch/main/graph/badge.svg?token=EWV8ZI20EI)](https://codecov.io/gh/najaeda/naja-verilog)
[![License](https://img.shields.io/badge/License-Apache_2.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)
***

## Introduction

Naja-Verilog is a structural (gate-level) Verilog parser and can be used to read synthesis generated netlists.

This library provides a verilog interface to [Naja SNL](https://github.com/najaeda/naja), however both projects are not tied and naja-verilog can be integrated in any project needing structural verilog support.

:information_desk_person: If you have any questions, bug to report or request, please open an issue or send me a [mail](mailto:christophe.alex@gmail.com).

:star: If you find **Naja-Verilog** interesting, and would like to stay up-to-date, consider starring this repo to help spread the word.

### Acknowledgement

[<img src="https://nlnet.nl/logo/banner.png" width=100>](https://nlnet.nl/project/Naja)
[<img src="https://nlnet.nl/image/logos/NGI0Entrust_tag.svg" width=100>](https://nlnet.nl/project/Naja)

This project is supported and funded by NLNet through the [NGI0 Entrust](https://nlnet.nl/entrust) Fund.

### Gate level netlist

This parser is dedicated to a very small part of [verilog](https://en.wikipedia.org/wiki/Verilog): the subset allowing to describe hierarchical gate level netlists. This is the format found at the output of synthesis tools such as [Yosys](https://github.com/YosysHQ/yosys).

To parse complete RTL level verilog or system verilog, please use projects such as: [Verible](https://github.com/chipsalliance/verible).
Apart the language support, the main difference with such RTL level parsing systems is that naja-verilog does not construct any AST but allows to construct the netlist on the fly while visiting the verilog source. Purpose is to reduce memory footprint and accelerate parsing time.

A comparable project can be found here: [Parser-Verilog](https://github.com/OpenTimer/Parser-Verilog).


<div align="right">[ <a href="#naja-verilog">↑ Back to top ↑</a> ]</div>

***

## Compilation

### Getting sources

```bash
# First clone the repository and go inside it
git clone https://github.com/najaeda/naja-verilog.git
cd naja-verilog
git submodule init
git submodule update
```

### Dependencies

Mandatory dependencies:
1. [cmake](https://cmake.org): at least 3.22 version.
2. [Bison](https://www.gnu.org/software/bison)
3. [Flex](https://github.com/westes/flex)

Embedded dependencies, through git sub module: [google test](https://github.com/google/googletest).

On Ubuntu:

```bash
sudo apt-get install bison
sudo apt-get install flex
```

Using nix-shell:

```bash
nix-shell -p cmake bison flex
```

### Building and Installing

```bash
#First define an env variable that points to the directory where you want naja-verilog to be installed:
export NAJA_INSTALL=<path_to_installation_dir>
# Create a build dir and go inside it
mkdir build
cd build
cmake <path_to_naja_sources_dir> -DCMAKE_INSTALL_PREFIX=$NAJA_INSTALL
#For instance: cmake ~/srcs/naja-verilog -DCMAKE_INSTALL_PREFIX=$NAJA_INSTALL
make
make test
make install
```

<div align="right">[ <a href="#naja-verilog">↑ Back to top ↑</a> ]</div>

***

## Create your own parser

Best starting point is to copy existing examples/implementations:

* [NajaVerilogSnippet](https://github.com/najaeda/naja-verilog/blob/main/src/NajaVerilogSnippet.cpp): very simple snippet application verbosely printing visited objects.
* [VerilogConstructorTest](https://github.com/najaeda/naja-verilog/blob/main/test/VerilogConstructorTest.h): Example class used in Naja-verilog unit tests: visit verilog and collect in simple data structures.
* [Unit tests](https://github.com/najaeda/naja-verilog/blob/main/test): covers trough unit testing most of the parser aspects.
* [SNLVRLConstructor](https://github.com/najaeda/naja/blob/main/src/snl/formats/verilog/frontend/SNLVRLConstructor.h): More concrete example showing Naja SNL (C++ gate netlist data structure) construction.

The principle of the parser is straightforward: inherit from [VerilogConstructor](https://github.com/najaeda/naja-verilog/blob/main/src/VerilogConstructor.h) and override [callback](#callbacks) methods launched while visiting verilog source.

### Two passes Parsing

As ordering of verilog modules in single or across multiple files is not preknown and module interfaces need to be created before instances and connectivity are created, parsing can be done in a two pass way with:

1. Parse modules, ports and parameters. Ignore instances and connectivity. Construct all interfaces.
2. Reparse. Ignore ports and parameters. Parse instances and nets. Construct connectivity.

<div align="right">[ <a href="#naja-verilog">↑ Back to top ↑</a> ]</div>

## Callbacks

Stuctures (Net, Port, Expression) details constructed by following callbacks can be found in [VerilogType.h](https://github.com/najaeda/naja-verilog/blob/main/src/VerilogTypes.h) header.

### Callbacks for Module

#### Starting a Module

```c++
void startModule(const std::string& name);
```

This callback is invoked when a module declaration begins. It receives the name of the module as an argument.

For instance, the callback is called with **name="foo"** for the Verilog module declaration below:

```verilog
module foo;
```

#### Ending a Module

```c++
void endModule();
```

This function is called upon reaching the end of a module declaration. It serves as a notifier for the end of the module's scope, allowing for any necessary cleanup.

The corresponding verilog end declaration is:

```verilog
endmodule //foo
```

### Callbacks for Ports

#### Simple Port Declaration in Module Interface

```c++
void moduleInterfaceSimplePort(const std::string& name)
```

Triggered when a module uses a simple declaration (only the port name) for ports. It will be called multiple times depending on the number of ports declared.

For example, it is called with **name="a", name="b", and name="c"** for:

```verilog
module foo(a, b, c);
```

#### Port details in module implementation

Associated to previous method, following method collects port details: direction and size in module implementation section.

```c++
void moduleImplementationPort(const Port& port)
```

is called for each port listed in the implementation section of a module.

```verilog
module foo(a, b, c);
input a;
output [3:0] b;
inout c;
//will invoke 3 times moduleImplementationPort with:
//Port name_=a, direction=Input, isBus()=true, range_.msb_=3, range_.lsb_=0
//Port name_=b, direction=Output, isBus()=true, range_.msb_=0, range_.lsb_=3
//Port name_=c, direction=InOut, isBus()=false
```

#### Port complete declaration in Module interface

```c++
void moduleInterfaceCompletePort(const Port& port)
```

Invoked for a complete interface port declaration, detailing port direction (input/output/inout) and in case of bus: msb and lsb.

```verilog
module foo(input[3:0] a, output[0:3] b, inout c);
//will invoke 3 times moduleInterfaceCompletePort with:
//Port name_=a, direction=Input, isBus()=true, range_.msb_=3, range_.lsb_=0
//Port name_=b, direction=Output, isBus()=true, range_.msb_=0, range_.lsb_=3
//Port name_=c, direction=InOut, isBus()=false
```

### Callbacks for Nets

```c++
void addNet(const Net& net)
```

This callback is invoked for each net declaration within a module and will construct **Net** structure containing net details: msb, lsb (for busses) and type: Wire, Supply0 or Supply1.

```verilog
wire net0, net1, net2;  // constructs 3 Net(s) named net0, net1, net2, type_=Wire, isBus()=false
wire [31:0] net3;       // construct 1 Net named net3, type_=Wire, isBus()=true, range_.msb_=31, range_.lsb_=0 
wire [-2:1] net4;       // construct 1 Net named net4, type_=Wire, isBus()=true, range_.msb_=-2, range_.lsb_=1 
supply0 constant0;      // construct 1 Net named constant0, type_=Supply0, isBus()=false
supply1 constant1;      // construct 1 Net named constant1, type_=Supply1, isBus()=false
```

#### Assign statements

```c++
void addAssign(const Identifiers& identifiers, const Expression& expression) 
```

is called for each assign statement, facilitating the capture of signal assignments.

Below are Verilog examples followed by pseudo C++ representations of the data structures that might be constructed by this callback.

```verilog
assign n0 = n1;
//identifiers = { {name_=n0, range_.valid_=false} }
//expressions = 
// {
//   { 
//     value_.index()=naja::verilog::Expression::Type::IDENTIFIER
//     with auto id=std::get<naja::verilog::Expression::Type::IDENTIFIER>(value_)
//     id.name_="n1", id.range_.valid_=false
//   } 
// }
```

```verilog
assign n1 = 1'b0;
//identifiers = { {name_=n1, range_.valid_=false} }
//expressions = 
// {
//   { 
//     value_.index()=naja::verilog::Expression::Type::NUMBER
//     with auto nb=std::get<naja::verilog::Expression::Type::NUMBER>(value_)
//     nb.base_=naja::verilog::BasedNumber::BINARY
//     nb.sign_=true, nb.signed_=false, nb.size_=1, nb.digits_="0"
//   } 
// }
```

```verilog
assign { n2[3:2], n2[1:0] } = { n0, n1, 2'h2 };
//identifiers =
// {
//    { name_=n2, range_.valid_=true, range_.msb_=3, range_.lsb=3 },
//    { name_=n2, range_.valid_=true, range_.msb_=1, range_.lsb=0 },
// }
//expressions = 
// {
//   { 
//     value_.index()=naja::verilog::Expression::Type::CONCATENATION
//     with auto concat=std::get<naja::verilog::Expression::Type::CONCATENATION>(value_)
//     concat[0] is an Identifier name_=n0, range_.valid_=false
//     concat[1] is an Identifier name_=n1, range_.valid_=false
//     concat[2] is an NUMBER with:
//     nb.base_=naja::verilog::BasedNumber::HEX
//     nb.size_=2, nb.digits_="2"
//   } 
// }
```

### Callback for instances

#### Starting Instantiation

```c++
void startInstantiation(const std::string& modelName)
```

allows to collect module (used as a model) name for one or multiple instanciations. This method will collect `modelName=Model` for the two following declarations:

```verilog
Model ins();
Model ins0(), ins1(), ins2();
```

#### Adding an Instance

```c++
void addInstance(const std::string& instanceName)
```

will be called 3 times with `instanceName=ins1, ins2, ins3` for following declaration:

```verilog
Model ins(), ins2(), ins3();
```

#### Ending Instantiation

```c++
void endInstantiation();
```

is called at the conclusion of an instance declaration, it signals that all instances have been processed and allows for post-processing cleanup.

### Callbacks for Instance Connections

#### Named Port Connection

```c++
void addInstanceConnection(const std::string& portName, const Expression& expression);
```

This function is called for each named port connection in an instance, capturing the relationship between the port and its connected net or expression.

```verilog
mod1 inst2(.i0(net4[3:6]), .o0(net5));
//addInstanceConnection is called 2 times with:
//portName=i0
//expression_.value_.index() = naja::verilog::Expression::Type::IDENTIFIER,
//with auto id = std::get<naja::verilog::Expression::Type::IDENTIFIER>(expression.value_)
//id.name_="net4", id.isBus=true, id.range_.msb_=3, is.range_.lsb_=6
//and:
//portName=o0
//expression_.value_.index() = naja::verilog::Expression::Type::IDENTIFIER,
//with auto id = std::get<naja::verilog::Expression::Type::IDENTIFIER>(expression.value_)
//id.name_="net5", id.isBus=false
```

#### Ordered Port Connection

```c++
void addOrderedInstanceConnection(size_t portIndex, const Expression& expression);
```

is invoked for each port connection when ports are connected by order rather than by name.

```verilog
mod1 inst4(net4[7:10], {net0, net1, net2, net3});
//addOrderedInstanceConnection is called 2 times with:
//portIndex=0
//expression_.value_.index() = naja::verilog::Expression::Type::IDENTIFIER,
//with auto id = std::get<naja::verilog::Expression::Type::IDENTIFIER>(expression.value_)
//id.name_="net4", id.isBus=true, id.range_.msb_=7, is.range_.lsb_=10
//and:
//portIndex=1
//expression_.value_.index() = naja::verilog::Expression::Type::CONCATENATION,
//with auto concat = std::get<naja::verilog::Expression::Type::CONCATENATION>(expression.value_)
//concat.size()=4 and all values are simple Identifiers
```

### Callback for Parameter Assignment

```c++
void addParameterAssignment(const std::string& parameterName, const Expression& expression);
```

This callback function is designed to handle parameter assignments within module instantiations.

```verilog
module test();
  mod #(
    .PARAM0("VAL"),
  ) ins();
endmodule
//addParameterAssignment is called one time with:
//parameterName=PARAM0 and expression is an Identifier with name="VAL"
```

<div align="right">[ <a href="#naja-verilog">↑ Back to top ↑</a> ]</div>
