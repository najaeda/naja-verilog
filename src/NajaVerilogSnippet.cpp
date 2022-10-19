#include "VerilogConstructor.h"

namespace {

class VerilogConstructorExample: public naja::verilog::VerilogConstructor {
  public:
    void createModule(const std::string& name) override {

    }
    void createPort(const std::string& name) override {

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
}
