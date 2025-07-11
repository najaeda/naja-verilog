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
    void startGateInstantiation(const naja::verilog::GateType& type) override;
    void addGateInstance(const naja::verilog::Identifier& name) override;
    void addGateOutputInstanceConnection(
      size_t portIndex,
      const naja::verilog::RangeIdentifiers identifiers) override;
    void addGateInputInstanceConnection(
      size_t portIndex,
      const naja::verilog::Expression& expression) override;
    void endGateInstantiation() override;
    void addNet(const naja::verilog::Net& net) override;
    void addAssign(
      const naja::verilog::RangeIdentifiers& identifiers,
      const naja::verilog::Expression& expression) override;
    virtual void addDefParameterAssignment(
      const naja::verilog::Identifiers& hierarchicalParameter,
      const naja::verilog::ConstantExpression& expression) override;
    void addAttribute(
      const naja::verilog::Identifier& attributeName,
      const naja::verilog::ConstantExpression& expression) override;
    struct OrderedInstanceConnection {
      enum Type { RANGEIDENTIFIERS=0, EXPRESSION=1 };
      using Value = std::variant<
        naja::verilog::RangeIdentifiers,
        naja::verilog::Expression>;
      OrderedInstanceConnection() = default;
      OrderedInstanceConnection(const OrderedInstanceConnection&) = default;
      OrderedInstanceConnection(
        const size_t portIndex,
        const naja::verilog::Expression& expression):
        portIndex_(portIndex),
        value_(expression)
      {}

      OrderedInstanceConnection(
        const size_t portIndex,
        const naja::verilog::RangeIdentifiers& identifiers):
        portIndex_(portIndex),
        value_(identifiers)
      {}

      std::string getString() const {
        std::ostringstream stream;
        stream << "OrderedInstanceConnection - port: "
          << portIndex_ << " : ";
        switch (value_.index()) {
          case EXPRESSION:
            stream << std::get<naja::verilog::Expression>(value_).getString();
            break;
          case RANGEIDENTIFIERS:
            break;
        }
        return stream.str();
      }
      
      size_t  portIndex_  {0};
      Value   value_ {};
    };

    using Attributes = std::list<naja::verilog::Attribute>;

    struct ObjectWithAttributes {
      ObjectWithAttributes() = default;
      void addAttributes(const Attributes& attributes) {
        attributes_.insert(attributes_.end(), attributes.begin(), attributes.end());
      }

      Attributes  attributes_ {};
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

    struct Instance: public ObjectWithAttributes {
      using OrderedConnections = std::vector<OrderedInstanceConnection>;
      virtual ~Instance() = default;

      virtual std::string getString() const = 0;

      OrderedConnections          orderedConnections_   {};
    };

    struct ModuleInstance: public Instance {
      using Connections = std::vector<InstanceConnection>;
      using ParameterAssignments = std::map<std::string, std::string>;
      ModuleInstance() = default;
      ModuleInstance(const ModuleInstance&) = default;
      ModuleInstance(const naja::verilog::Identifier& model, const naja::verilog::Identifier& id):
        model_(model), identifier_(id)
      {}

      std::string getString() const override {
        std::ostringstream stream;
        stream << "ModuleInstance: (" << model_.getString() << ") "
          << identifier_.getString();
        return stream.str();
      }
  
      naja::verilog::Identifier   model_                {};
      naja::verilog::Identifier   identifier_           {};
      ParameterAssignments        parameterAssignments_ {};
      Connections                 connections_          {};
    };

    struct GateInstance: public Instance {
      GateInstance() = default;
      GateInstance(const GateInstance&) = default;
      GateInstance(const naja::verilog::GateType& type,
        const naja::verilog::Identifier& id):
        type_(type),
        identifier_(id)
      {}

      bool isAnonymous() const { return identifier_.empty(); }

      std::string getString() const {
        std::ostringstream stream;
        stream << "GateInstance: " << identifier_.getString();
        return stream.str();
      }
      naja::verilog::GateType   type_       {};
      naja::verilog::Identifier identifier_ {};
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
      using Instances = std::vector<Instance*>;
      using Assigns = std::vector<Assign>;
      using DefParameterAssignment = std::pair<naja::verilog::Identifiers, naja::verilog::ConstantExpression>;
      using DefParameterAssignments = std::vector<DefParameterAssignment>; 
      naja::verilog::Identifier             identifier_                           {};
      Ports                                 ports_                                {};
      Nets                                  nets_                                 {};
      Assigns                               assigns_                              {};
      Instances                             instances_                            {};
      ModuleInstance::ParameterAssignments  currentInstanceParameterAssignments_  {};
      DefParameterAssignments               defParameterAssignments_              {};

      Module(const naja::verilog::Identifier& identifier):
        identifier_(identifier)
      {}
      ~Module() {
        for (auto instance : instances_) {
          delete instance;
        }
      }
    };

    using Modules = std::vector<Module*>;
    using ModulesMap = std::map<std::string, Module*>;
    void addModule(Module* module);
    
    bool                    firstPass_            {true};
    Modules                 modules_              {};
    ModulesMap              modulesMap_           {};
    Module*                 currentModule_        {nullptr};
    naja::verilog::GateType currentGateType_      {naja::verilog::GateType::Unknown};
    Attributes              nextObjectAttributes_ {};
    std::string             currentModelName_     {};
};

#endif /* __VERILOG_CONSTRUCTOR_TEST_H_ */
