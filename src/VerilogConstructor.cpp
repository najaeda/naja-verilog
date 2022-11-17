#include "VerilogConstructor.h"

#include <fstream>
#include "VerilogException.h"
#include "VerilogScanner.h"
#include "VerilogParser.hpp"

namespace naja { namespace verilog {

VerilogConstructor::Mode::Mode(const ModeEnum& modeEnum):
  modeEnum_(modeEnum)
{}

//LCOV_EXCL_START
std::string VerilogConstructor::Mode::getString() const {
  switch (modeEnum_) {
    case Mode::FirstPass: return "FirstPass";
    case Mode::SecondPass: return "SecondPass";
    case Mode::FullPass: return "FullPass";
  }
  return "Error";
}
//LCOV_EXCL_STOP

void VerilogConstructor::parse(const std::filesystem::path& path) {
  if (not std::filesystem::exists(path)) {
    std::string reason(path.string() + " does not exist");
    throw VerilogException(reason);
  }
  std::ifstream inFile(path);
  if (not inFile.good()) {
    std::string reason(path.string() + " is not a readable file");
    throw VerilogException(reason);
  }
  if (twoPass_) {
    mode_ = Mode::FirstPass;
    internalParse(inFile);
    mode_ = Mode::SecondPass;
    internalParse(inFile);
  } else {
    mode_ = Mode::FullPass;
    internalParse(inFile);
  }
}

void VerilogConstructor::parse(const VerilogConstructor::Paths& paths) {
  for (auto path: paths) {
    parse(path);
  }
}

bool VerilogConstructor::inFirstPass() const {
  return mode_ == Mode::FirstPass;
}

void VerilogConstructor::internalParse(std::istream &stream) {
  delete(scanner_);
  scanner_ = new VerilogScanner(&stream);
  delete(parser_); 
  parser_ = new VerilogParser(*scanner_, this);
  bool result = parser_->parse();
}

}} // namespace verilog // namespace naja
