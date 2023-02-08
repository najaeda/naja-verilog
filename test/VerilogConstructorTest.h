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
    void endInstantiation() override;
    void addNet(const naja::verilog::Net& net) override;

    struct Instance {
      Instance() = default;
      Instance(const Instance&) = default;

      Instance(const std::string& model, const std::string& name):
        model_(model), name_(name)
      {}
  
      std::string model_  {};
      std::string name_   {};

      std::string getString() const {
        std::ostringstream stream;
        stream << "Instance: (" << model_ << ") " << name_;
        return stream.str();
      } 
    };
    struct Module {
      using Ports = std::vector<naja::verilog::Port>;
      using Nets = std::vector<naja::verilog::Net>;
      using Instances = std::vector<Instance>;
      std::string name_       {};
      Ports       ports_      {};
      Nets        nets_       {};
      Instances   instances_  {};

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
