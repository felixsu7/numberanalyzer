#include "utils.hpp"
#include "evaluator.hpp"
#include "styling.hpp"
#include <algorithm>
#include <iostream>

bool fits_long(double d) {
  constexpr double lo = static_cast<double>(std::numeric_limits<long>::min());
  constexpr double hi = -lo;
  return d >= lo && d < hi;
}
unsigned long uabs(long x) {
  return x < 0 ? 0 - static_cast<unsigned long>(x)
               : static_cast<unsigned long>(x);
}

bool is_num(char c) { return c >= '0' && c <= '9'; }
bool is_alpha(char c) {
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}
bool is_whitespace(char c) { return (c == ' ') || (c == '\n') || (c == '\t'); }
bool is_alphanum(char c) { return is_num(c) || is_alpha(c); }

std::string trim_string(const std::string s) {
  std::string res = s;
  while (!res.empty() && is_whitespace(res.front()))
    res.erase(0, 1);
  while (!res.empty() && is_whitespace(res.back()))
    res.erase(res.size() - 1, 1);
  // Unicode content has their most significant bit set.
  for (size_t i = 0; i < res.size(); i++) {
    if (res[i] & 0x80)
      res.replace(i, 1, "?");
  }
  return res;
}

std::string lower_case_string(const std::string s) {
  std::string res = s;
  std::transform(res.begin(), res.end(), res.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return res;
}

std::pair<std::string, std::string> consume_front_string(const std::string s) {
  std::string remaining = s;
  size_t i = 0;
  while (is_whitespace(remaining[i]))
    i++;
  remaining.erase(0, i);

  i = 0;
  while (is_alphanum(remaining[i]))
    i++;
  auto consumed = remaining.substr(0, i);

  while (is_whitespace(remaining[i]))
    i++;
  remaining.erase(0, i);

  return {consumed, remaining};
}

// Returns the index into items of the closest match to the key.
// Returns -1 if not found, or -2 if found two or more matches.
int smart_find_index(const std::vector<std::string> items,
                     const std::string key) {
  auto transformed_key = lower_case_string(key);
  // Remove every whitespace
  for (size_t i = 0; i < transformed_key.size(); i++) {
    if (is_whitespace(transformed_key[i]))
      transformed_key.erase(i, 1);
  }
  auto transformed_items = items;
  // Here too.
  std::transform(transformed_items.begin(), transformed_items.end(),
                 transformed_items.begin(), [](const std::string s) {
                   std::string res = s;
                   for (size_t i = 0; i < res.size(); i++) {
                     if (is_whitespace(res[i]))
                       res.erase(i, 1);
                   }
                   return lower_case_string(res);
                 });

  std::string current_key;
  size_t key_idx = 0;
  int match_idx = -1;
  bool match = false;

  while (!match && key_idx < key.size()) {
    current_key.push_back(transformed_key[key_idx++]);
    match_idx = -1;
    match = false;

    for (size_t i = 0; i < transformed_items.size(); i++) {
      // is current_key a prefix of i?
      auto res = std::mismatch(current_key.begin(), current_key.end(),
                               transformed_items[i].begin());

      if (res.first == current_key.end()) {
        // first match of current key
        if (!match) {
          match = true;
          match_idx = static_cast<int>(i);
        } else {
          // second+ match, ambigious
          match = false;
          match_idx = -2;
          break;
        }
      }
    }
  }

  return match_idx;
}

bool is_error(EvaluationResult res) {
  return std::holds_alternative<EvaluationError>(res);
}

bool handle_error(EvaluationResult *res) {
  if (auto err = std::get_if<EvaluationError>(res)) {
    std::cout << style_error("ERROR: " + err->message + " (" + err->offending +
                             ") at position " + std::to_string(err->pos) + ".")
              << std::endl;
    return true;
  }
  return false;
}
