#ifndef __VERILOG_TYPES_H_
#define __VERILOG_TYPES_H_

#include <vector>
#include <string>
#include <sstream>

namespace naja { namespace verilog {

struct Range {
  bool  valid { false };
  int   msb;
  int   lsb;
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
        DirectionEnum dirEnum_ { Unknown };
  };  
  std::string name;
  Direction   direction;
  Range       range;

  std::string getString() const {
    std::ostringstream stream;
    stream << "Port: " << name;
    if (range.valid) {
      stream << "[" << range.msb << ":" << range.lsb << "]";
    }
    stream << " " << direction.getString();
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
