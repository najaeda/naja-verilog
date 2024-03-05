// SPDX-FileCopyrightText: 2023 The Naja verilog authors <https://github.com/najaeda/naja-verilog/blob/main/AUTHORS>
//
// SPDX-License-Identifier: Apache-2.0

#include "VerilogConstructor.h"

#include <fstream>
#include "VerilogException.h"
#include "VerilogScanner.h"
#include "VerilogParser.hpp"

namespace naja { namespace verilog {

VerilogConstructor::ModuleInterfaceType::ModuleInterfaceType(const ModuleInterfaceTypeEnum& typeEnum):
  typeEnum_(typeEnum)
{}

//LCOV_EXCL_START
std::string VerilogConstructor::ModuleInterfaceType::getString() const {
  switch (typeEnum_) {
    case ModuleInterfaceType::Unknown: return "Standard";
    case ModuleInterfaceType::PortDeclaration: return "PortDeclaration";
    case ModuleInterfaceType::Port: return "Port";
  }
  return "Error";
}
//LCOV_EXCL_STOP

//LCOV_EXCL_START
VerilogConstructor::Location VerilogConstructor::getCurrentLocation() const {
  return Location(getCurrentPath(), line_, column_);
}
//LCOV_EXCL_STOP

VerilogConstructor::~VerilogConstructor() {
  delete scanner_;
  delete parser_;
}

void VerilogConstructor::parse(const std::filesystem::path& path) {
  if (not std::filesystem::exists(path)) {
    std::string reason(path.string() + " does not exist");
    throw VerilogException(reason);
  }
  currentPath_ = path;
  std::ifstream inFile(path);
  //LCOV_EXCL_START
  if (not inFile.good()) {
    std::string reason(path.string() + " is not a readable file");
    throw VerilogException(reason);
  }
  //LCOV_EXCL_STOP
  internalParse(inFile);
}

void VerilogConstructor::parse(const VerilogConstructor::Paths& paths) {
  for (auto path: paths) {
    parse(path);
  }
}

void VerilogConstructor::internalParse(std::istream &stream) {
  delete(scanner_);
  scanner_ = new VerilogScanner(&stream);
  delete(parser_); 
  parser_ = new VerilogParser(*scanner_, this);
  bool result = parser_->parse();
}

void VerilogConstructor::internalStartModule(const std::string& name) {
  type_ = ModuleInterfaceType::Unknown;
  startModule(name);
}

void VerilogConstructor::internalEndModule() {
  endModule();
}
 
void VerilogConstructor::internalModuleInterfaceSimplePort(const std::string& name) {
  if (type_ == ModuleInterfaceType::Unknown) {
    type_ = ModuleInterfaceType::Port; 
  }
  if (type_ == ModuleInterfaceType::PortDeclaration) {
    moduleInterfaceCompletePort(Port(name, lastDirection_, lastRange_));
  } else {
    moduleInterfaceSimplePort(name);
  }
}
  
void VerilogConstructor::internalModuleInterfaceCompletePort(const Port& port) {
  if (type_ == ModuleInterfaceType::Unknown) {
    type_ = ModuleInterfaceType::PortDeclaration;
  }
  if (type_ == ModuleInterfaceType::Port) {
    //Error
  }
  lastDirection_ = port.direction_;
  lastRange_ = port.range_; 
  moduleInterfaceCompletePort(port);
}

void VerilogConstructor::internalModuleImplementationPort(const Port& port) {
  moduleImplementationPort(port);
}

}} // namespace verilog // namespace naja