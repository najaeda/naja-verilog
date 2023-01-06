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

#ifndef __VERILOG_CONSTRUCTOR_H_
#define __VERILOG_CONSTRUCTOR_H_

#include <list>
#include <filesystem>
#include <istream>

#include "VerilogTypes.h"

namespace naja { namespace verilog {

class VerilogScanner;
class VerilogParser;
  
class VerilogConstructor {
  public:
    VerilogConstructor() = default;
    ~VerilogConstructor();
    using Paths = std::list<std::filesystem::path>;
    void parse(const Paths& paths);
    void parse(const std::filesystem::path& path);
    virtual void startModule(std::string&& name) {}
    //Simple Port declaration (only name), no range, no direction in module interface
    virtual void moduleInterfaceSimplePort(std::string&& name) {}
    //Complete Port declaration in module interface
    virtual void moduleInterfaceCompletePort(Port&& port) {}
    virtual void moduleImplementationPort(Port&& port) {}
    virtual void addNet(Net&& net) {}
    //virtual void addInstance(Instance&& instance) {}
    virtual void startInstantiation(std::string&& modelName) {}
    virtual void addInstance(std::string&& instanceName) {}
    virtual void addInstanceConnection(std::string&& portName, Expression&& expression) {}
    virtual void endInstantiation() {}
    virtual void endModule() {}
  private:
    void internalParse(std::istream& stream);
    VerilogScanner* scanner_  {nullptr};
    VerilogParser*  parser_   {nullptr};
};

}} // namespace verilog // namespace naja

#endif /* __VERILOG_CONSTRUCTOR_H_ */
