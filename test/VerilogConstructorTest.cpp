// SPDX-FileCopyrightText: 2023 The Naja verilog authors <https://github.com/najaeda/naja-verilog/blob/main/AUTHORS>
//
// SPDX-License-Identifier: Apache-2.0

#include "VerilogConstructorTest.h"
 
#include <cassert>
#include <iostream>

VerilogConstructorTest::~VerilogConstructorTest() {
  for (auto module: modules_) {
    delete module;
  }
}

void VerilogConstructorTest::addModule(Module* module) {
  assert(modulesMap_.find(module->identifier_.name_) == modulesMap_.end());
  modulesMap_[module->identifier_.name_] = module;
  modules_.push_back(module);
}

void VerilogConstructorTest::startModule(const naja::verilog::Identifier& identifier) {
  if (inFirstPass()) {
    currentModule_ = new Module(identifier);
    addModule(currentModule_);
    //currentModuleAttributes_.clear();
    std::cerr << "Construct Module: " << identifier.getString() << std::endl;
  } else {
    if (auto it = modulesMap_.find(identifier.name_); it != modulesMap_.end()) {
      currentModule_ = it->second;
    } else {
      std::cerr << "Cannot find Module: " << identifier.getString() << std::endl;
      exit(5);
    }
  } 
}

void VerilogConstructorTest::moduleInterfaceSimplePort(const naja::verilog::Identifier& port) {
  if (inFirstPass()) {
    std::cerr << "Construct Simple: " << port.getString() << std::endl;
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
    //net.addAttributes(nextObjectAttributes_);
    currentModule_->nets_.push_back(std::move(net));
  }
  nextObjectAttributes_.clear();
}

void VerilogConstructorTest::startInstantiation(const naja::verilog::Identifier& model) {
  if (not inFirstPass()) {
    std::cerr << "startInstantiation: " << model.getString() << std::endl;
    currentModelName_ = model.name_;
  }
}

void VerilogConstructorTest::endInstantiation() {
  if (not nextObjectAttributes_.empty()) {
    //error
  }
  if (not inFirstPass()) {
    std::cerr << "Finish Instantiation of: " << currentModelName_ << std::endl;
    ModuleInstance* instance =
      dynamic_cast<VerilogConstructorTest::ModuleInstance*>(currentModule_->instances_.back());
    assert(instance);
    instance->parameterAssignments_.swap(currentModule_->currentInstanceParameterAssignments_);
    currentModelName_ = std::string();
  }
}

void VerilogConstructorTest::addInstance(const naja::verilog::Identifier& instanceID) {
  if (not inFirstPass()) {
    Instance* instance = nullptr;
    if (currentGateType_ == naja::verilog::GateType::Unknown) {
      assert(!currentModelName_.empty());
      std::cerr << "Add instance: " << instanceID.getString() << std::endl;
      instance = new ModuleInstance(currentModelName_, instanceID.getString());
    } else {
      assert(currentModelName_.empty());
      std::cerr << "Add gate instance model: " << currentGateType_.getString() << std::endl;
      instance = new GateInstance(currentGateType_, instanceID.getString());
    }
    instance->addAttributes(nextObjectAttributes_);
    currentModule_->instances_.push_back(instance);
  }
  nextObjectAttributes_.clear();
}

void VerilogConstructorTest::addInstanceConnection(
  const naja::verilog::Identifier& port,
  const naja::verilog::Expression& expression) {
  if (not inFirstPass()) {
    std::cerr << "Add instance connection: "
      << port.getString() << " " << expression.getString() <<  std::endl;
    Instance* instance = currentModule_->instances_.back();
    ModuleInstance* moduleInstance = dynamic_cast<ModuleInstance*>(instance);
    assert(moduleInstance);
    moduleInstance->connections_.push_back(InstanceConnection(port, expression));
  }
}

void VerilogConstructorTest::addOrderedInstanceConnection(
  size_t portIndex,
  const naja::verilog::Expression& expression) {
  if (not inFirstPass()) {
    std::cerr << "Add ordered instance connection: "
      << portIndex << " " << expression.getString() <<  std::endl;
    Instance* instance = currentModule_->instances_.back();
    instance->orderedConnections_.push_back(OrderedInstanceConnection(portIndex, expression));
  }
}

void VerilogConstructorTest::startGateInstantiation(
  const naja::verilog::GateType& type) {
  if (not inFirstPass()) {
    std::cerr << "Start Gate Instantiation: " << type.getString() << std::endl;
    currentGateType_ = type;
  }
}

void VerilogConstructorTest::endGateInstantiation() {
  //if (not nextObjectAttributes_.empty()) {
  //  //error
  //}
  if (not inFirstPass()) {
    std::cerr << "Finish Instantiation of: " << currentGateType_.getString() << std::endl;
    currentGateType_ = naja::verilog::GateType::Unknown;
  }
}

void VerilogConstructorTest::addParameterAssignment(
  const naja::verilog::Identifier& parameter,
  const naja::verilog::Expression& expression) {
  if (not inFirstPass()) {
    std::cerr << "Add parameter assignment: "
      << parameter.getString() << " " << expression.getString() <<  std::endl;
    currentModule_->currentInstanceParameterAssignments_[parameter.getString()] = expression.getString();
  }
}

void VerilogConstructorTest::addAssign(
  const naja::verilog::RangeIdentifiers& identifiers,
  const naja::verilog::Expression& expression) {
  if (not inFirstPass()) {
    currentModule_->assigns_.push_back(Assign(identifiers, expression));
  }
}

void VerilogConstructorTest::addDefParameterAssignment(
  const naja::verilog::Identifiers& hierarchicalParameter,
  const naja::verilog::ConstantExpression& expression) {
  if (not inFirstPass()) {
    currentModule_->defParameterAssignments_.push_back(
      VerilogConstructorTest::Module::DefParameterAssignment(hierarchicalParameter, expression));
  }
}

void VerilogConstructorTest::addAttribute(
  const naja::verilog::Identifier& attributeName,
  const naja::verilog::ConstantExpression& expression) {
  nextObjectAttributes_.push_back(naja::verilog::Attribute(attributeName, expression));
}