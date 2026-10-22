#include "screen.hpp"
#include <cerrno>
#include <cstdlib>
#include <random>
#include <stdio.h>
#include <thread>

using namespace std;

int main() {
  Screen screen(50, 50);
  Cell c = default_cell;
  c.glyph = '*';
  c.foreground = {255, 0, 0, false};
  c.background = {255, 0, 0, false};
  int running = 0;
  int px = 0;
  int py = 0;
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<int64_t> dist(-10000, 10000);
  std::uniform_int_distribution<int64_t> distd(-5, 5);
  screen.activate();
  while (running < 60) {
    c.foreground.red += distd(gen) * 10;
    c.foreground.green += distd(gen) * 10;
    c.foreground.blue += distd(gen) * 10;
    c.background.red += distd(gen) * 10;
    c.background.green += distd(gen) * 10;
    c.background.blue += distd(gen) * 10;
    screen.draw_rect(10 + distd(gen), 10 + distd(gen), 5 + distd(gen),
                     6 + distd(gen), c);
    screen.draw_text(10 + distd(gen), 10 + distd(gen), "Hello!!", c);
    int d = distd(gen);
    if (dist(gen) % 2 == 0) {
      px += d;
    } else {
      py += d;
    }
    if (px < 0)
      px = 0;
    if (py < 0)
      py = 0;
    screen.present();
    running++;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
  screen.deactivate();
  // bool running = true;
  // string input;
  //
  // while (running) {
  //   cout << "> ";
  //   cin >> input;
  //   if (input == "quit") {
  //     running = false;
  //     break;
  //   }
  //   char *end;
  //   errno = 0;
  //   int64_t numeric_input = strtoll(input.c_str(), &end, 10);
  //   if (errno == ERANGE) {
  //     cout << "Number is out of range." << endl;
  //     continue;
  //   }
  //   if (end == input.c_str()) {
  //     cout << "Number cannot be parsed." << endl;
  //     continue;
  //   }
  //
  //   if (numeric_input > 0) {
  //     cout << "Number is positive." << endl;
  //   } else if (numeric_input < 0) {
  //     cout << "Number is negative." << endl;
  //   } else {
  //     cout << "Number is zero." << endl;
  //   }
  //
  //   if (numeric_input % 2 == 0) {
  //     cout << "Number is even." << endl;
  //   } else {
  //     cout << "Number is odd." << endl;
  //   }
  //
  //   cout << "Factors of " << numeric_input << ": ";
  //   for (int y = -numeric_input; y <= numeric_input; y++) {
  //     for (int x = -numeric_input; x <= numeric_input; x++) {
  //       if (x * y == numeric_input) {
  //         cout << x << " * " << y << ", ";
  //       }
  //     }
  //   }
  //
  //   cout << endl << endl;
  // }
  return 0;
}
