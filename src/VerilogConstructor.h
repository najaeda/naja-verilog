#ifndef __VERILOG_CONSTRUCTOR_H_
#define __VERILOG_CONSTRUCTOR_H_

#include <list>
#include <filesystem>

class VerilogConstructor {
  public:
    using Paths = std::list<std::filesystem::path>;
    void parse(const Paths& paths);
    void parse(const std::filesystem::path& path);
    virtual void createModule(const char* name) {}
    virtual void createPort(const char* name) {}
};

#endif /* __VERILOG_CONSTRUCTOR_H_ */