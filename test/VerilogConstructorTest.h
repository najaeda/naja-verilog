// SPDX-FileCopyrightText: 2023 The Naja verilog authors <https://github.com/xtofalex/naja-verilog/blob/main/AUTHORS>
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
    void startModule(const std::string& name) override;
    void moduleInterfaceSimplePort(const std::string& name) override;
    void moduleImplementationPort(const naja::verilog::Port& port) override;
    void moduleInterfaceCompletePort(const naja::verilog::Port& port) override;
    void startInstantiation(const std::string& modelName) override;
    void addInstance(const std::string& name) override;
    void addInstanceConnection(
      const std::string& portName,
      const naja::verilog::Expression& expression) override;
    void addOrderedInstanceConnection(
      size_t portIndex,
      const naja::verilog::Expression& expression) override;
    void addParameterAssignment(
      const std::string& parameterName,
      const naja::verilog::Expression& expression) override;
    void endInstantiation() override;
    void addNet(const naja::verilog::Net& net) override;
    void addAssign(
      const naja::verilog::Identifiers& identifiers,
      const naja::verilog::Expression& expression) override;


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
        const std::string& port,
        const naja::verilog::Expression& expression):
        port_(port),
        expression_(expression)
      {}

      std::string getString() const {
        std::ostringstream stream;
        stream << "InstanceConnection - port: "
          << port_ << " : " << expression_.getString();
        return stream.str();
      }
      
      std::string               port_       {};
      naja::verilog::Expression expression_ {};
    };

    struct Instance {
      using Connections = std::vector<InstanceConnection>;
      using OrderedConnections = std::vector<OrderedInstanceConnection>;
      using ParameterAssignments = std::map<std::string, std::string>;
      Instance() = default;
      Instance(const Instance&) = default;
      Instance(const std::string& model, const std::string& name):
        model_(model), name_(name)
      {}

      std::string getString() const {
        std::ostringstream stream;
        stream << "Instance: (" << model_ << ") " << name_;
        return stream.str();
      }
  
      std::string           model_                {};
      std::string           name_                 {};
      ParameterAssignments  parameterAssignments_ {};
      Connections           connections_          {};
      OrderedConnections    orderedConnections_   {};
    };

    struct Assign {
      Assign(
        const naja::verilog::Identifiers& identifiers,
        const naja::verilog::Expression& expression):
        identifiers_(identifiers),
        expression_(expression)
      {}
      naja::verilog::Identifiers  identifiers_  {};
      naja::verilog::Expression   expression_   {};
    };

    struct Module {
      using Ports = std::vector<naja::verilog::Port>;
      using Nets = std::vector<naja::verilog::Net>;
      using Instances = std::vector<Instance>;
      using Assigns = std::vector<Assign>;
      std::string                     name_                                 {};
      Ports                           ports_                                {};
      Nets                            nets_                                 {};
      Assigns                         assigns_                              {};
      Instances                       instances_                            {};
      Instance::ParameterAssignments  currentInstanceParameterAssignments_  {};

      Module(const std::string& name):
        name_(name)
      {}
    };

    using Modules = std::vector<Module*>;
    using ModulesMap = std::map<std::string, Module*>;
    void addModule(Module* module);
    
    bool        firstPass_        {true};
    Modules     modules_          {};
    ModulesMap  modulesMap_       {};
    Module*     currentModule_    {nullptr};
    std::string currentModelName_ {};
};

#endif /* __VERILOG_CONSTRUCTOR_TEST_H_ */
