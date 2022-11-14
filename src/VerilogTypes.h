#ifndef __VERILOG_TYPES_H_
#define __VERILOG_TYPES_H_

#include <vector>
#include <string>
#include <sstream>

namespace naja { namespace verilog {

struct Range {
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
    name_(name),
    direction_(direction)
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
};
 
}} // namespace verilog // namespace naja

#endif /* __VERILOG_TYPES_H_ */
