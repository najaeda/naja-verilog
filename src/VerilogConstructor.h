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
    using Paths = std::list<std::filesystem::path>;
    void parse(const Paths& paths);
    void parse(const std::filesystem::path& path);
    bool inFirstPass() const;
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
    virtual void addInstanceConnection(std::string&& portName) {}
    virtual void endInstantiation() {}
    virtual void endModule() {}
  private:
    class Mode {
      public:
        enum ModeEnum {
          FirstPass, SecondPass, FullPass
        };
        Mode() = default;
        Mode(const ModeEnum& modeEnum);
        Mode(const Mode&) = default;
        Mode(Mode&&) = default;
        Mode& operator=(const Mode&) = default;

        operator const ModeEnum&() const {return modeEnum_;}
        std::string getString() const;
        private:
          ModeEnum modeEnum_ { FullPass };
    };
    void internalParse(std::istream& stream);
    bool            twoPass_  {false};
    Mode            mode_     {Mode::FullPass};
    VerilogScanner* scanner_  {nullptr};
    VerilogParser*  parser_   {nullptr};
};

}} // namespace verilog // namespace naja

#endif /* __VERILOG_CONSTRUCTOR_H_ */
