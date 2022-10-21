#include "VerilogConstructorTest.h"

#include <iostream>

void VerilogConstructorTest::createModule(const std::string& name) {
  std::cerr << "Construct Module: " << name << std::endl;
}

void VerilogConstructorTest::createPort(const std::string& name) {
  std::cerr << "Construct Port: " << name << std::endl;
}
