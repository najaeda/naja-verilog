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

#include <variant>
#include <vector>
#include <string>
#include <cassert>

namespace naja { namespace verilog {

struct Range {
  Range() = default;
  Range(const Range&) = default;

  std::string getString() const;

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

  std::string getString() const;
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

  std::string getString() const;
};

struct Identifier {
  Identifier() = default;
  Identifier(const Identifier&) = default;
  Identifier(const std::string& name): name_(name) {}
  Identifier(const std::string& name, const Range& range): name_(name), range_(range) {}
  std::string getString() const;

  std::string name_;
  Range       range_;
};

using Identifiers = std::vector<Identifier>;

struct BasedNumber {
  BasedNumber() = default;
  BasedNumber(const std::string& size, const std::string& base, const std::string& digits) {
    size_ = static_cast<unsigned>(std::stoul(size));
    assert(base.size() == 1);
    char baseChar = base[0];
    switch (baseChar) {
      case 'b': case 'B': 
        base_ = BINARY;
        break;
      case 'o': case 'O':
        base_ = OCTAL;
        break;
      case 'h': case 'H':
        base_ = HEX;
        break;
      case 'd': case 'D':
        base_ = DECIMAL;
        break;
    }
    digits_ = digits;
  }
  enum Base { BINARY, OCTAL, HEX, DECIMAL };

  std::string getString() const;
  std::string getDescription() const;
 
  size_t      size_   {0};
  Base        base_   {};
  std::string digits_ {};
};

struct Number {
  Number() = default;
  Number(const Number&) = default;
  Number(const std::string& value) {
    value_ = static_cast<unsigned>(std::stoul(value));
  }
  Number(const std::string& size, const std::string& base, const std::string& value) {
    value_ = BasedNumber(size, base, value);
  }
  std::string getString() const;
  std::string getDescription() const;

  int getInt() const;
  enum Type { BASED, UNSIGNED };
  using Value = std::variant<BasedNumber, unsigned>;

  bool  sign_   {true}; //true positive, false negative
  Value value_  {};
};

struct Expression;

struct Concatenation {
  using Expressions = std::vector<Expression>;
  Concatenation() = default;
  Concatenation(const Concatenation&) = default;
  Concatenation(const Expressions& expressions): expressions_(expressions) {}
  std::string getString() const;
  std::string getDescription() const;

  Expressions expressions_  {};
};

struct Expression {
  using Expressions = std::vector<Expression>;
  Expression() = default;
  Expression(const Expression&) = default;
  std::string getString() const;
  std::string getDescription() const;

  enum Type { IDENTIFIER=0, NUMBER=1, CONCATENATION=2 }; 
  using Value = std::variant<Identifier, Number, Concatenation>;

  bool          valid_          {false};
  //If valid_ is true and supported_ is false, then this expression construction
  //is not currently supported.
  bool          supported_      {true};
  Value         value_          {};
  Concatenation concatenation_  {};
};


}} // namespace verilog // namespace naja

#endif /* __VERILOG_TYPES_H_ */
