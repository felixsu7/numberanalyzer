#pragma once

#include "commands.hpp"
#include <cstdint>
#include <map>
#include <string>
#include <vector>

using string_vec_map = std::map<std::string, std::vector<std::string>>;

using AnalysisFuncDef = void(int64_t);

struct NumberAnalysis {
  int64_t num;
  string_vec_map logs;
  string_vec_map errors;
};

AnalysisFuncDef analysis_sign;
AnalysisFuncDef analysis_even_odd;
AnalysisFuncDef analysis_divisibility;
AnalysisFuncDef analysis_factors;
AnalysisFuncDef analysis_factors_amount;
AnalysisFuncDef analysis_prime_composite;
AnalysisFuncDef analysis_factors_sum;
AnalysisFuncDef analysis_perfect;
AnalysisFuncDef analysis_collatz;
AnalysisFuncDef analysis_prime_factors;
AnalysisFuncDef analysis_perfect_square_cube;
AnalysisFuncDef analysis_is_triangular;
// I guess this is also considered as 12. Triangular Number
AnalysisFuncDef analysis_nth_triangular;

static const std::map<std::string, AnalysisFuncDef *> analysis_funcs = {
    {"Positive/Negative/Zero", analysis_sign},
    {"Even/Odd", analysis_even_odd},
    {"Divisibility", analysis_divisibility},
    {"Factors", analysis_factors},
    {"Number of Factors", analysis_factors_amount},
    {"Prime or composite", analysis_prime_composite},
    {"Sum of Factors", analysis_factors_sum},
    {"Perfect Number", analysis_perfect},
    {"Collatz Conjecture Sequence", analysis_collatz},
    {"Prime Factorization", analysis_prime_factors},
    {"Perfect Square/Cube", analysis_perfect_square_cube},
    {"Triangular Number", analysis_is_triangular},
    {"nth Triangular Number", analysis_nth_triangular},
};

// Theres no ordered maps, so we depend on this vector.
static const std::vector<std::string> analysis_funcs_order = {
    "Positive/Negative/Zero",
    "Even/Odd",
    "Divisibility",
    "Factors",
    "Number of Factors",
    "Prime or composite",
    "Sum of Factors",
    "Perfect Number",
    "Collatz Conjecture Sequence",
    "Prime Factorization",
    "Perfect Square/Cube",
    "Triangular Number",
    "nth Triangular Number",
};

void set_iterations_limit(uint n);
uint get_iteration_limit();

void set_secondary_inputs(const std::vector<int64_t> v);

NumberAnalysis analyze_number(int64_t num,
                              const std::map<std::string, bool> enabled);

void handle_secondary_input_dialogue(ProgramState *state);
