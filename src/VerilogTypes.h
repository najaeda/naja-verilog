#ifndef __VERILOG_TYPES_H_
#define __VERILOG_TYPES_H_

#include <vector>
#include <string>

namespace naja { namespace verilog {

struct Port {
  enum class Direction {
    Input, Output, Inout
  };
  std::string name;
  Direction   direction;
};

}} // namespace verilog // namespace naja

#endif /* __VERILOG_TYPES_H_ */