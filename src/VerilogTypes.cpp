// SPDX-FileCopyrightText: 2023 The Naja verilog authors <https://github.com/najaeda/naja-verilog/blob/main/AUTHORS>
//
// SPDX-License-Identifier: Apache-2.0

#include "VerilogTypes.h"
#include "VerilogException.h"

#include <sstream>

namespace naja { namespace verilog {

std::string Identifier::getString() const {
  return name_;
}

//LCOV_EXCL_START
std::string Identifier::getDescription() const {
  if (escaped_) {
    return "Identifier: [\\" + name_ + " ] (escaped)";
  } else {
    return "Identifier: [" + name_ + "]";
  }
}
//LCOV_EXCL_STOP

//LCOV_EXCL_START
std::string Range::getString() const {
  std::ostringstream stream;
  if (not valid_) {
    stream << "[not valid]";
    return stream.str();
  }
  if (singleValue_) {
    stream << "[" << msb_ << "]";
  } else {
    stream << "[" << msb_ << ":" << lsb_ << "]";
  }
  return stream.str();
}
//LCOV_EXCL_STOP

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

//LCOV_EXCL_START
std::string Port::getString() const {
  std::ostringstream stream;
  stream << "Port: " << identifier_.getString();
  if (range_.valid_) {
    stream << range_.getString();
  }
  stream << " " << direction_.getString();
  return stream.str();
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

//LCOV_EXCL_START
std::string Net::getString() const {
  std::ostringstream stream;
  stream << "Net: " << identifier_.getString();
  if (range_.valid_) {
    stream << range_.getString();
  }
  stream << " " << type_.getString();
  return stream.str();
}
//LCOV_EXCL_STOP

//LCOV_EXCL_START
std::string RangeIdentifier::getString() const {
  std::ostringstream stream;
  stream << identifier_.getString();
  if (range_.valid_) {
    stream << range_.getString();
  }
  return stream.str();
}
//LCOV_EXCL_STOP

//LCOV_EXCL_START
std::string RangeIdentifier::getDescription() const {
  return "RangeIdentifier: " + getString();
}
//LCOV_EXCL_STOP

//LCOV_EXCL_START
std::string BasedNumber::getBaseString(Base base) {
  switch (base) {
    case BINARY:
      return "BINARY";
    case OCTAL:
      return "OCTAL";
    case HEX:
      return "HEX";
    case DECIMAL:
      return "DECIMAL";
  }
  return "UNKNOWN";
}
//LCOV_EXCL_STOP

//LCOV_EXCL_START
std::string BasedNumber::getString() const {
  std::ostringstream stream;
  if (hasSize_) {
    stream << size_;
  }
  stream << "'";
  if (signed_) {
    stream << 's';
  }
  switch (base_) {
    case BINARY:
      stream << "b";
      break;
    case OCTAL:
      stream << "o";
      break;
    case HEX:
      stream << "h";
      break;
    case DECIMAL:
      stream << "d";
      break;
  }
  stream << digits_;
  return stream.str();
}
//LCOV_EXCL_STOP

//LCOV_EXCL_START
std::string BasedNumber::getDescription() const {
  return "BasedNumber: " + getString();
}
//LCOV_EXCL_STOP

int Number::getInt() const {
  switch (value_.index()) {
    case Type::UNSIGNED: {
      int value = std::get<Type::UNSIGNED>(value_);
      return sign_?value:-value;
    }
    default: {
      throw VerilogException("Unsupported number");
    }
  }
  return 0;
}

//LCOV_EXCL_START
std::string Number::getString() const {
  switch (value_.index()) {
    case Type::BASED:
      return std::get<Type::BASED>(value_).getString();
    case Type::UNSIGNED:
      return std::to_string(std::get<Type::UNSIGNED>(value_));
  }
  return std::string();
}
//LCOV_EXCL_STOP

//LCOV_EXCL_START
std::string Number::getDescription() const {
  std::ostringstream stream;
  switch (value_.index()) {
    case Type::BASED:
      stream << std::get<Type::BASED>(value_).getDescription();
      break; 
    case Type::UNSIGNED:
      stream << "unsigned: " << std::get<Type::UNSIGNED>(value_);
      break; 
  }
  return stream.str();
}
//LCOV_EXCL_STOP

Concatenation::Concatenation(const Expressions& expressions)
  :expressions_(expressions)
{}

//LCOV_EXCL_START
std::string Concatenation::getString() const {
  std::ostringstream stream;
  stream << "{ ";
  bool first = true;
  for (auto expression: expressions_) {
    if (not first) {
      stream << ", ";
    }
    stream << expression.getString();
    first = false;
  }
  stream << " }";
  return stream.str(); 
}
//LCOV_EXCL_STOP

//LCOV_EXCL_START
std::string Concatenation::getDescription() const {
  return "Concatenation: " + getString();
}
//LCOV_EXCL_STOP

//LCOV_EXCL_START
std::string Expression::getString() const {
  std::ostringstream stream;
  switch (value_.index()) {
    case Type::RANGEIDENTIFIER:
      return std::get<Type::RANGEIDENTIFIER>(value_).getString();
    case Type::NUMBER:
      return std::get<Type::NUMBER>(value_).getString();
    case Type::STRING:
      return std::get<Type::STRING>(value_);
    case Type::CONCATENATION:
      return std::get<Type::CONCATENATION>(value_).getString();
  }
  return std::string();
} 
//LCOV_EXCL_STOP

//LCOV_EXCL_START
std::string Expression::getDescription() const {
  std::ostringstream stream;
  stream << "Expression: (valid: " << valid_ << " supported: " << supported_ << ") ";
  switch (value_.index()) {
    case Type::RANGEIDENTIFIER:
      stream << std::get<Type::RANGEIDENTIFIER>(value_).getDescription();
      break; 
    case Type::NUMBER:
      stream << std::get<Type::NUMBER>(value_).getDescription();
      break; 
    case Type::CONCATENATION:
      stream << std::get<Type::CONCATENATION>(value_).getDescription();
      break;
  }
  return stream.str();
} 
//LCOV_EXCL_STOP

}} // namespace verilog // namespace naja