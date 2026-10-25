#include "analysis.hpp"
#include "commands.hpp"
#include "evaluator.hpp"
#include "styling.hpp"
#include "utils.hpp"
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

static const std::string endl = "\n";
static std::stringstream cout = std::stringstream();

static uint ITERATION_LIMIT = 100;

void set_iterations_limit(uint n) { ITERATION_LIMIT = n; }

uint get_iteration_limit() { return ITERATION_LIMIT; }

// this is just horrible practice in general.
static std::vector<int64_t> SECONDARY_INPUTS;

void set_secondary_inputs(const std::vector<int64_t> v) {
  SECONDARY_INPUTS = v;
}

// The order of definitions was based on when they were submitted

// It should be noted that I modified their submissions, mainly on checks, the
// mathematical basis is kept exactly as they intended.

// Originally, this accepted two inputs.
// However, I turned it into analysis of what numbers does it divide cleanly
// with. Checking from 1 to N.
// UPDATE: special yarn?
void analysis_divisibility(int64_t num) {
  if (num == 0) {
    cout << "Zero divided by any number is always zero." << endl;
    return;
  };

  if (SECONDARY_INPUTS.empty()) {
    cout << "ERROR: No selected integers set." << endl;
    return;
  }

  bool at_least_one = false;

  cout << num << " is divisible by ";
  for (auto d : SECONDARY_INPUTS) {
    if (d == 0)
      continue;
    if (num % d == 0) {
      at_least_one = true;
      cout << d << ", ";
    }
  }

  if (!at_least_one)
    cout << "none of the selected integers.";

  cout << endl;
}

void analysis_sign(int64_t num) {
  if (num < 0) {
    cout << num << " is negative." << endl;
  } else if (num == 0) {
    cout << num << " is zero." << endl;
  } else {
    cout << num << " is positive." << endl;
  }
}

// Submitted to me in this form, making a wrapper wasn't difficult anyways.
std::vector<int64_t> _primefactors(int64_t n) {
  std::vector<int64_t> factors;
  if (n < 2)
    return factors;

  // step 1, divide n to 2 while n is even
  while (n % 2 == 0) {
    factors.push_back(2);
    n /= 2;
  }
  // step 2, iterate through odd numbers starting from 3 to the quotient of n
  // and i, then check if it is divisible to i, then divide it to i
  int i = 3;
  while (i <= n / i) {
    if (n % i == 0) {
      factors.push_back(i);
      n /= i;
    } else {
      i += 2;
    }
  }
  // step 3, if the remaining number is more than 1, it is another factor of the
  // number
  if (n > 1) {
    factors.push_back(n);
  }
  return factors;
}

void analysis_prime_factors(int64_t num) {
  auto prime_factors = _primefactors(num);

  if (prime_factors.empty()) {
    cout << num << " has no prime factors." << endl;
    return;
  }

  std::vector<int64_t> deduped_pfs;
  for (auto pf : prime_factors) {
    if (std::find(deduped_pfs.begin(), deduped_pfs.end(), pf) ==
        deduped_pfs.end())
      deduped_pfs.push_back(pf);
  }

  for (auto pf : deduped_pfs) {
    cout << pf << ", ";
  }

  cout << endl;
}

void analysis_perfect_square_cube(int64_t num) {
  // I added this check.
  if (num < 0) {
    cout << "ERROR: Input cannot be negative." << endl;
    return;
  }

  int64_t iskwerot = static_cast<int64_t>(round(sqrt(num)));
  int64_t kyubrot = static_cast<int64_t>(round(cbrt(num)));

  if (num >= 0 && iskwerot * iskwerot == num) {
    cout << num << " is a perfect square." << endl;
  } else {
    cout << num << " is not a perfect square." << endl;
  }

  if (kyubrot * kyubrot * kyubrot == num) {
    cout << num << " is a perfect cube." << endl;
  } else {
    cout << num << " is not a perfect cube." << endl;
  }
}

void analysis_even_odd(int64_t num) {
  if (num % 2 == 0) {
    cout << num << " is an even number." << endl;
  } else {
    cout << num << " is an odd number." << endl;
  }
}

void analysis_factors_sum(int64_t num) {
  int64_t sum = 0;

  if (num < 0) {
    cout << "ERROR: Input cannot be negative." << endl;
    return;
  }

  for (int64_t x = 1; x <= num; x++) {
    if (num % x == 0) {
      // cout << x << ", ";
      sum += x;
    }
  }
  cout << num << " has sum of positive factors of " << sum << "." << endl;
}

void analysis_factors_amount(int64_t num) {
  int64_t factorCount = 0;

  if (num == 0) {
    cout << "Zero has infinitely many factors.";
  } else {
    for (int divisor = 1; divisor <= abs(num); divisor++) {
      if (num % divisor == 0) {
        factorCount += 1;
      }
    }

    cout << num << " has " << factorCount << " factor(s)." << endl;
  }
}

void analysis_prime_composite(int64_t num) {
  int64_t divisor = 0;

  // How about 1, 0, and the negatives?
  if (num <= 0) {
    cout << "ERROR: " << num << " is not a natural number (1+).";
  } else if (num <= 1) {
    cout << "One is neither prime nor composite.";
  } else {
    for (divisor = 2; divisor < num; divisor++) {
      if (num % divisor == 0) {
        break;
      }
    }

    if (divisor == num) {
      cout << num << " is a prime number.";
    } else {
      cout << num << " is a composite number.";
    }
  }
}

void analysis_collatz(int64_t num) {
  int64_t Colture = num;

  // I added this check because it is infinitely looping on negative and zeroes.
  if (num <= 0) {
    cout << "ERROR: Input cannot be negative or zero." << endl;
    return;
  }

  uint iteration = 0;

  while (Colture != 1 && iteration < ITERATION_LIMIT) {
    cout << Colture << ", ";

    if (Colture % 2 == 0) {
      Colture = Colture / 2;
    } else {
      Colture = (Colture * 3) + 1;
    }

    iteration++;
  }

  if (iteration >= ITERATION_LIMIT) {
    cout << "..., ";
  }

  cout << 1 << endl;
}

void analysis_perfect(int64_t num) {
  // Numbers <= 1 cannot be perfect numbers (the smallest perfect number is 6)
  if (num <= 1) {
    cout << num << " is not a perfect number." << endl;
    return;
  }

  int64_t divisor_sum = 1; // 1 is always a proper divisor
  // Stop at sqrt to avoid checking duplicate divisor pairs
  for (int64_t i = 2; i <= std::sqrt(num); i++) {
    if (num % i == 0) {
      divisor_sum += i; // Add smaller divisor
      if (i != num / i) {
        divisor_sum += num / i; // Add matching larger divisor pair
      }
    }
  }

  if (divisor_sum == num) {
    cout << num << " is a perfect number." << endl;
  } else {
    cout << num << " is not a perfect number" << endl;
  }
}

std::string analysis_suffix(int64_t num) {
  auto ones = num % 10;
  if (ones == 1) {
    return "st";
  } else if (ones == 2) {
    return "nd";
  } else if (ones == 3) {
    return "rd";
  } else {
    return "th";
  }
}

void analysis_nth_triangular(int64_t num) {
  if (num < 0) {
    cout << "ERROR: Input cannot be negative." << endl;
    return;
  }

  // Triangular number: n(n+1)/2
  cout << num << analysis_suffix(num) << " triangular number is "
       << (num * (num + 1)) / 2 << "." << endl;
}

void analysis_is_triangular(int64_t num) {
  if (num < 0) {
    cout << num << " is not a triangular number." << endl;
    return;
  }

  // A number is a triangular number if 8x + 1 is a perfect square.
  int64_t val = 8 * num + 1;
  int64_t root = static_cast<int64_t>(std::sqrt(val));

  if (root * root == val) {
    cout << num << " is a triangular number." << endl;
  } else {
    cout << num << " is not a triangular number." << endl;
  }
}

void analysis_factors(int64_t num) {
  if (num == 0) {
    cout << "Zero has infinite amount of factors." << endl;
    return;
  }

  std::vector<int64_t> factors;
  auto absolute = uabs(num);

  for (uint64_t x = 1; x <= absolute; x++) {
    if (absolute % x == 0) {
      cout << x << ", ";
    }
  }
}

std::string clean_line(const std::string s) {
  auto trimmed = trim_string(s);
  if (trimmed.back() == ',')
    trimmed.pop_back();
  return trimmed;
}

void dump_logs(const std::string name, string_vec_map *logs,
               string_vec_map *errors) {
  const std::string ERROR_MARKER = "ERROR: ";
  std::string line;

  while (std::getline(cout, line, '\n')) {
    auto res =
        std::mismatch(ERROR_MARKER.begin(), ERROR_MARKER.end(), line.begin());
    // prefixed with ERROR:
    if (res.first == ERROR_MARKER.end()) {
      (*errors)[name].push_back(clean_line(line.erase(0, ERROR_MARKER.size())));
    } else {
      (*logs)[name].push_back(clean_line(line));
    }
  }

  cout.str("");
  cout.clear();
}

NumberAnalysis analyze_number(int64_t num,
                              const std::map<std::string, bool> enabled) {
  cout.str("");
  cout.clear();
  auto analysis = NumberAnalysis{};
  analysis.num = num;

  for (auto [name, func] : analysis_funcs) {
    // Skip functions that are not enabled.
    if (auto it = enabled.find(name); it != enabled.end() && !it->second)
      continue;
    func(num);
    dump_logs(name, &analysis.logs, &analysis.errors);
  }

  return analysis;
}

// idk whats a better place to place this
// just bad code all around
void handle_secondary_input_dialogue(ProgramState *state) {
  set_secondary_inputs({});
  if (!state->enabled_analysis["Divisibility"])
    return;

  std::cout
      << std::endl
      << style_info(
             "Since the Divisibility analysis function is enabled, please "
             "input "
             "selected integers to be tested against; separated by semicolons "
             "(';'). "
             "Type in exactly "
             "'q' to not input anything.")
      << std::endl;

  while (true) {
    std::string user_input;
    std::cout << style_input_secondary("> ");
    if (is_ansi_enabled())
      std::cout << "\033[38;5;34m" << std::flush;
    // std::in immediately returns on space
    std::getline(std::cin, user_input);
    if (is_ansi_enabled())
      std::cout << RESET_STYLE << std::flush;

    user_input = lower_case_string(trim_string(user_input));
    if (user_input.empty())
      continue;
    if (user_input == "q") {
      std::cout << style_output_primary("No selected integers set, continuing.")
                << std::endl;
      break;
    }

    std::vector<int64_t> collected;
    std::string expr;
    while (!user_input.empty() || !expr.empty()) {
      while (!user_input.empty() && user_input[0] != ';') {
        expr.push_back(user_input[0]);
        user_input.erase(0, 1);
      }
      if (!user_input.empty() && user_input[0] == ';') {
        user_input.erase(0, 1);
      }

      auto res = evaluate_expression(expr, state->user_constants);
      expr = "";
      if (handle_eval_error(&res, true))
        continue;

      auto evaled = std::get<double>(res);
      if (!fits_long(evaled)) {
        std::cout << style_error("ERROR: " + std::to_string(evaled) +
                                 " is too large to be analyzed!")
                  << std::endl;
        continue;
      }
      auto as_int = static_cast<int64_t>(evaled);

      collected.push_back(as_int);
    }

    if (collected.empty()) {
      std::cout << style_error("ERROR: No parsable inputs, try again or 'q' to "
                               "stop immediately.")
                << std::endl;
      continue;
    } else {
      std::cout << style_info("Selected Integers:") << " ";
      bool first = true;
      for (auto si : collected) {
        if (!first)
          std::cout << ", ";
        first = false;
        std::cout << style_input_secondary(std::to_string(si));
      }
      std::cout << std::endl;
    }

    state->secondary_inputs = collected;
    set_secondary_inputs(state->secondary_inputs);
    break;
  }
};
