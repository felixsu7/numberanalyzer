#include "math_funcs.hpp"
#include "platform.hpp"
#include "screen.hpp"
#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <random>
#include <stdio.h>
#include <thread>

PLATFORM_ERROR enter_tui_mode() {
  Screen screen(100, 50);
  Cell c = default_cell;
  int running = 0;
  int px = 0;
  int py = 0;
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<int64_t> dist(-10000, 10000);
  std::uniform_int_distribution<int64_t> distd(-5, 5);
  std::uniform_int_distribution<int64_t> distpos(0, 50);

  screen.activate();
  c.glyph = '0';
  c.fg.active = true;
  c.bg.active = true;
  while (running < 200) {
    for (int i = 0; i < 5; i++) {
      c.fg.red += distd(gen) * 3;
      c.fg.green += distd(gen) * 3;
      c.fg.blue += distd(gen) * 3;
      c.bg.red += distd(gen) * 3;
      c.bg.green += distd(gen) * 3;
      c.bg.blue += distd(gen) * 3;

      screen.draw_rect(distpos(gen), distpos(gen), 3, 3, c);
    }
    screen.present();
    running++;
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
  }
  return screen.deactivate();
  return PLATFORM_ERROR::NONE;
}

int main() {
  enter_tui_mode();
  // bool running = true;
  // std::string input;
  //
  // while (running) {
  //   std::cout << "> ";
  //   std::cin >> input;
  //   if (input == "quit") {
  //     running = false;
  //     break;
  //   } else if (input == "tui") {
  //     enter_tui_mode();
  //   }
  //   char *end;
  //   errno = 0;
  //   int64_t numeric_input = strtoll(input.c_str(), &end, 10);
  //   if (errno == ERANGE) {
  //     std::cout << "Number is out of range." << std::endl;
  //     continue;
  //   }
  //   if (end == input.c_str()) {
  //     std::cout << "Number cannot be parsed." << std::endl;
  //     continue;
  //   }
  //
  //   // TODO: num2
  //   const NumberAnalysis analysis = analyze_number(numeric_input, 1);
  //
  //   for (auto [f, v] : analysis.logs) {
  //     std::cout << f << ":" << std::endl;
  //     for (auto l : v) {
  //       std::cout << "    " << l << std::endl;
  //     }
  //   }
  //
  //   std::cout << std::endl << std::endl;
  // }
  //
  return 0;
}
