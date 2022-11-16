#include "VerilogConstructorTest.h"

#include <iostream>

void VerilogConstructorTest::startModule(std::string&& name) {
  modules_.push_back(Module(name));
  std::cerr << "Construct Module: " << name << std::endl;
}

void VerilogConstructorTest::moduleInterfaceSimplePort(std::string&& name) {
  std::cerr << "Construct Simple Port: " << name << std::endl;
}

void VerilogConstructorTest::moduleInterfaceCompletePort(naja::verilog::Port&& port) {
  std::cerr << "Interface Complete Port: " << port.getString() << std::endl;
  modules_.back().ports_.push_back(port);
}

void VerilogConstructorTest::moduleImplementationPort(naja::verilog::Port&& port) {
  std::cerr << "Implementation Complete Port: " << port.getString() << std::endl;
}

void VerilogConstructorTest::addNet(naja::verilog::Net&& net) {
  std::cerr << "Add net: " << net.getString() << std::endl;
  modules_.back().nets_.push_back(net);
}