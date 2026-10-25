#pragma once

#include "evaluator.hpp"
#include <string>
#include <utility>
#include <vector>

bool fits_long(double d);
unsigned long uabs(long x);

bool is_num(char c);
bool is_alpha(char c);
bool is_whitespace(char c);
bool is_alphanum(char c);

// Trims whitespace from start and end of s, and just erases every unicode
std::string trim_string(const std::string s);
std::string lower_case_string(const std::string s);
std::pair<std::string, std::string> consume_front_string(const std::string s);

// Returns the index into items of the closest match to the key.
// Returns -1 if not found, or -2 if found two or more matches.
int smart_find_index(const std::vector<std::string> items,
                     const std::string key);

bool is_eval_error(EvaluationResult res);
bool handle_eval_error(EvaluationResult *res, bool report_expr);
