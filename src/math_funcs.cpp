#include <cassert>
#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <math_funcs.hpp>
#include <sstream>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <vector>

static std::string mfunc_name;
void math_function_name(const std::string name) { mfunc_name = name; }

static const std::string endl = "\n";
static std::stringstream cout = std::stringstream();

void num_divisbility(int64_t num1, int64_t num2) {
  math_function_name("Divisibility");

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

void num_sign(int64_t num) {
  math_function_name("Positive/Negative/Zero");
  if (num < 0) {
    cout << "The number is negative." << endl;
  } else if (num == 0) {
    cout << "The number is zero." << endl;
  } else {
    cout << "The number is positive." << endl;
  }
}

std::vector<int64_t> primefactors(int64_t n) {
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
  math_function_name("Prime Factors");
  cout << "Prime factors: ";
  auto prime_factors = primefactors(num);
  for (auto pf : prime_factors) {
    cout << pf << " ";
  }
  cout << endl;
  // TODO
}

void num_perfect_square_cube(int64_t num) {
  math_function_name("Perfect Square / Cube");
  cout << "Enter a number: ";

  int64_t iskwerot = round(sqrt(num));
  int64_t kyubrot = round(cbrt(num));

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
  math_function_name("Even/Odd");
  cout << "Enter a number: ";

  if (num % 2 == 0) {
    cout << num << " is an even number." << endl;
  } else {
    cout << num << " is an odd number." << endl;
  }
}

void num_factors_sum(int64_t num) {
  math_function_name("Sum of factors");

  int64_t sum = 0;
  cout << "Factors of " << num << ":\n";

  for (int64_t x = 1; x <= num; x++) {
    if (num % x == 0) {
      cout << x << ", ";
      sum += x;
    }
  }
  cout << "\nSum of the factors: " << sum << endl;
}

void num_factors_amount(int64_t num) {
  math_function_name("Number of Factors");

  int64_t factorCount = 0;

  if (num == 0) {
    cout << "Zero has infinitely many factors.";
  } else {
    for (int divisor = 1; divisor <= abs(num); divisor++) {
      if (num % divisor == 0) {
        factorCount += 2;
      }
    }

    cout << "Number of integer factors: " << factorCount;
  }
}

void num_prime_composite(int64_t num) {
  math_function_name("Prime or Composite Number");

  int64_t divisor = 0;

  // How about 1, 0, and the negatives?
  if (num < 2) {
    cout << "Neither prime nor composite.";
  } else {
    for (divisor = 2; divisor < num; divisor++) {
      if (num % divisor == 0) {
        break;
      }
    }

    if (divisor == num) {
      cout << "It is a prime number";
    } else {
      cout << "It is a composite number";
    }
  }
}

void num_collatz(int64_t num) {
  math_function_name("Collatz Conjecture");
  // Felix's TODO: somehow yield

  int64_t Colture = num;

  cout << "\nCollatz Sequence: ";

  while (Colture != 1) {
    cout << Colture << " ";

    if (Colture % 2 == 0) {
      Colture = Colture / 2;
    }

    else {
      Colture = (Colture * 3) + 1;
    }
  }

  cout << 1 << endl;
}

void num_perfect(int64_t num) {
  math_function_name("Perfect Number Checker");
  // Numbers <= 1 cannot be perfect numbers (the smallest perfect number is 6)
  if (num <= 1) {
    cout << "Number is not a perfect number." << endl;
    return;
  }

  uint64_t divisor_sum = 1; // 1 is always a proper divisor
  // Stop at sqrt to avoid checking duplicate divisor pairs
  for (int64_t i = 2; i <= std::sqrt(num); i++) {
    if (num % i == 0) {
      divisor_sum += i; // Add smaller divisor
      if (i != num / i) {
        divisor_sum += num / i; // Add matching larger divisor pair
      }
    }
  }

  cout << std::boolalpha << (divisor_sum == num) << endl;
}

void num_nth_triangular(int64_t num) {
  math_function_name("nth Triangular Number");
  if (num < 0) {
    cout << "ERROR: Input cannot be negative." << std::endl;
    return;
  }

  // Triangular number: n(n+1)/2
  cout << (num * (num + 1)) / 2 << endl;
}

void num_is_triangular(int64_t num) {
  math_function_name("Triangular Number Checker");
  if (num < 0) {
    cout << "false" << std::endl;
    return;
  }

  // A number is a triangular number if 8x + 1 is a perfect square.
  int64_t val = 8 * num + 1;
  int64_t root = std::sqrt(val);
  cout << std::boolalpha << (root * root == val) << std::endl;
}

void dump_logs(
    std::unordered_map<std::string, std::vector<std::string>> *logs) {
  assert(!mfunc_name.empty());

  std::string line;
  while (std::getline(cout, line, '\n')) {
    (*logs)[mfunc_name].push_back(line);
  }
  mfunc_name = "";
  cout.str("");
  cout.clear();
}

NumberAnalysis analyze_number(int64_t num1, int64_t num2) {
  mfunc_name = "";
  cout.str("");
  cout.clear();
  std::unordered_map<std::string, std::vector<std::string>> logs;

  num_sign(num1);
  dump_logs(&logs);
  num_even_odd(num1);
  dump_logs(&logs);
  // Handle num2
  num_divisbility(num1, num2);
  dump_logs(&logs);
  // num_factors(num1);
  num_factors_amount(num1);
  dump_logs(&logs);
  num_prime_composite(num1);
  dump_logs(&logs);
  num_factors_sum(num1);
  dump_logs(&logs);
  num_perfect(num1);
  dump_logs(&logs);
  num_collatz(num1);
  dump_logs(&logs);
  num_prime_factors(num1);
  dump_logs(&logs);
  num_perfect_square_cube(num1);
  dump_logs(&logs);
  num_is_triangular(num1);
  dump_logs(&logs);
  // I guess this is also considered as 12. Triangular Number
  num_nth_triangular(num1);
  dump_logs(&logs);

  NumberAnalysis analysis = NumberAnalysis{};
  analysis.num1 = num1;
  analysis.num2 = num2;
  analysis.logs = logs;
  // TODO: errors

  return analysis;
}
