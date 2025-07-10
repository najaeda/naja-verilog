// SPDX-FileCopyrightText: 2023 The Naja verilog authors <https://github.com/najaeda/naja-verilog/blob/main/AUTHORS>
//
// SPDX-License-Identifier: Apache-2.0

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
    using Paths = std::vector<std::filesystem::path>;
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
    virtual void startModule(const Identifier& id) {}
    //Simple Port declaration (only name), no range, no direction in module interface
    virtual void moduleInterfaceSimplePort(const Identifier& port) {}
    //Complete Port declaration in module interface
    virtual void moduleInterfaceCompletePort(const Port& port) {}
    virtual void moduleImplementationPort(const Port& port) {}
    virtual void addNet(const Net& net) {}
    virtual void addAssign(const RangeIdentifiers& identifiers, const Expression& expression) {}
    virtual void startInstantiation(const Identifier& model) {}
    virtual void addInstanceConnection(const Identifier& port, const Expression& expression) {}
    virtual void addOrderedInstanceConnection(size_t portIndex, const Expression& expression) {}
    virtual void endInstantiation() {}
    virtual void startGateInstantiation(const GateType& type) {}
    virtual void endGateInstantiation() {}
    virtual void addInstance(const Identifier& instance) {}
    virtual void addParameterAssignment(const Identifier& parameter, const Expression& expression) {}
    virtual void addDefParameterAssignment(
      const Identifiers& hierarchicalParameter,
      const ConstantExpression& expression) {}
    virtual void addAttribute(
      const Identifier& attributeName,
      const ConstantExpression& expression) {}
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
    void internalStartModule(const Identifier& id);
    void internalEndModule();
    void internalModuleInterfaceSimplePort(const Identifier& id);
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
