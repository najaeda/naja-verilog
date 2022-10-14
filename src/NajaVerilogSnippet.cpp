#include "VerilogConstructor.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    return -54;
  }

  VerilogConstructor constructor;
  std::filesystem::path filePath(argv[1]);
  constructor.parse(filePath);
}