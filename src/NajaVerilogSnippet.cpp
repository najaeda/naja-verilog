/*
 * Copyright 2022 The Naja Authors.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "VerilogConstructor.h"

#include <iostream>

namespace {

void commandLine() {
  std::cerr << "naja_verilog_test design.v" << std::endl;
}

class VerilogConstructorExample: public naja::verilog::VerilogConstructor {
  public:
    void startModule(const std::string& name) override {
      std::cout << "Construct Module: " << name << std::endl;
    }
    void moduleInterfaceSimplePort(const std::string& name) override {
      std::cout << "Simple Port: " << name << std::endl;
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
    void startInstantiation(const std::string& modelName) override {
      std::cout << "startInstantiation: " << modelName << std::endl;
    }
    void addInstance(const std::string& instanceName) override {
      std::cout << "addInstance: " << instanceName << std::endl;
    }
    void addInstanceConnection(const std::string& portName, const naja::verilog::Expression& expression) override {
      std::cout << "addInstanceConnection: " << portName << ": " << expression.getString() << std::endl;
    }
    void endInstantiation() override {
      std::cout << "endInstantiation" << std::endl; 
    }
    void addParameterAssignment(const std::string& parameterName, const naja::verilog::Expression& expression) override {
      std::cout << "addParameterAssignment: " << parameterName << ": " <<  expression.getString() << std::endl;
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
