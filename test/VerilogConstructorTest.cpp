#include "VerilogConstructorTest.h"
 
#include <cassert>
#include <iostream>

VerilogConstructorTest::~VerilogConstructorTest() {
  for (auto it: modules_) {
    delete it.second;
  }
}

void VerilogConstructorTest::addModule(Module* module) {
  assert(modules_.find(module->name_) == modules_.end());
  modules_[module->name_] = module;
}

void VerilogConstructorTest::startModule(std::string&& name) {
  if (inFirstPass()) {
    currentModule_ = new Module(name);
    addModule(currentModule_);
    std::cerr << "Construct Module: " << name << std::endl;
  } else {
    if (modules_.find(name) == modules_.end()) {
      std::cerr << "Cannot find Module: " << name << std::endl;
      exit(5);
    }
    currentModule_ = modules_.find(name)->second;
  } 
}

void VerilogConstructorTest::moduleInterfaceSimplePort(std::string&& name) {
  if (inFirstPass()) {
    std::cerr << "Construct Simple Port: " << name << std::endl;
  }
}

void VerilogConstructorTest::moduleInterfaceCompletePort(const naja::verilog::Port& port) {
  if (inFirstPass()) {
    std::cerr << "Interface Complete Port: " << port.getString() << std::endl;
    currentModule_->ports_.push_back(std::move(port));
  }
}

void VerilogConstructorTest::moduleImplementationPort(const naja::verilog::Port& port) {
  if (inFirstPass()) {
    std::cerr << "Implementation Complete Port: " << port.getString() << std::endl;
    currentModule_->ports_.push_back(std::move(port));
  }
}

void VerilogConstructorTest::addNet(naja::verilog::Net&& net) {
  if (not inFirstPass()) {
    std::cerr << "Add net: " << net.getString() << std::endl;
    currentModule_->nets_.push_back(std::move(net));
  }
}

void VerilogConstructorTest::startInstantiation(std::string&& modelName) {
  if (not inFirstPass()) {
    std::cerr << "startInstantiation: " << modelName << std::endl;
    currentModelName_ = modelName;
  }
}

void VerilogConstructorTest::endInstantiation() {
  if (not inFirstPass()) {
    std::cerr << "Finish Instantiation of: " << currentModelName_ << std::endl;
    currentModelName_ = std::string();
  }
}

void VerilogConstructorTest::addInstance(std::string&& instanceName) {
  if (not inFirstPass()) {
    std::cerr << "Add instance: " << instanceName << std::endl;
    currentModule_->instances_.push_back(Instance(currentModelName_, instanceName));
  }
}

void VerilogConstructorTest::addInstanceConnection(
  std::string&& portName,
  naja::verilog::Expression&& expression) {
  if (not inFirstPass()) {
    std::cerr << "Add instance connection: "
      << portName << " " << expression.getString() <<  std::endl;
  }
}
