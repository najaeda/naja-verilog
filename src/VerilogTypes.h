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

#ifndef __VERILOG_TYPES_H_
#define __VERILOG_TYPES_H_

#include <vector>
#include <string>
#include <sstream>

namespace naja { namespace verilog {

struct Range {
  Range() = default;
  Range(const Range&) = default;

  std::string getString() const {
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

  //Range has been parsed
  bool  valid_        {false};
  bool  singleValue_  {false};
  int   msb_          {0};
  int   lsb_          {0};
};

struct Port {
  class Direction {
    public:
      enum DirectionEnum {
        Input, Output, InOut, Unknown
      };
      Direction() = default;
      Direction(const DirectionEnum& dirEnum);
      Direction(const Direction&) = default;
      Direction(Direction&&) = default;
      Direction& operator=(const Direction&) = default;

      operator const DirectionEnum&() const {return dirEnum_;}
      std::string getString() const;
      private:
        DirectionEnum dirEnum_  { Unknown };
  };  

  Port() = default;
  Port(const Port&) = default;
  Port(const std::string& name, Direction direction):
    name_(name),
    direction_(direction)
  {}

  Port(const std::string& name, Direction direction, const Range& range):
    name_(name),
    direction_(direction),
    range_(range)
  {}

  bool isBus() const { return range_.valid_; }

  std::string name_       {};
  Direction   direction_  {};
  Range       range_      {};

  std::string getString() const {
    std::ostringstream stream;
    stream << "Port: " << name_;
    if (range_.valid_) {
      stream << range_.getString();
    }
    stream << " " << direction_.getString();
    return stream.str();
  }
};

struct Net {
  class Type {
    public:
      enum TypeEnum {
        Wire, Supply0, Supply1, Unknown
      };
      Type() = default;
      Type(const TypeEnum& dirEnum);
      Type(const Type&) = default;
      Type(Type&&) = default;
      Type& operator=(const Type&) = default;

      operator const TypeEnum&() const {return typeEnum_;}
      std::string getString() const;
      private:
        TypeEnum typeEnum_ { Unknown };
  };

  Net() = default;
  Net(const Net&) = default;
  
  Net(const std::string& name, const Range& range, Type type):
    name_(name),
    range_(range),
    type_(type)
  {}

  bool isBus() const { return range_.valid_; }

  std::string name_   {};
  Range       range_  {};
  Type        type_   {};

  std::string getString() const {
    std::ostringstream stream;
    stream << "Net: " << name_;
    if (range_.valid_) {
      stream << range_.getString();
    }
    stream << " " << type_.getString();
    return stream.str();
  } 
};

struct Identifier {
  Identifier() = default;
  Identifier(const Identifier&) = default;
  std::string getString() const {
    std::ostringstream stream;
    stream << name_;
    if (range_.valid_) {
      stream << range_.getString();
    }
    return stream.str();
  }

  std::string name_;
  Range       range_;
};

struct Expression {
  using Expressions = std::vector<Expression>;
  Expression() = default;
  Expression(const Expression&) = default;
  enum Type { IDENTIFIER }; 
  //Means 
  bool        valid_          {false};
  //If valid_ is true and supported_ is false, then this expression construction
  //is not currently supported.
  bool        supported_      {true};
  Type        type_           {Type::IDENTIFIER};
  Identifier  identifier_     {};
  Range       range_          {};      
  Expressions concatenation_  {};
  
  std::string getString() const {
    std::ostringstream stream;
    stream << "Expression: (valid: " << valid_ << " supported: " << supported_ << ") ";
    if (range_.valid_) {
      stream << range_.getString();
    }
    if (type_ == Type::IDENTIFIER) {
      stream << " id: " << identifier_.getString();
    }
    return stream.str();
  } 
};


}} // namespace verilog // namespace naja

#endif /* __VERILOG_TYPES_H_ */
