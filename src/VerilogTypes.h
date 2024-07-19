// SPDX-FileCopyrightText: 2023 The Naja verilog authors <https://github.com/najaeda/naja-verilog/blob/main/AUTHORS>
//
// SPDX-License-Identifier: Apache-2.0

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
  Range(int msb, int lsb):
    valid_(true), singleValue_(false), msb_(msb), lsb_(lsb) {}
  Range(int value):
    valid_(true), singleValue_(true), msb_(value) {}
  bool operator==(const Range& other) const {
    return valid_ == other.valid_
    && singleValue_ == other.singleValue_
    && msb_ == other.msb_
    && lsb_ == other.lsb_;
  }

  std::string getString() const;

  //Range has been parsed
  bool  valid_        {false};
  bool  singleValue_  {false};
  int   msb_          {0};
  int   lsb_          {0};
};

struct Identifier {
  Identifier() = default;
  Identifier(const Identifier&) = default;
  Identifier(const std::string& name, bool escaped=false): name_(name), escaped_(escaped) {}
  bool operator==(const Identifier& other) const {
    return name_ == other.name_ && escaped_ == other.escaped_;
  }
  std::string getString() const;
  std::string getDescription() const;

  std::string name_;
  bool        escaped_  {false};
};

using Identifiers = std::vector<Identifier>;

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
  Port(const Identifier& identifier, Direction direction):
    identifier_(identifier),
    direction_(direction)
  {}

  Port(const Identifier& identifier, Direction direction, const Range& range):
    identifier_(identifier),
    range_(range),
    direction_(direction)
  {}

  bool isBus() const { return range_.valid_; }

  Identifier  identifier_ {};
  Range       range_      {};
  Direction   direction_  {};

  std::string getString() const;
};

using Ports = std::vector<Port>;

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
  
  Net(const Identifier& identifier, const Range& range, Type type):
    identifier_(identifier),
    range_(range),
    type_(type)
  {}

  bool isBus() const { return range_.valid_; }

  Identifier  identifier_ {};
  Range       range_      {};
  Type        type_       {};

  std::string getString() const;
};

struct RangeIdentifier {
  RangeIdentifier() = default;
  RangeIdentifier(const RangeIdentifier&) = default;
  RangeIdentifier(const Identifier& identifier): identifier_(identifier) {}
  RangeIdentifier(const Identifier& identifier, const Range& range): identifier_(identifier), range_(range) {}
  std::string getString() const;
  std::string getDescription() const;

  Identifier  identifier_;
  Range       range_;
};

using RangeIdentifiers = std::vector<RangeIdentifier>;

struct BasedNumber {
  BasedNumber() = default;
  BasedNumber(bool isSigned, char base, const std::string& digits) {
    hasSize_ = false;
    signed_ = isSigned;
    switch (base) {
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
  BasedNumber(const std::string& size, bool isSigned, char base, const std::string& digits):
    BasedNumber(isSigned, base, digits) {
    hasSize_ = true;
    size_ = static_cast<unsigned>(std::stoul(size));
  }
  enum Base { BINARY, OCTAL, HEX, DECIMAL };

  static std::string getBaseString(Base base);
  std::string getString() const;
  std::string getDescription() const;
 
  bool        hasSize_  {false};
  size_t      size_     {0};
  bool        signed_   {false};
  Base        base_     {};
  std::string digits_   {};
};

struct Number {
  Number() = default;
  Number(const Number&) = default;
  Number(const std::string& value) {
    value_ = static_cast<unsigned>(std::stoul(value));
  }
  Number(const std::string& size, bool isSigned, char base, const std::string& value) {
    value_ = BasedNumber(size, isSigned, base, value);
  }
  Number(bool isSigned, char base, const std::string& value) {
    value_ = BasedNumber(isSigned, base, value);
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
  Concatenation(const Expressions& expressions);
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

  enum Type { RANGEIDENTIFIER=0, NUMBER=1, STRING=2, CONCATENATION=3 }; 
  using Value = std::variant<RangeIdentifier, Number, std::string, Concatenation>;

  bool          valid_          {false};
  //If valid_ is true and supported_ is false, then this expression construction
  //is not currently supported.
  bool          supported_      {true};
  Value         value_          {};
};

}} // namespace verilog // namespace naja

#endif /* __VERILOG_TYPES_H_ */