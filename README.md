# naja-verilog
![build](https://github.com/xtofalex/naja-verilog/actions/workflows/build.yml/badge.svg)
[![codecov](https://codecov.io/gh/xtofalex/naja-verilog/branch/main/graph/badge.svg?token=EWV8ZI20EI)](https://codecov.io/gh/xtofalex/naja-verilog)
[![License](https://img.shields.io/badge/License-Apache_2.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)
***
## Introduction
Naja-Verilog is a structural (gate-level) Verilog parser and can be used to read synthesis generated netlists (for instance Yosys or Vivado).

This project primarly purpose is to offer  be used in conjunction with [Naja](), however both projects are not tied and Naja-Verilog can be integrated in any project needing structural verilog support.

### Acknowledgement
[<img src="https://nlnet.nl/logo/banner.png" width=100>](https://nlnet.nl/project/Naja)
[<img src="https://nlnet.nl/image/logos/NGI0Entrust_tag.svg" width=100>](https://nlnet.nl/project/Naja)

This project is supported and funded by NLNet through the [NGI0 Entrust](https://nlnet.nl/entrust) Fund.

### Gate level netlist
To parse complete RTL level verilog or system verilog, please use projects such as: [Verible](https://github.com/chipsalliance/verible)

A comparable project is [Parser-Verilog](https://github.com/OpenTimer/Parser-Verilog). 
## Compilation
### Getting sources
```bash
# First clone the repository and go inside it
git clone https://github.com/xtofalex/naja-verilog.git
cd naja
git submodule init
git submodule update
```
### Dependencies
Mandatory dependencies:
1. cmake: at least 3.22 version.
2. (Bison)[https://www.gnu.org/software/bison/]
3. (Flex)[https://github.com/westes/flex]

Embedded dependencies, through git sub module: (google test)[https://github.com/google/googletest].

## How to create your own parser
Best starting point is to copy existing examples/implementations:
* (NajaVerilogSnippet)[https://github.com/xtofalex/naja-verilog/blob/main/src/NajaVerilogSnippet.cpp]: very simple snippet application verbosely printing visited objects.
* (VerilogConstructorTest)[https://github.com/xtofalex/naja-verilog/blob/main/test/VerilogConstructorTest.h]: Example class used in Naja-verilog unit tests: visit verilog and collect in simple data structures.
* (SNLVRLConstructor)[https://github.com/xtofalex/naja/blob/main/src/snl/formats/verilog/frontend/SNLVRLConstructor.h]: More concrete example showing Naja SNL (C++ gate netlist data structure) construction.

The principle of the parser is straightforward: inherit from () and override methods 

### Two pass Parsing
As ordering of verilog modules in single or across multiple files is not preknown and module interfaces need to be created before instances and connectivity are created, parsing can be done in a two pass way with:
1. Parse modules, ports and parameters. Ignore instances and connectivity. Construct all interfaces.
2. Reparse. Ignore ports and parameters. Parse instances and nets. Construct connectivity.
