#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <stdio.h>

using namespace std;

int main() {
  bool running = true;
  string input;
  while (running) {
    cout << "> ";
    cin >> input;
    if (input == "quit") {
      running = false;
      break;
    }
    char *end;
    errno = 0;
    int64_t numeric_input = strtoll(input.c_str(), &end, 10);
    if (errno == ERANGE) {
      cout << "Number is out of range." << endl;
      continue;
    }
    if (end == input.c_str()) {
      cout << "Number cannot be parsed." << endl;
      continue;
    }

    if (numeric_input > 0) {
      cout << "Number is positive." << endl;
    } else if (numeric_input < 0) {
      cout << "Number is negative." << endl;
    } else {
      cout << "Number is zero." << endl;
    }

    if (numeric_input % 2 == 0) {
      cout << "Number is even." << endl;
    } else {
      cout << "Number is odd." << endl;
    }

    cout << "Factors of " << numeric_input << ": ";
    for (int y = -numeric_input; y <= numeric_input; y++) {
      for (int x = -numeric_input; x <= numeric_input; x++) {
        if (x * y == numeric_input) {
          cout << x << " * " << y << ", ";
        }
      }
    }

    cout << endl << endl;
  }
  return 0;
}
