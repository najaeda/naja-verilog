// SPDX-FileCopyrightText: 2023 The Naja verilog authors <https://github.com/najaeda/naja-verilog/blob/main/AUTHORS>
//
// SPDX-License-Identifier: Apache-2.0

#include "VerilogConstructor.h"

#include <iostream>
#include <sstream>

namespace {

void commandLine() {
  std::cerr << "naja_verilog_test design.v" << std::endl;
}

class VerilogConstructorExample: public naja::verilog::VerilogConstructor {
  public:
    void startModule(const naja::verilog::Identifier& id) override {
      std::cout << "Construct Module: " << id.getDescription() << std::endl;
    }
    void moduleInterfaceSimplePort(const naja::verilog::Identifier& port) override {
      std::cout << "Simple Port: " << port.getString() << std::endl;
    }
    void moduleInterfaceCompletePort(const naja::verilog::Port& port) override {
      std::cout << "Complete Port: " << port.getString() << std::endl;
    }
    void moduleImplementationPort(const naja::verilog::Port& port) override {
      std::cout << "Construct Port: " << port.getString() << std::endl;
    }
    void addNet(const naja::verilog::Net& net) override {
      std::cout << "Construct Net: " << net.getString() << std::endl;
    }
    void startInstantiation(const naja::verilog::Identifier& model) override {
      std::cout << "startInstantiation: " << model.getString() << std::endl;
    }
    void addInstance(const naja::verilog::Identifier& instance) override {
      std::cout << "addInstance: " << instance.getString() << std::endl;
    }
    void addInstanceConnection(const naja::verilog::Identifier& port, const naja::verilog::Expression& expression) override {
      std::cout << "addInstanceConnection: " << port.getString() << ": " << expression.getString() << std::endl;
    }
    void endInstantiation() override {
      std::cout << "endInstantiation" << std::endl; 
    }
    void addParameterAssignment(const naja::verilog::Identifier& parameter, const naja::verilog::Expression& expression) override {
      std::cout << "addParameterAssignment: " << parameter.getString() << ": " <<  expression.getString() << std::endl;
    }
    virtual void addDefParameterAssignment(
      const naja::verilog::Identifiers& hierarchicalParameter,
      const naja::verilog::ConstantExpression& expression) override {
      std::ostringstream oss;
      for (size_t i = 0; i < hierarchicalParameter.size(); i++) {
        oss << hierarchicalParameter[i].getString();
        if (i < hierarchicalParameter.size() - 1) {
          oss << ".";
        }
      }
      std::cout << "addDefParameterAssignment: "
        << oss.str()
        << ": " << expression.getString() << std::endl;
    }
    void endModule() override {
      std::cout << "endModule" << std::endl; 
    }
};

}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    commandLine();
    exit(1);
  }

  VerilogConstructorExample constructor;
  std::filesystem::path filePath(argv[1]);
  constructor.parse(filePath);
  return 0;
}
