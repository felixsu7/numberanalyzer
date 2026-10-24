#include <analysis.hpp>
#include <cassert>
#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

static const std::string endl = "\n";
static std::stringstream cout = std::stringstream();

// The order of definitions was based on when they were submitted

// It should be noted that I modified their submissions, mainly on checks, the
// mathematical basis is kept exactly as they intended.

// Originally, this accepted two inputs.
// However, I turned it into analysis of what numbers does it divide cleanly
// with. Checking from 1 to N.
void num_divisbility(int64_t num) {
  if (num == 0) {
    cout << "Zero divided by any number is always zero." << endl;
    return;
  }

  cout << num << " is divisible by ";
  for (int64_t d = 1; d <= abs(num); d++) {
    if (num % d == 0) {
      cout << d << ", ";
    }
  }

  cout << endl;
}

void num_sign(int64_t num) {
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

void num_prime_factors(int64_t num) {
  auto prime_factors = _primefactors(num);

  // TODO real ba?
  if (prime_factors.empty()) {
    cout << num << " has no prime factors." << endl;
    return;
  }

  for (auto pf : prime_factors) {
    cout << pf << ", ";
  }

  cout << endl;
}

void num_perfect_square_cube(int64_t num) {
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

void num_even_odd(int64_t num) {
  if (num % 2 == 0) {
    cout << num << " is an even number." << endl;
  } else {
    cout << num << " is an odd number." << endl;
  }
}

void num_factors_sum(int64_t num) {
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

void num_factors_amount(int64_t num) {
  int64_t factorCount = 0;

  if (num == 0) {
    cout << "Zero has infinitely many factors.";
  } else {
    for (int divisor = 1; divisor <= abs(num); divisor++) {
      if (num % divisor == 0) {
        factorCount += 2;
      }
    }

    cout << num << " has " << factorCount << " factor(s)." << endl;
  }
}

void num_prime_composite(int64_t num) {
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

void num_collatz(int64_t num) {
  int64_t Colture = num;

  // I added this check because it is infinitely looping on negative and zeroes.
  if (num <= 0) {
    cout << "ERROR: Input cannot be negative or zero." << endl;
    return;
  }

  // Without limiting to N iterations, this would just lag the entire program.
  const int COLLATZ_ITERATIONS_LIMIT = 100;
  int iteration = 0;

  while (Colture != 1 && iteration < COLLATZ_ITERATIONS_LIMIT) {
    cout << Colture << ", ";

    if (Colture % 2 == 0) {
      Colture = Colture / 2;
    } else {
      Colture = (Colture * 3) + 1;
    }

    iteration++;
  }

  if (iteration >= COLLATZ_ITERATIONS_LIMIT) {
    cout << "..., ";
  }

  cout << 1 << endl;
}

void num_perfect(int64_t num) {
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

std::string num_suffix(int64_t num) {
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

void num_nth_triangular(int64_t num) {
  if (num < 0) {
    cout << "ERROR: Input cannot be negative." << endl;
    return;
  }

  // Triangular number: n(n+1)/2
  // TODO: change th to proper suffix
  cout << num << num_suffix(num) << " triangular number is "
       << (num * (num + 1)) / 2 << endl;
}

void num_is_triangular(int64_t num) {
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

void num_factors(int64_t num) { cout << "NOT IMPLEMENTED YET." << endl; }

void dump_logs(const std::string name, string_vec_map *logs) {
  std::string line;
  while (std::getline(cout, line, '\n')) {
    (*logs)[name].push_back(line);
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
    dump_logs(name, &analysis.logs);
  }

  // TODO: process errors

  return analysis;
}
