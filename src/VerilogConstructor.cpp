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

void VerilogConstructor::parseFirstPass(const std::filesystem::path& path) {
  if (not std::filesystem::exists(path)) {
    std::string reason(path.string() + " does not exist");
    throw VerilogException(reason);
  }
  std::ifstream inFile(path);
  if (not inFile.good()) {
    std::string reason(path.string() + " is not a readable file");
    throw VerilogException(reason);
  }
  mode_ = Mode::FirstPass;
  internalParse(inFile);
}

void VerilogConstructor::parseSecondPass(const std::filesystem::path& path) {
  if (not std::filesystem::exists(path)) {
    std::string reason(path.string() + " does not exist");
    throw VerilogException(reason);
  }
  std::ifstream inFile(path);
  if (not inFile.good()) {
    std::string reason(path.string() + " is not a readable file");
    throw VerilogException(reason);
  }
  mode_ = Mode::SecondPass;
  internalParse(inFile);
}

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
  mode_ = Mode::FullPass;
  internalParse(inFile);
}

void VerilogConstructor::parseFirstPass(const VerilogConstructor::Paths& paths) {
  for (auto path: paths) {
    parseFirstPass(path);
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

bool VerilogConstructor::inFullPass() const {
  return mode_ == Mode::FullPass;
}

void VerilogConstructor::internalParse(std::istream &stream) {
  delete(scanner_);
  scanner_ = new VerilogScanner(&stream);
  delete(parser_); 
  parser_ = new VerilogParser(*scanner_, this);
  bool result = parser_->parse();
}

}} // namespace verilog // namespace naja
