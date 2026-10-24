#include "math_funcs.hpp"
#include "platform.hpp"
#include "screen.hpp"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <random>
#include <stdio.h>
#include <thread>

PLATFORM_ERROR enter_tui_mode() {
  // TODO: query screen size
  Screen screen(100, 50);
  screen.activate();
  Cell c = default_cell;
  bool running = true;
  while (running) {
    PlatformEvent ev = screen.event_poll(100);
    c.bg.active = true;
    c.bg.red = 127;
    c.bg.green = 127;
    c.bg.blue = 127;
    screen.draw_rect(0, 0, 100, 50, c);
    screen.present();
    if (ev.glyph == 'q') {
      running = false;
      break;
    }
  }
  return screen.deactivate();
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
