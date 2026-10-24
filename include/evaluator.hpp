#pragma once
#include <functional>
#include <random>
#include <string>
#include <variant>
#include <vector>

using MFuncArgs = const std::vector<long double> &;

struct MFuncDef {
  size_t arity;
  std::function<long double(MFuncArgs)> func;
};

static const std::unordered_map<std::string, MFuncDef> mfuncs{
    {"sin", {1, [](MFuncArgs a) { return std::sin(a[0]); }}},
    {"cos", {1, [](MFuncArgs a) { return std::cos(a[0]); }}},
    {"tan", {1, [](MFuncArgs a) { return std::tan(a[0]); }}},
    {"csc", {1, [](MFuncArgs a) { return 1.0 / std::sin(a[0]); }}},
    {"sec", {1, [](MFuncArgs a) { return 1.0 / std::cos(a[0]); }}},
    {"cot", {1, [](MFuncArgs a) { return 1.0 / std::tan(a[0]); }}},
    {"asin", {1, [](MFuncArgs a) { return std::asin(a[0]); }}},
    {"acos", {1, [](MFuncArgs a) { return std::acos(a[0]); }}},
    {"atan", {1, [](MFuncArgs a) { return std::atan(a[0]); }}},

    {"sinh", {1, [](MFuncArgs a) { return std::sinh(a[0]); }}},
    {"cosh", {1, [](MFuncArgs a) { return std::cosh(a[0]); }}},
    {"tanh", {1, [](MFuncArgs a) { return std::tanh(a[0]); }}},
    {"asinh", {1, [](MFuncArgs a) { return std::asinh(a[0]); }}},
    {"acosh", {1, [](MFuncArgs a) { return std::acosh(a[0]); }}},
    {"atanh", {1, [](MFuncArgs a) { return std::atanh(a[0]); }}},

    {"sqrt", {1, [](MFuncArgs a) { return std::sqrt(a[0]); }}},
    {"cbrt", {1, [](MFuncArgs a) { return std::cbrt(a[0]); }}},
    {"ln", {1, [](MFuncArgs a) { return std::log(a[0]); }}},
    {"pow", {2, [](MFuncArgs a) { return std::pow(a[0], a[1]); }}},

    {"floor", {1, [](MFuncArgs a) { return std::floor(a[0]); }}},
    {"ceil", {1, [](MFuncArgs a) { return std::ceil(a[0]); }}},
    {"abs", {1, [](MFuncArgs a) { return std::fabs(a[0]); }}},
    {"min", {2, [](MFuncArgs a) { return std::min(a[0], a[1]); }}},
    {"max", {2, [](MFuncArgs a) { return std::max(a[0], a[1]); }}},

    {"rand",
     {0,
      [](MFuncArgs _) {
        std::mt19937_64 rng{std::random_device{}()};
        std::uniform_real_distribution<long double> dist(0.0L, 1.0L);

        return dist(rng);
      }}},
    {"randr",
     {2,
      [](MFuncArgs a) {
        std::mt19937_64 rng{std::random_device{}()};
        std::uniform_real_distribution<long double> dist(a[0], a[1]);

        return dist(rng);
      }}},
    // Not really an int.
    {"randint",
     {2,
      [](MFuncArgs a) {
        std::mt19937_64 rng{std::random_device{}()};
        std::uniform_real_distribution<long double> dist(a[0], a[1]);

        return std::ceil(dist(rng));
      }}},

};

// https://en.wikipedia.org/wiki/List_of_mathematical_constants
static const std::unordered_map<std::string, long double> mconsts{
    {"pi", 3.14159265358979323846},
    {"e", 2.71828182845904523536},
    {"tau", 6.28318530717958647692},
    {"phi", 1.61803398874989484820},
    {"sixseven", 67.67},

};

struct EvaluationError {
  size_t pos;
  std::string message;
  std::string offending;
};

using EvaluationResult = std::variant<long double, EvaluationError>;

EvaluationResult evaluate_expression(std::string expr);
bool is_error(EvaluationResult res);
