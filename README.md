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

## Parser callbacks

### Module declaration

```c++
void startModule(const std::string& name);
```

is called with **name=foo** for the following declaration:

```verilog
module foo;
endmodule //foo
```

***

```c++
void endModule();
```

is called at the end of a module:

```verilog
endmodule //foo
```

and can be used for cleaning purposes.

### Ports

```c++
void moduleInterfaceSimplePort(const std::string& name)
```

is called when module uses simple declaration interface ports. It will be called 3 times with **name=a, b, c** for the following declaration:

```verilog
module foo(a, b, c);
```

***

```c++
void moduleInterfaceCompletePort(const Port& port)
```

is called for complete interface port declaration:

```verilog
module foo(input a, output b, inout c);
```

```c++
void moduleImplementationPort(const Port& port)
```

is called for associated ports in implementation part:

```verilog
/* module foo(a, b, c); */
input a;
input b;
output c;
```

### Nets

```c++
void addNet(const Net& net)
```

is called

```verilog
wire n1;
wire n2;
```

***

```c++
void addAssign(const Identifiers& identifiers, const Expression& expression) 
```

```verilog
assign n1 = n2;
assign n2 = n[4:2];
assign n3 = {n4[4], n5[8]};
```

### Instances

```c++
void startInstantiation(const std::string& modelName)
```

is called with **modelName=MODEL** for following declaration:

```verilog
MODEL /* ins(); */
```

```c++
void addInstance(const std::string& instanceName)
```

will be called 3 times with **instanceName=ins1, ins2, ins3**

```verilog
/* MODEL */ ins(), ins2(), ins3();
```

```c++
void endInstantiation();
```

is called at the end of an instance declaration and can be used for cleaning purposes.

```c++
void addInstanceConnection(const std::string& portName, const Expression& expression);
```

```c++
void addOrderedInstanceConnection(size_t portIndex, const Expression& expression);
```

***

```c++
void addParameterAssignment(const std::string& parameterName, const Expression& expression);
```

<div align="right">[ <a href="#naja-verilog">↑ Back to top ↑</a> ]</div>

***

## How to create your own parser

Best starting point is to copy existing examples/implementations:

* [NajaVerilogSnippet](https://github.com/xtofalex/naja-verilog/blob/main/src/NajaVerilogSnippet.cpp): very simple snippet application verbosely printing visited objects.
* [VerilogConstructorTest](https://github.com/xtofalex/naja-verilog/blob/main/test/VerilogConstructorTest.h): Example class used in Naja-verilog unit tests: visit verilog and collect in simple data structures.
* [SNLVRLConstructor](https://github.com/xtofalex/naja/blob/main/src/snl/formats/verilog/frontend/SNLVRLConstructor.h): More concrete example showing Naja SNL (C++ gate netlist data structure) construction.

The principle of the parser is straightforward: inherit from [VerilogConstructor](https://github.com/xtofalex/naja-verilog/blob/main/src/VerilogConstructor.h) and override methods launched while visiting verilog source.

### Two passes Parsing

As ordering of verilog modules in single or across multiple files is not preknown and module interfaces need to be created before instances and connectivity are created, parsing can be done in a two pass way with:

1. Parse modules, ports and parameters. Ignore instances and connectivity. Construct all interfaces.
2. Reparse. Ignore ports and parameters. Parse instances and nets. Construct connectivity.

<div align="right">[ <a href="#naja-verilog">↑ Back to top ↑</a> ]</div>