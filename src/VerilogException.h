// SPDX-FileCopyrightText: 2023 The Naja verilog authors <https://github.com/xtofalex/naja-verilog/blob/main/AUTHORS>
//
// SPDX-License-Identifier: Apache-2.0

#ifndef __VERILOG_EXCEPTION_H_
#define __VERILOG_EXCEPTION_H_

namespace naja { namespace verilog {

struct VerilogException: public std::exception {
  public:
    VerilogException() = delete;
    VerilogException(const VerilogException&) = default;

    VerilogException(const std::string& reason):
      std::exception(),
      reason_(reason)
    {}

    std::string getReason() const {
      return reason_;
    }

    //LCOV_EXCL_START
    const char* what() const noexcept override {
      return reason_.c_str();
    }
    //LCOV_EXCL_STOP

  private:
    const std::string reason_;
};

}} // namespace verilog // namespace naja

#endif // __VERILOG_EXCEPTION_H_
