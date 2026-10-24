#pragma once

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

AnalysisFuncDef num_sign;
AnalysisFuncDef num_even_odd;
AnalysisFuncDef num_divisbility;
AnalysisFuncDef num_factors;
AnalysisFuncDef num_factors_amount;
AnalysisFuncDef num_prime_composite;
AnalysisFuncDef num_factors_sum;
AnalysisFuncDef num_perfect;
AnalysisFuncDef num_collatz;
AnalysisFuncDef num_prime_factors;
AnalysisFuncDef num_perfect_square_cube;
AnalysisFuncDef num_is_triangular;
// I guess this is also considered as 12. Triangular Number
AnalysisFuncDef num_nth_triangular;

static const std::map<std::string, AnalysisFuncDef *> analysis_funcs = {
    {"Positive/Negative/Zero", num_sign},
    {"Even/Odd", num_even_odd},
    {"Divisibility", num_divisbility},
    {"Factors", num_factors},
    {"Number of Factors", num_factors_amount},
    {"Prime or composite", num_prime_composite},
    {"Sum of Factors", num_factors_sum},
    {"Perfect Number", num_perfect},
    {"Collatz Conjecture Sequence", num_collatz},
    {"Prime Factorization", num_prime_factors},
    {"Perfect Square/Cube", num_perfect_square_cube},
    {"Triangular Number", num_is_triangular},
    {"nth Triangular Number", num_nth_triangular},
};

// Theres no ordered maps.
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

NumberAnalysis analyze_number(int64_t num,
                              const std::map<std::string, bool> enabled);
