/*******************************
 *                             *
 *   Math function template    *
 *  For my fellow FOC members  *
 *                             *
 *******************************/

/* Please enter your details here for documentation purposes sa paper, will
not be published sa github:
// Your name:


Description of the function:



Notes:



*/

#include <cstdint>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

static std::string mfunc_name;
void math_function_name(const std::string name) { mfunc_name = name; }

static const std::string endl = "\r\n";
std::ostringstream cout;

//// BEGIN MATH FUNCTION

// even/odd function given as example.
void math_function(int64_t num) {
  // Pa replace yung Even/Odd sa pangalan ng function niyo, needed
  math_function_name("Even/Odd");

  // now, dito kayo mag code
  if (num % 2 == 0) {
    cout << "The number is even." << endl;
  } else {
    cout << "The number is odd." << endl;
  }

  // error kayo if needed talaga, also ganyan dapat format para marecognize
  // ERROR: <error message follows>
  if (num == 67) {
    cout << "ERROR: Bawal po ang six seven dito." << endl;
  }
}

///// END MATH FUNCTION

int main() {
  bool running = true;
  std::string input;

  // para maget yung function name inside
  math_function(0);
  cout.str("");
  cout.clear();

  std::cout << "The epic number analyzer template by 8-bit group!!!!"
            << std::endl;
  std::cout << "Function: " << mfunc_name << std::endl << std::endl;

  while (running) {
    std::cout << "Enter an integer, or type exactly: \nq) quit \nr) test "
                 "random number \n> ";
    std::cin >> input;

    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<int64_t> dist(-10000, 10000);

    int64_t numeric_input = 0;
    if (input == "q") {
      running = false;
      break;
    } else if (input == "r") {
      numeric_input = dist(gen);
    } else {
      char *end;
      errno = 0;
      numeric_input = strtoll(input.c_str(), &end, 10);
      if (errno == ERANGE) {
        std::cout << "Number is out of range." << std::endl << std::endl;
        continue;
      }
      if (end == input.c_str()) {
        std::cout << "Number cannot be parsed." << std::endl << std::endl;
        continue;
      }
    }

    std::cout << "Input parsed (for sanity purposes): '" << numeric_input << "'"
              << std::endl
              << std::endl;

    math_function(numeric_input);

    std::cout << cout.str();
    cout.str("");
    cout.clear();

    std::cout << std::endl
              << "========================" << std::endl
              << std::endl;
  }
}
