#include "analysis.hpp"
#include "evaluator.hpp"
#include "platform.hpp"
#include "screen.hpp"
#include <cerrno>
#include <commands.hpp>
#include <cstdlib>
#include <cstring>
#include <iostream>

PLATFORM_ERROR enter_tui_mode();
void display_welcome_banner();

int main() {
  std::string input;
  display_welcome_banner();
  auto state = ProgramState{};
  state.running = true;
  state.user_constants = default_math_consts;

  for (auto f : analysis_funcs_order) {
    state.enabled[f] = true;
  }

  while (state.running) {
    std::cout
        << std::endl
        << "Input an integer to analyze, an expression like '2*-(42+3)' to "
           "evaluate and analyze, or type '/info' for more details"
        << std::endl;
    std::cout << "> ";
    // std::in immediately returns on space
    std::getline(std::cin, input);

    auto cmd_res = execute_command(input, &state);

    if (cmd_res == CommandResult::QUIT) {
      break;
    } else if (cmd_res == CommandResult::MISSING_COMMAND) {
      std::cout << "Missing command." << std::endl;
      continue;
    } else if (cmd_res == CommandResult::INVALID_COMMAND) {
      std::cout << "Invalid command." << std::endl;
      continue;
    } else if ((cmd_res == CommandResult::SUCCESS) ||
               (cmd_res == CommandResult::FAILURE)) {
      continue;
    }

    // Not a command, so we continue on expressions
    auto evaluation = evaluate_expression(input, state.user_constants);
    if (std::holds_alternative<EvaluationError>(evaluation)) {
      auto err = std::get<EvaluationError>(evaluation);

      std::cout << "ERROR: " << err.message << " (" << err.offending
                << ") at position " << err.pos << std::endl;

      continue;
    }

    auto decimal_input = std::get<long double>(evaluation);
    std::cout << "Evaluator output: " << decimal_input << std::endl;
    // Handle invalids TODO
    auto integer_input = static_cast<int64_t>(decimal_input);
    std::cout << "As integer: " << integer_input << std::endl;

    const auto analysis = analyze_number(integer_input, state.enabled);

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

PLATFORM_ERROR enter_tui_mode() {
  // TODO: query screen size
  Screen screen(100, 50);
  screen.activate();
  Cell c = default_cell;
  cell_set_glyph(&c, "A", 1);
  bool running = true;
  while (running) {
    PlatformEvent ev = screen.event_poll(100);
    c.bg.active = true;
    c.bg.red = 127;
    c.bg.green = 127;
    c.bg.blue = 127;
    // why
    screen.draw_rect(0, 0, 101, 50, c);
    screen.present();
    if (ev.key == 'q') {
      running = false;
      break;
    }
  }
  return screen.deactivate();
}

void display_welcome_banner() {
  std::cout << "NUMBER THEORY ANALYZER" << std::endl;
}
