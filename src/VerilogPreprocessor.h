// SPDX-FileCopyrightText: 2026 The Naja verilog authors
//
// SPDX-License-Identifier: Apache-2.0

#ifndef __VERILOG_PREPROCESSOR_H_
#define __VERILOG_PREPROCESSOR_H_

#include <filesystem>
#include <istream>
#include <string>
#include <unordered_map>
#include <vector>

namespace naja { namespace verilog {

class VerilogPreprocessor {
  public:
    VerilogPreprocessor() = default;

    void reset();
    std::string preprocessFile(const std::filesystem::path& path);

  private:
    struct IfState {
      bool parent_active {true};
      bool this_active {true};
      bool any_taken {false};
    };

    std::string preprocessStream(
      std::istream& stream,
      const std::filesystem::path& currentPath);

    std::string expandMacrosInLine(const std::string& line);

    bool handleDirective(
      const std::string& line,
      const std::filesystem::path& currentPath,
      unsigned lineNumber,
      std::string& output);

    bool isActive() const;

    std::filesystem::path resolveInclude(
      const std::string& includePath,
      const std::filesystem::path& currentPath);

    static bool isIdentifierStart(char ch);
    static bool isIdentifierChar(char ch);

    std::unordered_map<std::string, std::string> macros_ {};
    std::vector<IfState> ifStack_ {};
    std::vector<std::filesystem::path> includeStack_ {};
    bool inBlockComment_ {false};
};

}} // namespace verilog // namespace naja

#endif // __VERILOG_PREPROCESSOR_H_
