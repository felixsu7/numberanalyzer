#include "evaluator.hpp"
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <variant>
#include <vector>

// Based on https://nicolovaligi.com/articles/precedence-climbing
// Also https://craftinginterpreters.com/parsing-expressions.html
// Instead of an AST though, we immediately evaluate.
// Also, it's a long double first since trig functions would otherwise just
// return 0 almost always.

static bool is_num(char c) { return c >= '0' && c <= '9'; }
static bool is_alpha(char c) {
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}
static bool is_whitespace(char c) {
  return (c == ' ') || (c == '\n') || (c == '\t');
}
static bool is_alphanum(char c) { return is_num(c) || is_alpha(c); }

int prec(char c) {
  switch (c) {
  case '+':
  case '-':
    return 1;
  case '*':
  case '/':
    return 2;
  case '^':
    return 4;
  default:
    return 0;
  }
}

bool is_error(EvaluationResult res) {
  return std::holds_alternative<EvaluationError>(res);
}

class Evaluator {
  std::string source;
  size_t pos = 0;

  EvaluationResult expression(int min) {
    auto left_res = primary();
    if (is_error(left_res))
      return left_res;
    auto left = std::get<long double>(left_res);

    while (true) {
      char op = peek();
      int pr = prec(op);
      if (pr < min)
        return left;
      size_t op_pos = pos++;

      // Exponentation is the only right-associative operator we have, so
      // ternary is fine.
      auto right_res = expression(op == '^' ? pr : pr + 1);
      if (is_error(right_res))
        return right_res;
      auto right = std::get<long double>(right_res);

      switch (op) {
      case '+':
        left += right;
        break;
      case '-':
        left -= right;
        break;
      case '*':
        left *= right;
        break;
      case '/':
        if (right == 0)
          return EvaluationError{op_pos, "Division by zero", "0"};
        left /= right;
        break;
      case '^':
        left = std::pow(left, right);
        break;
      }
    }
  }

  EvaluationResult primary() {
    char c = peek();

    if (c == '(') {
      pos++;
      auto sub_res = expression(1);
      if (is_error(sub_res))
        return sub_res;

      if (!consume(')')) {
        return EvaluationError{pos, "Missing ending parenthesis", ")"};
      };

      return std::get<long double>(sub_res);
    }

    if (is_alpha(c)) {
      size_t start = pos;
      while (is_alphanum(source[pos]))
        pos++;
      std::string id = source.substr(start, pos - start);

      if (consume('(')) {
        // Functions
        std::vector<long double> args;
        if (peek() != ')')
          do {
            auto arg_res = expression(1);
            if (is_error(arg_res))
              return arg_res;
            args.push_back(std::get<long double>(arg_res));
          } while (peek() == ',' && pos++);
        if (!consume(')')) {
          return EvaluationError{pos, "Missing ending parenthesis", ")"};
        };

        auto it = math_funcs.find(id);
        if (it == math_funcs.end())
          return EvaluationError{start, "Unknown function", id};

        if (args.size() != it->second.arity)
          return EvaluationError{
              start, "Function " + id + " got wrong amount of arguments",
              std::to_string(args.size())};

        long double val = it->second.func(args);
        if (!std::isfinite(val))
          return EvaluationError{start, "Function domain error", id};

        return val;
      } else {
        // Constants
        // TODO take in a map of consts
        auto it = user_constants.find(id);
        if (it == user_constants.end())
          return EvaluationError{start, "Unknown constant", id};
        return it->second;
      }
    }

    if (consume('-')) {
      auto neg_res = expression(4);
      if (is_error(neg_res))
        return neg_res;
      return -std::get<long double>(neg_res);
    }

    // Integers, decimals, and somehow hexidecimals for free.
    char *end;
    long double decimal = std::strtold(source.c_str() + pos, &end);
    if (end == source.c_str() + pos)
      return EvaluationError{pos, "Expected number", ""};

    // Moves to position after last digit
    pos = static_cast<size_t>(end - source.c_str());
    return decimal;
  }

  char peek() {
    // Whitespace is meaningless always, so ignore it here.
    while (is_whitespace(source[pos]))
      pos++;
    return source[pos];
  }

  bool consume(char c) {
    if (peek() == c) {
      pos++;
      return true;
    }
    return false;
  }

public:
  std::map<std::string, long double> user_constants;
  EvaluationResult evaluate_expression(std::string expr) {
    source = expr;
    pos = 0;
    auto res = expression(1);

    // Must consume all.
    if (peek() != '\0')
      return EvaluationError{pos, "Unexpected character",
                             std::string(1, source[pos])};

    return res;
  }
};

EvaluationResult
evaluate_expression(std::string expr,
                    std::map<std::string, long double> user_constants) {
  Evaluator p;
  p.user_constants = user_constants;
  return p.evaluate_expression(expr);
}
