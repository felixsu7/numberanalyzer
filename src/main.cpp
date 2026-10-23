#include "math_funcs.hpp"
#include "screen.hpp"
#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <random>
#include <stdio.h>
#include <thread>

int tui_mode() {
  Screen screen(50, 50);
  Cell c = default_cell;
  c.glyph = '*';
  c.attrs = static_cast<CellAttr>(ATTR_BOLD | ATTR_UNDERLINE | ATTR_ITALIC);
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
}

int main() {
  bool running = true;
  std::string input;

  while (running) {
    std::cout << "> ";
    std::cin >> input;
    if (input == "quit") {
      running = false;
      break;
    }
    char *end;
    errno = 0;
    int64_t numeric_input = strtoll(input.c_str(), &end, 10);
    if (errno == ERANGE) {
      std::cout << "Number is out of range." << std::endl;
      continue;
    }
    if (end == input.c_str()) {
      std::cout << "Number cannot be parsed." << std::endl;
      continue;
    }

    // TODO: num2
    const NumberAnalysis analysis = analyze_number(numeric_input, 1);

    for (auto [f, v] : analysis.logs) {
      std::cout << f << ":" << std::endl;
      for (auto l : v) {
        std::cout << "    " << l << std::endl;
      }
    }

    std::cout << std::endl << std::endl;
  }
  return 0;
}
