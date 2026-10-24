#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

using string_vec_map = std::map<std::string, std::vector<std::string>>;

using math_function = void(int64_t num1, int64_t num2);

struct NumberAnalysis {
  int64_t num1;
  int64_t num2;
  string_vec_map logs;
  string_vec_map errors;
};

math_function num_sign;
math_function num_even_odd;
math_function num_divisbility;
math_function num_factors;
math_function num_factors_amount;
math_function num_prime_composite;
math_function num_factors_sum;
math_function num_perfect;
math_function num_collatz;
math_function num_prime_factors;
math_function num_perfect_square_cube;
math_function num_is_triangular;
// I guess this is also considered as 12. Triangular Number
math_function num_nth_triangular;

static const std::map<std::string, math_function *> math_funcs_map = {
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

NumberAnalysis analyze_number(int64_t num1, int64_t num2);
