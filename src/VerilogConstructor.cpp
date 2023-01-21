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

VerilogConstructor::~VerilogConstructor() {
  delete scanner_;
  delete parser_;
}

void VerilogConstructor::parse(const std::filesystem::path& path) {
  if (not std::filesystem::exists(path)) {
    std::string reason(path.string() + " does not exist");
    throw VerilogException(reason);
  }
  std::ifstream inFile(path);
  if (not inFile.good()) {
    std::string reason(path.string() + " is not a readable file");
    throw VerilogException(reason);
  }
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
