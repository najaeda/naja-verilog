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
  friend class VerilogParser;
  public:
    VerilogConstructor() = default;
    ~VerilogConstructor();
    using Paths = std::list<std::filesystem::path>;
    void parse(const Paths& paths);
    void parse(const std::filesystem::path& path);

    //LCOV_EXCL_START
    std::string getCurrentPath() const { return currentPath_; }
    struct Location {
      std::filesystem::path currentPath_  {};
      unsigned              line_         {0};
      unsigned              column_       {0};

      Location() = delete;
      Location(const Location&) = default;
      Location(
        const std::filesystem::path& currentPath,
        unsigned line,
        unsigned column):
        currentPath_(currentPath),
        line_(line),
        column_(column)
      {}
    };
    Location getCurrentLocation() const;
    //LCOV_EXCL_STOP
    void setCurrentLocation(unsigned line, unsigned column) {
      line_= line; column_ = column;
    }
    
    //LCOV_EXCL_START
    virtual void startModule(const std::string& name) {}
    //Simple Port declaration (only name), no range, no direction in module interface
    virtual void moduleInterfaceSimplePort(const std::string& name) {}
    //Complete Port declaration in module interface
    virtual void moduleInterfaceCompletePort(const Port& port) {}
    virtual void moduleImplementationPort(const Port& port) {}
    virtual void addNet(const Net& net) {}
    virtual void addAssign(const Identifiers& identifiers, const Expression& expression) {}
    virtual void startInstantiation(const std::string& modelName) {}
    virtual void addInstance(const std::string& instanceName) {}
    virtual void addInstanceConnection(const std::string& portName, const Expression& expression) {}
    virtual void addOrderedInstanceConnection(size_t portIndex, const Expression& expression) {}
    virtual void endInstantiation() {}
    virtual void addParameterAssignment(const std::string& parameterName, const Expression& expression) {}
    virtual void endModule() {}
    //LCOV_EXCL_STOP
  private:
    class ModuleInterfaceType {
      public:
        enum ModuleInterfaceTypeEnum {
          Unknown, PortDeclaration, Port
        };  
        ModuleInterfaceType(const ModuleInterfaceTypeEnum& typeEnum);
        ModuleInterfaceType(const ModuleInterfaceType& type) = default;
        operator const ModuleInterfaceTypeEnum&() const {return typeEnum_;}
        std::string getString() const;
        private:
          ModuleInterfaceTypeEnum typeEnum_;
    }; 
    void internalParse(std::istream& stream);
    void internalStartModule(const std::string& name);
    void internalEndModule();
    void internalModuleInterfaceSimplePort(const std::string& name);
    void internalModuleInterfaceCompletePort(const Port& port);
    void internalModuleImplementationPort(const Port& port);

    ModuleInterfaceType type_           {ModuleInterfaceType::Unknown};
    Port::Direction     lastDirection_  {Port::Direction::Unknown};
    Range               lastRange_      {};
    
    VerilogScanner*       scanner_      {nullptr};
    VerilogParser*        parser_       {nullptr};
    std::filesystem::path currentPath_  {};
    unsigned              line_         {0};
    unsigned              column_       {0};
};

}} // namespace verilog // namespace naja

#endif /* __VERILOG_CONSTRUCTOR_H_ */
