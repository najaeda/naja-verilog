#include "VerilogConstructor.h"

#include <iostream>

namespace {

class VerilogConstructorExample: public naja::verilog::VerilogConstructor {
  public:
    void startModule(std::string&& name) override {
      std::cerr << "Construct Module: " << name << std::endl;
    }
    void moduleInterfacePort(std::string&& name) override {
      std::cerr << "Simple Port: " << name << std::endl;
    }
    void moduleImplementationPort(naja::verilog::Port&& port) override {
      std::cerr << "Construct Port: " << port.getString() << std::endl;
    }
};

}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    return -54;
  }

  VerilogConstructorExample constructor;
  std::filesystem::path filePath(argv[1]);
  constructor.parse(filePath);
  return 0;
}
