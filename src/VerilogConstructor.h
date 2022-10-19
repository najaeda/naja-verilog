#ifndef __VERILOG_CONSTRUCTOR_H_
#define __VERILOG_CONSTRUCTOR_H_

#include <list>
#include <filesystem>
#include <istream>

namespace naja { namespace verilog {

class VerilogScanner;
class VerilogParser;
  
class VerilogConstructor {
  public:
    using Paths = std::list<std::filesystem::path>;
    void parse(const Paths& paths);
    void parse(const std::filesystem::path& path);
    virtual void createModule(const std::string& name) =0;
    virtual void createPort(const std::string& name) =0;
  private:
    void internalParse(std::istream& stream);
    VerilogScanner* scanner_  {nullptr};
    VerilogParser*  parser_   {nullptr};
};

}} // namespace verilog // namespace naja

#endif /* __VERILOG_CONSTRUCTOR_H_ */
