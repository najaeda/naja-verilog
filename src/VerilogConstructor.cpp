#include "VerilogConstructor.h"

#include <fstream>
#include "VerilogScanner.h"
#include "VerilogParser.hpp"

namespace naja { namespace verilog {

void VerilogConstructor::parse(const std::filesystem::path& path) {
  if (not std::filesystem::exists(path)) {
    return;
  }
  std::ifstream inFile(path);
  if (not inFile.good()) {
  }
  internalParse(inFile);
}

void VerilogConstructor::parse(const VerilogConstructor::Paths& paths) {
  for (auto path: paths) {
    parse(path);
  }
}

void VerilogConstructor::internalParse(std::istream &stream) {
  delete(scanner_);
  scanner_ = new VerilogScanner(&stream);
  delete(parser_); 
  parser_ = new VerilogParser(*scanner_, this);
  bool result = parser_->parse();
}

}} // namespace verilog // namespace naja
