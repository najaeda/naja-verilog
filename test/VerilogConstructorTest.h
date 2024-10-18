// SPDX-FileCopyrightText: 2023 The Naja verilog authors <https://github.com/najaeda/naja-verilog/blob/main/AUTHORS>
//
// SPDX-License-Identifier: Apache-2.0

#ifndef __VERILOG_CONSTRUCTOR_TEST_H_
#define __VERILOG_CONSTRUCTOR_TEST_H_

#include "VerilogConstructor.h"

#include <sstream>
#include <map>

class VerilogConstructorTest: public naja::verilog::VerilogConstructor {
  public:
    ~VerilogConstructorTest();
    bool inFirstPass() const { return firstPass_; }
    void setFirstPass(bool mode) { firstPass_ = mode; }
    void startModule(const naja::verilog::Identifier& id) override;
    void moduleInterfaceSimplePort(const naja::verilog::Identifier& id) override;
    void moduleImplementationPort(const naja::verilog::Port& port) override;
    void moduleInterfaceCompletePort(const naja::verilog::Port& port) override;
    void startInstantiation(const naja::verilog::Identifier& model) override;
    void addInstance(const naja::verilog::Identifier& name) override;
    void addInstanceConnection(
      const naja::verilog::Identifier& port,
      const naja::verilog::Expression& expression) override;
    void addOrderedInstanceConnection(
      size_t portIndex,
      const naja::verilog::Expression& expression) override;
    void addParameterAssignment(
      const naja::verilog::Identifier& parameter,
      const naja::verilog::Expression& expression) override;
    void endInstantiation() override;
    void addNet(const naja::verilog::Net& net) override;
    void addAssign(
      const naja::verilog::RangeIdentifiers& identifiers,
      const naja::verilog::Expression& expression) override;
    virtual void addDefParameterAssignment(
      const naja::verilog::Identifiers& hierarchicalParameter,
      const naja::verilog::Expression& expression) override;
    void addAttribute(
      const std::string& attributeName,
      const naja::verilog::ConstantExpression& expression) override;

    struct OrderedInstanceConnection {
      OrderedInstanceConnection() = default;
      OrderedInstanceConnection(const OrderedInstanceConnection&) = default;
      OrderedInstanceConnection(
        const size_t portIndex,
        const naja::verilog::Expression& expression):
        portIndex_(portIndex),
        expression_(expression)
      {}

      std::string getString() const {
        std::ostringstream stream;
        stream << "OrderedInstanceConnection - port: "
          << portIndex_ << " : " << expression_.getString();
        return stream.str();
      }
      
      size_t                    portIndex_  {0};
      naja::verilog::Expression expression_ {};
    };

    struct InstanceConnection {
      InstanceConnection() = default;
      InstanceConnection(const InstanceConnection&) = default;
      InstanceConnection(
        const naja::verilog::Identifier& port,
        const naja::verilog::Expression& expression):
        port_(port),
        expression_(expression)
      {}

      std::string getString() const {
        std::ostringstream stream;
        stream << "InstanceConnection - port: "
          << port_.getString() << " : " << expression_.getString();
        return stream.str();
      }

      naja::verilog::Identifier port_       {};
      naja::verilog::Expression expression_ {};
    };

    struct Instance {
      using Connections = std::vector<InstanceConnection>;
      using OrderedConnections = std::vector<OrderedInstanceConnection>;
      using ParameterAssignments = std::map<std::string, std::string>;
      Instance() = default;
      Instance(const Instance&) = default;
      Instance(const naja::verilog::Identifier& model, const naja::verilog::Identifier& id):
        model_(model), identifier_(id)
      {}

      std::string getString() const {
        std::ostringstream stream;
        stream << "Instance: (" << model_.getString() << ") "
          << identifier_.getString();
        return stream.str();
      }
  
      naja::verilog::Identifier   model_                {};
      naja::verilog::Identifier   identifier_           {};
      ParameterAssignments        parameterAssignments_ {};
      Connections                 connections_          {};
      OrderedConnections          orderedConnections_   {};
    };

    struct Assign {
      Assign(
        const naja::verilog::RangeIdentifiers& identifiers,
        const naja::verilog::Expression& expression):
        identifiers_(identifiers),
        expression_(expression)
      {}
      naja::verilog::RangeIdentifiers  identifiers_  {};
      naja::verilog::Expression   expression_   {};
    };

    struct Module {
      using Ports = std::vector<naja::verilog::Port>;
      using Nets = std::vector<naja::verilog::Net>;
      using Instances = std::vector<Instance>;
      using Assigns = std::vector<Assign>;
      using DefParameterAssignment = std::pair<naja::verilog::Identifiers, naja::verilog::Expression>;
      using DefParameterAssignments = std::vector<DefParameterAssignment>; 
      naja::verilog::Identifier       identifier_                           {};
      Ports                           ports_                                {};
      Nets                            nets_                                 {};
      Assigns                         assigns_                              {};
      Instances                       instances_                            {};
      Instance::ParameterAssignments  currentInstanceParameterAssignments_  {};
      DefParameterAssignments         defParameterAssignments_              {};

      Module(const naja::verilog::Identifier& identifier):
        identifier_(identifier)
      {}
    };

    using Modules = std::vector<Module*>;
    using ModulesMap = std::map<std::string, Module*>;
    using Attributes = std::list<naja::verilog::Attribute>;
    void addModule(Module* module);
    
    bool        firstPass_        {true};
    Modules     modules_          {};
    ModulesMap  modulesMap_       {};
    Module*     currentModule_    {nullptr};
    Attributes  attributes_       {};
    std::string currentModelName_ {};
};

#endif /* __VERILOG_CONSTRUCTOR_TEST_H_ */
