#include "VerilogConstructorTest.h"
 
#include <cassert>
#include <iostream>

VerilogConstructorTest::~VerilogConstructorTest() {
  for (auto module: modules_) {
    delete module;
  }
}

void VerilogConstructorTest::addModule(Module* module) {
  assert(modulesMap_.find(module->name_) == modulesMap_.end());
  modulesMap_[module->name_] = module;
  modules_.push_back(module);
}

void VerilogConstructorTest::startModule(const std::string& name) {
  if (inFirstPass()) {
    currentModule_ = new Module(name);
    addModule(currentModule_);
    std::cerr << "Construct Module: " << name << std::endl;
  } else {
    if (modulesMap_.find(name) == modulesMap_.end()) {
      std::cerr << "Cannot find Module: " << name << std::endl;
      exit(5);
    }
    currentModule_ = modulesMap_.find(name)->second;
  } 
}

void VerilogConstructorTest::moduleInterfaceSimplePort(const std::string& name) {
  if (inFirstPass()) {
    std::cerr << "Construct Simple: " << name << std::endl;
  }
}

void VerilogConstructorTest::moduleInterfaceCompletePort(const naja::verilog::Port& port) {
  if (inFirstPass()) {
    std::cerr << "Interface Complete: " << port.getString() << std::endl;
    currentModule_->ports_.push_back(std::move(port));
  }
}

void VerilogConstructorTest::moduleImplementationPort(const naja::verilog::Port& port) {
  if (inFirstPass()) {
    std::cerr << "Implementation Complete: " << port.getString() << std::endl;
    currentModule_->ports_.push_back(std::move(port));
  }
}

void VerilogConstructorTest::addNet(const naja::verilog::Net& net) {
  if (not inFirstPass()) {
    std::cerr << "Add net: " << net.getString() << std::endl;
    currentModule_->nets_.push_back(std::move(net));
  }
}

void VerilogConstructorTest::startInstantiation(const std::string& modelName) {
  if (not inFirstPass()) {
    std::cerr << "startInstantiation: " << modelName << std::endl;
    currentModelName_ = modelName;
  }
}

void VerilogConstructorTest::endInstantiation() {
  if (not inFirstPass()) {
    std::cerr << "Finish Instantiation of: " << currentModelName_ << std::endl;
    Instance& instance = currentModule_->instances_.back();
    instance.parameterAssignments_.swap(currentModule_->currentInstanceParameterAssignments_);
    currentModelName_ = std::string();
  }
}

void VerilogConstructorTest::addInstance(const std::string& instanceName) {
  if (not inFirstPass()) {
    std::cerr << "Add instance: " << instanceName << std::endl;
    currentModule_->instances_.push_back(Instance(currentModelName_, instanceName));
  }
}

void VerilogConstructorTest::addInstanceConnection(
  const std::string& portName,
  const naja::verilog::Expression& expression) {
  if (not inFirstPass()) {
    std::cerr << "Add instance connection: "
      << portName << " " << expression.getString() <<  std::endl;
    Instance& instance = currentModule_->instances_.back();
    instance.connections_.push_back(InstanceConnection(portName, expression));
  }
}

void VerilogConstructorTest::addParameterAssignment(
  const std::string& parameterName,
  const naja::verilog::Expression& expression) {
  if (not inFirstPass()) {
    std::cerr << "Add parameter assignment: "
      << parameterName << " " << expression.getString() <<  std::endl;
    currentModule_->currentInstanceParameterAssignments_[parameterName] = expression.getString();
  }
}

void VerilogConstructorTest::addAssign(
  const naja::verilog::Identifiers& identifiers,
  const naja::verilog::Expression& expression) {
  if (not inFirstPass()) {
    std::cerr << "Add assign: ";
    for (auto identifier: identifiers) {
      std::cerr << identifier.getDescription() << std::endl;
    }
    std::cerr << expression.getDescription() << std::endl;
    //currentModule_->assigns_.push_back(
    //  Assign(naja::verilog::Identifier(parameterName), expression)
  }
}