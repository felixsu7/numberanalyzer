#include "evaluator.hpp"
#include "math_funcs.hpp"
#include "platform.hpp"
#include "screen.hpp"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdio.h>

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
    if (ev.key == 'q') {
      running = false;
      break;
    }
  }
  return screen.deactivate();
}

int main() {
  // enter_tui_mode();
  bool running = true;
  std::string input;

  while (running) {
    std::cout << "> ";
    std::getline(std::cin, input);
    if (input == "quit") {
      running = false;
      break;
    } else if (input == "tui") {
      enter_tui_mode();
    }
    auto evaluation = evaluate_expression(input);
    if (std::holds_alternative<EvaluationError>(evaluation)) {
      auto err = std::get<EvaluationError>(evaluation);

      std::cout << "EVAL ERROR: " << err.message << " (" << err.offending
                << ") at position " << err.pos << std::endl;

      continue;
    }

    auto decimal_input = std::get<long double>(evaluation);
    std::cout << "Evaluator output: " << decimal_input << std::endl;
    auto integer_input = static_cast<int64_t>(decimal_input);
    std::cout << "As integer: " << integer_input << std::endl;

    // TODO: num2
    const auto analysis = analyze_number(integer_input, 1);

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
