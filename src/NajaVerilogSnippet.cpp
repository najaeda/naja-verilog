#include "VerilogConstructor.h"

namespace {

class VerilogConstructorExample: public naja::verilog::VerilogConstructor {
  public:
    void createModule(const char* name) override {

    }
    void createPort(const char* name) override {

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