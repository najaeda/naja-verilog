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
    //Simple Port declaration (only name), no range, no direction in module interface
    virtual void moduleInterfaceSimplePortDeclaration(std::string&& name) {}
    virtual void moduleContentFullPortDeclaration(Port&& port) {}
    virtual void startModule(std::string&& name) {}
  private:
    void internalParse(std::istream& stream);
    VerilogScanner* scanner_  {nullptr};
    VerilogParser*  parser_   {nullptr};
};

}} // namespace verilog // namespace naja

#endif /* __VERILOG_CONSTRUCTOR_H_ */
