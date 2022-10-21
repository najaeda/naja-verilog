#ifndef __VERILOG_CONSTRUCTOR_TEST_H_
#define __VERILOG_CONSTRUCTOR_TEST_H_

#include "VerilogConstructor.h"

class VerilogConstructorTest: public naja::verilog::VerilogConstructor {
  public:
    void createModule(const std::string& name);
    void createPort(const std::string& name);
};

#endif /* __VERILOG_CONSTRUCTOR_TEST_H_ */
