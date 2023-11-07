# naja-verilog

![build](https://github.com/xtofalex/naja-verilog/actions/workflows/build.yml/badge.svg)
[![codecov](https://codecov.io/gh/xtofalex/naja-verilog/branch/main/graph/badge.svg?token=EWV8ZI20EI)](https://codecov.io/gh/xtofalex/naja-verilog)
[![License](https://img.shields.io/badge/License-Apache_2.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)
***

## Introduction

Naja-Verilog is a structural (gate-level) Verilog parser and can be used to read synthesis generated netlists.

The main purpose of this project is to provide a verilog interface to [Naja SNL](https://github.com/xtofalex/naja), however both projects are not tied and naja-verilog can be integrated in any project needing structural verilog support.

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
git clone https://github.com/xtofalex/naja-verilog.git
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

* [NajaVerilogSnippet](https://github.com/xtofalex/naja-verilog/blob/main/src/NajaVerilogSnippet.cpp): very simple snippet application verbosely printing visited objects.
* [VerilogConstructorTest](https://github.com/xtofalex/naja-verilog/blob/main/test/VerilogConstructorTest.h): Example class used in Naja-verilog unit tests: visit verilog and collect in simple data structures.
* [SNLVRLConstructor](https://github.com/xtofalex/naja/blob/main/src/snl/formats/verilog/frontend/SNLVRLConstructor.h): More concrete example showing Naja SNL (C++ gate netlist data structure) construction.

The principle of the parser is straightforward: inherit from [VerilogConstructor](https://github.com/xtofalex/naja-verilog/blob/main/src/VerilogConstructor.h) and override [callback](#callbacks) methods launched while visiting verilog source.

### Two passes Parsing

As ordering of verilog modules in single or across multiple files is not preknown and module interfaces need to be created before instances and connectivity are created, parsing can be done in a two pass way with:

1. Parse modules, ports and parameters. Ignore instances and connectivity. Construct all interfaces.
2. Reparse. Ignore ports and parameters. Parse instances and nets. Construct connectivity.

<div align="right">[ <a href="#naja-verilog">↑ Back to top ↑</a> ]</div>

## Callbacks

Stuctures (Net, Port, Expression) details constructed by following callbacks can be found in [VerilogType.h](https://github.com/xtofalex/naja-verilog/blob/main/src/VerilogTypes.h) header.

### Module declaration

```c++
void startModule(const std::string& name);
```

This callback is invoked when a module declaration begins. It receives the name of the module as an argument.

For instance, the callback is called with **name="foo"** for the Verilog module declaration below:

```verilog
module foo;
```

***

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
/* module foo(a, b, c); */
input a;
input b;
output c;
```

#### Port complete declaration in Module interface

```c++
void moduleInterfaceCompletePort(const Port& port)
```

Invoked for a complete interface port declaration, detailing port direction (input/output/inout) and size.

```verilog
module foo(input a, output b, inout c);
```

### Callbacks for Nets

```c++
void addNet(const Net& net)
```

This callback is invoked for each net declaration within a module and will construct **Net** structure containing net details: size (for busses) and type: Wire, Supply0 or Supply1;

```verilog
wire n1;
wire n2;
```

#### Assign statements

```c++
void addAssign(const Identifiers& identifiers, const Expression& expression) 
```

is called for each assign statement, facilitating the capture of signal assignments.

```verilog
assign n1 = n2;
assign n2 = n[4:2];
assign n3 = {n4[4], n5[8]};
```

### Callback for instances

#### Starting Instantiation

```c++
void startInstantiation(const std::string& modelName)
```

allows to collect module (used as a model) name for one or multiple instanciations. This method will be collect **modelName=Model** for following declarations: 

```verilog
Model ins();
Model ins0(), ins1(), ins2();
```

#### Adding an Instance

```c++
void addInstance(const std::string& instanceName)
```

will be called 3 times with **instanceName=ins1, ins2, ins3** for following declaration:

```verilog
Model ins(), ins2(), ins3();
```

```c++
void endInstantiation();
```

is called at the conclusion of an instance declaration, it signals that all instances have been processed and allows for post-processing cleanup.

#### Ending Instantiation

### Callbacks for Instance Connections

#### Named Port Connection

```c++
void addInstanceConnection(const std::string& portName, const Expression& expression);
```

This function is called for each named port connection in an instance, capturing the relationship between the port and its connected net or expression.

```verilog
Model ins(.p1(n1), .p2(n2), .p3(n3));
```

#### Ordered Port Connection

```c++
void addOrderedInstanceConnection(size_t portIndex, const Expression& expression);
```

is invoked for each port connection when ports are connected by order rather than by name.

```verilog
Model ins(n1, n2, n3);
```

### Callback for Parameter Assignment

```c++
void addParameterAssignment(const std::string& parameterName, const Expression& expression);
```

```verilog
Model #param instance();
```

<div align="right">[ <a href="#naja-verilog">↑ Back to top ↑</a> ]</div>