#include "VerilogConstructorTest.h"

#include <iostream>

void VerilogConstructorTest::startModule(std::string&& name) {
  modules_.push_back(Module(name));
  std::cerr << "Construct Module: " << name << std::endl;
}

void VerilogConstructorTest::moduleImplementationPort(naja::verilog::Port&& port) {
  std::cerr << "Construct Port: " << port.getString() << std::endl;
}