#include "VerilogConstructor.h"

#include <iostream>

namespace {

class VerilogConstructorExample: public naja::verilog::VerilogConstructor {
  public:
    void createModule(const std::string& name) override {
      std::cerr << "Construct Module: " << name << std::endl;
    }
    void createPort(const std::string& name) override {
      std::cerr << "Construct Port: " << name << std::endl;
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
