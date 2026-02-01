// SPDX-FileCopyrightText: 2026 The Naja verilog authors
//
// SPDX-License-Identifier: Apache-2.0

#include "VerilogPreprocessor.h"

#include <algorithm>
#include <fstream>
#include <sstream>

#include "VerilogException.h"

namespace naja { namespace verilog {

void VerilogPreprocessor::reset() {
  macros_.clear();
  ifStack_.clear();
  includeStack_.clear();
  inBlockComment_ = false;
}

std::string VerilogPreprocessor::preprocessFile(
  const std::filesystem::path& path) {
  if (not std::filesystem::exists(path)) {
    std::string reason(path.string() + " does not exist");
    throw VerilogException(reason);
  }
  std::ifstream inFile(path);
  if (not inFile.good()) {
    std::string reason(path.string() + " is not a readable file");
    throw VerilogException(reason);
  }
  includeStack_.push_back(path);
  auto output = preprocessStream(inFile, path);
  includeStack_.pop_back();
  return output;
}

void VerilogPreprocessor::preprocessFileToPath(
  const std::filesystem::path& path,
  const std::filesystem::path& outputPath) {
  auto output = preprocessFile(path);
  std::ofstream outFile(outputPath);
  if (not outFile.good()) {
    std::string reason(outputPath.string() + " is not a writable file");
    throw VerilogException(reason);
  }
  outFile << output;
}

std::string VerilogPreprocessor::preprocessStream(
  std::istream& stream,
  const std::filesystem::path& currentPath) {
  std::ostringstream output;
  std::string line;
  unsigned lineNumber = 0;
  while (std::getline(stream, line)) {
    ++lineNumber;
    std::string directiveOutput;
    bool isDirective = handleDirective(line, currentPath, lineNumber, directiveOutput);
    if (isDirective) {
      output << directiveOutput;
      continue;
    }

    if (!isActive()) {
      continue;
    }

    output << expandMacrosInLine(line) << '\n';
  }
  if (!ifStack_.empty()) {
    std::ostringstream reason;
    reason << "Unclosed `ifdef/`ifndef at end of "
           << currentPath.string();
    throw VerilogException(reason.str());
  }
  return output.str();
}

static bool startsWithDirective(const std::string& line, size_t& idx) {
  while (idx < line.size() && (line[idx] == ' ' || line[idx] == '\t')) {
    ++idx;
  }
  return idx < line.size() && line[idx] == '`';
}

bool VerilogPreprocessor::handleDirective(
  const std::string& line,
  const std::filesystem::path& currentPath,
  unsigned lineNumber,
  std::string& output) {
  if (inBlockComment_) {
    return false;
  }
  size_t idx = 0;
  if (!startsWithDirective(line, idx)) {
    return false;
  }

  ++idx; // skip `
  auto readToken = [&](std::string& token) {
    token.clear();
    while (idx < line.size() && (line[idx] == ' ' || line[idx] == '\t')) {
      ++idx;
    }
    while (idx < line.size() && isIdentifierChar(line[idx])) {
      token.push_back(line[idx]);
      ++idx;
    }
  };

  std::string keyword;
  readToken(keyword);
  if (keyword.empty()) {
    return false;
  }

  if (keyword == "define") {
    if (!isActive()) {
      return true;
    }
    std::string name;
    readToken(name);
    if (name.empty()) {
      std::ostringstream reason;
      reason << "Missing macro name in `define at "
             << currentPath.string() << ":" << lineNumber;
      throw VerilogException(reason.str());
    }
    while (idx < line.size() && (line[idx] == ' ' || line[idx] == '\t')) {
      ++idx;
    }
    std::string value = line.substr(idx);
    macros_[name] = value;
    return true;
  }

  if (keyword == "undef") {
    if (!isActive()) {
      return true;
    }
    std::string name;
    readToken(name);
    if (name.empty()) {
      std::ostringstream reason;
      reason << "Missing macro name in `undef at "
             << currentPath.string() << ":" << lineNumber;
      throw VerilogException(reason.str());
    }
    macros_.erase(name);
    return true;
  }

  if (keyword == "ifdef" || keyword == "ifndef") {
    std::string name;
    readToken(name);
    if (name.empty()) {
      std::ostringstream reason;
      reason << "Missing macro name in `" << keyword << " at "
             << currentPath.string() << ":" << lineNumber;
      throw VerilogException(reason.str());
    }
    bool defined = macros_.find(name) != macros_.end();
    bool condition = (keyword == "ifdef") ? defined : !defined;
    bool parentActive = isActive();
    IfState state;
    state.parent_active = parentActive;
    state.this_active = parentActive && condition;
    state.any_taken = state.this_active;
    ifStack_.push_back(state);
    return true;
  }

  if (keyword == "elsif") {
    if (ifStack_.empty()) {
      std::ostringstream reason;
      reason << "Unexpected `elsif at "
             << currentPath.string() << ":" << lineNumber;
      throw VerilogException(reason.str());
    }
    std::string name;
    readToken(name);
    if (name.empty()) {
      std::ostringstream reason;
      reason << "Missing macro name in `elsif at "
             << currentPath.string() << ":" << lineNumber;
      throw VerilogException(reason.str());
    }
    auto& state = ifStack_.back();
    if (!state.parent_active) {
      state.this_active = false;
      return true;
    }
    if (state.any_taken) {
      state.this_active = false;
      return true;
    }
    bool defined = macros_.find(name) != macros_.end();
    state.this_active = defined;
    if (state.this_active) {
      state.any_taken = true;
    }
    return true;
  }

  if (keyword == "else") {
    if (ifStack_.empty()) {
      std::ostringstream reason;
      reason << "Unexpected `else at "
             << currentPath.string() << ":" << lineNumber;
      throw VerilogException(reason.str());
    }
    auto& state = ifStack_.back();
    if (!state.parent_active) {
      state.this_active = false;
      return true;
    }
    state.this_active = !state.any_taken;
    state.any_taken = true;
    return true;
  }

  if (keyword == "endif") {
    if (ifStack_.empty()) {
      std::ostringstream reason;
      reason << "Unexpected `endif at "
             << currentPath.string() << ":" << lineNumber;
      throw VerilogException(reason.str());
    }
    ifStack_.pop_back();
    return true;
  }

  if (keyword == "include") {
    if (!isActive()) {
      return true;
    }
    while (idx < line.size() && (line[idx] == ' ' || line[idx] == '\t')) {
      ++idx;
    }
    if (idx >= line.size() || line[idx] != '"') {
      std::ostringstream reason;
      reason << "Expected quoted include path at "
             << currentPath.string() << ":" << lineNumber;
      throw VerilogException(reason.str());
    }
    ++idx;
    size_t start = idx;
    while (idx < line.size() && line[idx] != '"') {
      ++idx;
    }
    if (idx >= line.size()) {
      std::ostringstream reason;
      reason << "Unterminated include path at "
             << currentPath.string() << ":" << lineNumber;
      throw VerilogException(reason.str());
    }
    std::string includePath = line.substr(start, idx - start);
    auto resolved = resolveInclude(includePath, currentPath);
    if (!std::filesystem::exists(resolved)) {
      std::ostringstream reason;
      reason << "Included file not found: " << resolved.string();
      throw VerilogException(reason.str());
    }
    if (std::find(includeStack_.begin(), includeStack_.end(), resolved)
        != includeStack_.end()) {
      std::ostringstream reason;
      reason << "Recursive include detected: " << resolved.string();
      throw VerilogException(reason.str());
    }
    std::ifstream includeFile(resolved);
    if (!includeFile.good()) {
      std::ostringstream reason;
      reason << "Included file not readable: " << resolved.string();
      throw VerilogException(reason.str());
    }
    includeStack_.push_back(resolved);
    output += preprocessStream(includeFile, resolved);
    includeStack_.pop_back();
    return true;
  }

  if (keyword == "timescale") {
    if (!isActive()) {
      return true;
    }
    // Consume but do not emit to keep parser-compatible output.
    return true;
  }

  std::ostringstream reason;
  reason << "Unsupported preprocessor directive `" << keyword
         << " at " << currentPath.string() << ":" << lineNumber;
  throw VerilogException(reason.str());
}

bool VerilogPreprocessor::isActive() const {
  if (ifStack_.empty()) {
    return true;
  }
  return ifStack_.back().this_active;
}

std::filesystem::path VerilogPreprocessor::resolveInclude(
  const std::string& includePath,
  const std::filesystem::path& currentPath) {
  if (std::filesystem::path(includePath).is_absolute()) {
    return std::filesystem::path(includePath);
  }
  return currentPath.parent_path() / includePath;
}

bool VerilogPreprocessor::isIdentifierStart(char ch) {
  return (ch == '_' || ch == '$' ||
          (ch >= 'A' && ch <= 'Z') ||
          (ch >= 'a' && ch <= 'z'));
}

bool VerilogPreprocessor::isIdentifierChar(char ch) {
  return isIdentifierStart(ch) || (ch >= '0' && ch <= '9');
}

std::string VerilogPreprocessor::expandMacrosInLine(const std::string& line) {
  std::string expanded;
  expanded.reserve(line.size());
  size_t i = 0;
  while (i < line.size()) {
    if (line[i] == '"' && !inBlockComment_) {
      // Copy string literals as-is.
      size_t start = i++;
      while (i < line.size() && line[i] != '"') {
        if (line[i] == '\\' && i + 1 < line.size()) {
          i += 2;
        } else {
          ++i;
        }
      }
      if (i < line.size()) {
        ++i;
      }
      expanded.append(line.substr(start, i - start));
      continue;
    }

    if (!inBlockComment_ && i + 1 < line.size() &&
        line[i] == '/' && line[i + 1] == '/') {
      expanded.append(line.substr(i));
      break;
    }

    if (i + 1 < line.size() && line[i] == '/' && line[i + 1] == '*') {
      inBlockComment_ = true;
      expanded.append("/*");
      i += 2;
      continue;
    }

    if (inBlockComment_) {
      if (i + 1 < line.size() && line[i] == '*' && line[i + 1] == '/') {
        inBlockComment_ = false;
        expanded.append("*/");
        i += 2;
      } else {
        expanded.push_back(line[i]);
        ++i;
      }
      continue;
    }

    if (line[i] == '`') {
      size_t start = i + 1;
      size_t j = start;
      if (j < line.size() && isIdentifierStart(line[j])) {
        ++j;
        while (j < line.size() && isIdentifierChar(line[j])) {
          ++j;
        }
        std::string name = line.substr(start, j - start);
        auto it = macros_.find(name);
        if (it != macros_.end()) {
          expanded.append(it->second);
        } else {
          expanded.append(line.substr(i, j - i));
        }
        i = j;
        continue;
      }
    }

    expanded.push_back(line[i]);
    ++i;
  }
  return expanded;
}

}} // namespace verilog // namespace naja
