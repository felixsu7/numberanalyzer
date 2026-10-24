#include <cassert>
#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <math_funcs.hpp>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

static const std::string endl = "\n";
static std::stringstream cout = std::stringstream();

// The order of definitions was based on when they were submitted.

void num_divisbility(int64_t num1, int64_t num2) {
  // now, dito kayo mag code
  if (num1 % num2 == 0) {
    cout << num1 << " is divisible by " << num2 << endl;
  } else {
    cout << num1 << " is not divisible by " << num2 << endl;
  }

  if (num2 == 0) {
    cout << "ERROR: Can not divide by zero." << endl;
  };
}

void num_sign(int64_t num1, int64_t _) {

  if (num1 < 0) {
    cout << num1 << " is negative." << endl;
  } else if (num1 == 0) {
    cout << num1 << " is zero." << endl;
  } else {
    cout << num1 << " is positive." << endl;
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

void num_prime_factors(int64_t num1, int64_t _) {
  auto prime_factors = _primefactors(num1);
  // real ba?
  if (prime_factors.empty()) {
    cout << num1 << " has no prime factors." << endl;
    return;
  }

  for (auto pf : prime_factors) {
    cout << pf << ", ";
  }

  cout << endl;
}

void num_perfect_square_cube(int64_t num1, int64_t _) {
  // I added this check.
  if (num1 < 0) {
    cout << "ERROR: Input cannot be negative." << endl;
    return;
  }

  int64_t iskwerot = static_cast<int64_t>(round(sqrt(num1)));
  int64_t kyubrot = static_cast<int64_t>(round(cbrt(num1)));

  if (num1 >= 0 && iskwerot * iskwerot == num1) {
    cout << num1 << " is a perfect square." << endl;
  } else {
    cout << num1 << " is not a perfect square." << endl;
  }

  if (kyubrot * kyubrot * kyubrot == num1) {
    cout << num1 << " is a perfect cube." << endl;
  } else {
    cout << num1 << " is not a perfect cube." << endl;
  }
}

void num_even_odd(int64_t num1, int64_t _) {
  if (num1 % 2 == 0) {
    cout << num1 << " is an even number." << endl;
  } else {
    cout << num1 << " is an odd number." << endl;
  }
}

void num_factors_sum(int64_t num1, int64_t _) {
  int64_t sum = 0;

  for (int64_t x = 1; x <= num1; x++) {
    if (num1 % x == 0) {
      // cout << x << ", ";
      sum += x;
    }
  }
  cout << num1 << " has sum of factors of " << sum << "." << endl;
}

void num_factors_amount(int64_t num1, int64_t _) {
  int64_t factorCount = 0;

  if (num1 == 0) {
    cout << "Zero has infinitely many factors.";
  } else {
    for (int divisor = 1; divisor <= abs(num1); divisor++) {
      if (num1 % divisor == 0) {
        factorCount += 2;
      }
    }

    cout << num1 << " has " << factorCount << " factor(s)." << endl;
  }
}

void num_prime_composite(int64_t num1, int64_t _) {
  int64_t divisor = 0;

  // How about 1, 0, and the negatives?
  if (num1 < 2) {
    cout << num1 << " is neither prime nor composite.";
  } else {
    for (divisor = 2; divisor < num1; divisor++) {
      if (num1 % divisor == 0) {
        break;
      }
    }

    if (divisor == num1) {
      cout << num1 << " is a prime number.";
    } else {
      cout << num1 << " is a composite number.";
    }
  }
}

void num_collatz(int64_t num1, int64_t _) {
  // Felix's TODO: somehow yield due to long running times, potentially.
  int64_t Colture = num1;

  // I added this check because it is infinitely looping on negative and zeroes.
  if (num1 <= 0) {
    cout << "ERROR: Input cannot be negative or zero." << endl;
    return;
  }

  while (Colture != 1) {
    cout << Colture << ", ";

    if (Colture % 2 == 0) {
      Colture = Colture / 2;
    }

    else {
      Colture = (Colture * 3) + 1;
    }
  }

  cout << 1 << endl;
}

void num_perfect(int64_t num1, int64_t _) {
  // Numbers <= 1 cannot be perfect numbers (the smallest perfect number is 6)
  if (num1 <= 1) {
    cout << num1 << " is not a perfect number." << endl;
    return;
  }

  int64_t divisor_sum = 1; // 1 is always a proper divisor
  // Stop at sqrt to avoid checking duplicate divisor pairs
  for (int64_t i = 2; i <= std::sqrt(num1); i++) {
    if (num1 % i == 0) {
      divisor_sum += i; // Add smaller divisor
      if (i != num1 / i) {
        divisor_sum += num1 / i; // Add matching larger divisor pair
      }
    }
  }

  if (divisor_sum == num1) {
    cout << num1 << " is a perfect number." << endl;
  } else {
    cout << num1 << " is not a perfect number" << endl;
  }
}

void num_nth_triangular(int64_t num1, int64_t _) {
  if (num1 < 0) {
    cout << "ERROR: Input cannot be negative." << std::endl;
    return;
  }

  // Triangular number: n(n+1)/2
  // TODO: change th to proper suffix
  cout << num1 << "th triangular number is " << (num1 * (num1 + 1)) / 2 << endl;
}

void num_is_triangular(int64_t num1, int64_t _) {
  if (num1 < 0) {
    cout << num1 << " is not a triangular number." << endl;
    return;
  }

  // A number is a triangular number if 8x + 1 is a perfect square.
  int64_t val = 8 * num1 + 1;
  int64_t root = static_cast<int64_t>(std::sqrt(val));

  if (root * root == val) {
    cout << num1 << " is a triangular number." << endl;
  } else {
    cout << num1 << " is not a triangular number." << endl;
  }
}

void num_factors(int64_t num1, int64_t _) {
  cout << "NOT IMPLEMENTED YET." << endl;
}

void dump_logs(std::string name, string_vec_map *logs) {
  std::string line;
  while (std::getline(cout, line, '\n')) {
    (*logs)[name].push_back(line);
  }

  cout.str("");
  cout.clear();
}

NumberAnalysis analyze_number(int64_t num1, int64_t num2) {
  cout.str("");
  cout.clear();
  auto analysis = NumberAnalysis{};
  analysis.num1 = num1;
  analysis.num2 = num2;

  for (auto [name, func] : math_funcs_map) {
    func(num1, num2);
    dump_logs(name, &analysis.logs);
  }

  // TODO: process errors

  return analysis;
}
