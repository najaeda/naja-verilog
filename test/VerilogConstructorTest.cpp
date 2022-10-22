#include "VerilogConstructorTest.h"

#include <iostream>

void VerilogConstructorTest::createModule(const std::string& name) {
  std::cerr << "Construct Module: " << name << std::endl;
}

void VerilogConstructorTest::createPort(naja::verilog::Port&& port) {
  std::cerr << "Construct Port: " << port.getString() << std::endl;
}