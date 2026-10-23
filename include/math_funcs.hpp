#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

struct NumberAnalysis {
  int64_t num1;
  int64_t num2;
  std::unordered_map<std::string, std::vector<std::string>> logs;
  std::unordered_map<std::string, std::vector<std::string>> errors;
};

NumberAnalysis analyze_number(int64_t num1, int64_t num2);
