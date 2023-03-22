/*
 * Copyright 2022 The Naja Authors.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "VerilogTypes.h"
#include "VerilogException.h"

#include <sstream>

namespace naja { namespace verilog {

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
  stream << "Port: " << name_;
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
  stream << "Net: " << name_;
  if (range_.valid_) {
    stream << range_.getString();
  }
  stream << " " << type_.getString();
  return stream.str();
}
//LCOV_EXCL_STOP

//LCOV_EXCL_START
std::string Identifier::getString() const {
  std::ostringstream stream;
  stream << name_;
  if (range_.valid_) {
    stream << range_.getString();
  }
  return stream.str();
}
//LCOV_EXCL_STOP

//LCOV_EXCL_START
std::string Identifier::getDescription() const {
  return "Identifier: " + getString();
}
//LCOV_EXCL_STOP

//LCOV_EXCL_START
std::string BasedNumber::getString() const {
  std::ostringstream stream;
  stream << size_ << "'";
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
    case Type::IDENTIFIER:
      return std::get<Type::IDENTIFIER>(value_).getString();
    case Type::NUMBER:
      return std::get<Type::NUMBER>(value_).getString();
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
    case Type::IDENTIFIER:
      stream << std::get<Type::IDENTIFIER>(value_).getDescription();
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