#ifndef __VERILOG_TYPES_H_
#define __VERILOG_TYPES_H_

#include <vector>
#include <string>
#include <sstream>

namespace naja { namespace verilog {

struct Range {
  Range() = default;
  Range(const Range&) = default;
  bool  valid_  {false};
  int   msb_    {0};
  int   lsb_    {0};
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
  
  Port(std::string&& name, Direction direction):
    name_(std::move(name)),
    direction_(direction)
  {}

  Port(std::string&& name, Direction direction, Range&& range):
    name_(std::move(name)),
    direction_(direction),
    range_(std::move(range))
  {}

  std::string name_       {};
  Direction   direction_  {};
  Range       range_      {};

  std::string getString() const {
    std::ostringstream stream;
    stream << "Port: " << name_;
    if (range_.valid_) {
      stream << "[" << range_.msb_ << ":" << range_.lsb_ << "]";
    }
    stream << " " << direction_.getString();
    return stream.str();
  }
};

struct NetIdentifier {
  NetIdentifier() = default;
  NetIdentifier(const NetIdentifier&) = default;
  NetIdentifier(std::string&& name): name_(name) {}
  NetIdentifier(std::string&& name, Range&& range): name_(name), range_(range) {}
  std::string name_   {};
  Range       range_  {};
};

using NetIdentifiers = std::vector<NetIdentifier>;

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

  std::string name_   {};
  Range       range_  {};
  Type        type_   {};

  std::string getString() const {
    std::ostringstream stream;
    stream << "Net: " << name_;
    if (range_.valid_) {
      stream << "[" << range_.msb_ << ":" << range_.lsb_ << "]";
    }
    stream << " " << type_.getString();
    return stream.str();
  } 
};
 
}} // namespace verilog // namespace naja

#endif /* __VERILOG_TYPES_H_ */
