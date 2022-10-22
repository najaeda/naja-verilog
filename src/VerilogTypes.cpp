#include "VerilogTypes.h"

namespace naja { namespace verilog {

Port::Direction::Direction(const DirectionEnum& dirEnum):
  dirEnum_(dirEnum)
{}

//LCOV_EXCL_START
std::string Port::Direction::getString() const {
  switch (dirEnum_) {
    case Direction::Input: return "Input";
    case Direction::Output: return "Output";
    case Direction::InOut: return "InOut";
    case Direction::Unknown: return "Unknown";
  }
  return "Error";
}
//LCOV_EXCL_STOP

Net::Type::Type(const TypeEnum& typeEnum):
  typeEnum_(typeEnum)
{}

//LCOV_EXCL_START
std::string Net::Type::getString() const {
  switch (typeEnum_) {
    case Type::Supply0: return "Supply0";
    case Type::Supply1: return "Supply1";
    case Type::Wire: return "Wire";
    case Type::Unknown: return "Unknown";
  }
  return "Error";
}
//LCOV_EXCL_STOP

}} // namespace verilog // namespace naja