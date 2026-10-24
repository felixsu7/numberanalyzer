#include "analysis.hpp"
#include "evaluator.hpp"
#include "platform.hpp"
#include "screen.hpp"
#include "styling.hpp"
#include <cerrno>
#include <commands.hpp>
#include <cstdlib>
#include <cstring>
#include <iostream>

PLATFORM_ERROR enter_tui_mode();
void display_welcome_banner();

int main() {
  // TODO: make windows accept ansi regardless of tui mode or not.
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
        << style_info(
               "Input an integer to analyze, an expression like '2*-(42+3)' to "
               "evaluate and analyze, or type '/info' for more details")
        << std::endl;
    std::cout << style_input_secondary("> ");
    // make sure this doesnt mess up on any exit TODO
    std::cout << "\033[38;5;34m" << std::flush;
    // std::in immediately returns on space
    std::getline(std::cin, input);
    std::cout << RESET_STYLE << std::flush;

    auto cmd_res = execute_command(input, &state);

    if (cmd_res == CommandResult::QUIT) {
      break;
    } else if (cmd_res == CommandResult::MISSING_COMMAND) {
      std::cout << style_error("ERROR: Missing command.") << std::endl;
      continue;
    } else if (cmd_res == CommandResult::INVALID_COMMAND) {
      std::cout << style_error("ERROR: Invalid command.") << std::endl;
      continue;
    } else if (cmd_res == CommandResult::AMBIGIOUS_COMMAND) {
      std::cout << style_error("ERROR: Ambigious command.") << std::endl;
      continue;

    } else if ((cmd_res == CommandResult::SUCCESS) ||
               (cmd_res == CommandResult::FAILURE)) {
      continue;
    }

    // Not a command, so we continue on expressions
    auto evaluation = evaluate_expression(input, state.user_constants);
    if (std::holds_alternative<EvaluationError>(evaluation)) {
      auto err = std::get<EvaluationError>(evaluation);

      std::cout << style_error("ERROR: " + err.message + " (" + err.offending +
                               ") at position " + std::to_string(err.pos))
                << std::endl;

      continue;
    }

    auto evaled = std::get<long double>(evaluation);
    std::cout << style_info("Evaluator output: ")
              << style_output_secondary(std::to_string(evaled)) << std::endl;
    // Handle invalids TODO
    auto integer_input = static_cast<int64_t>(evaled);
    std::cout << style_info("As integer: ")
              << style_output_secondary(std::to_string(integer_input))
              << std::endl;

    state.user_constants["ans"] = evaled;

    const auto analysis = analyze_number(integer_input, state.enabled);

    for (auto [f, v] : analysis.logs) {
      std::cout << style_info(f) << ":" << std::endl;
      for (auto l : v) {
        std::cout << "    " << style_output_primary(l) << std::endl;
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
