#include "VerilogConstructor.h"

#include "VerilogParser.hpp"

void VerilogConstructor::parse(const std::filesystem::path& path) {
  if (not std::filesystem::exists(path)) {
    return;
  }

  auto parser = new verilog::VerilogParser(constructor);
}

void VerilogConstructor::parse(const VerilogConstructor::Paths& paths) {
  for (auto path: paths) {
    parse(path);
  }
  #if 0

  std::ifstream ifs(p);

  if(!_scanner){
    _scanner = new VerilogScanner(&ifs);
  }
  if(!_parser){
    _parser = new VerilogParser(*_scanner, this);
  }
  _parser->parse();
  #endif
}
