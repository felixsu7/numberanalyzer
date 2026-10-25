#include "analysis.hpp"
#include "commands.hpp"
#include "evaluator.hpp"
#include "platform.hpp"
#include "screen.hpp"
#include "styling.hpp"
#include "utils.hpp"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>

PLATFORM_ERROR enter_tui_mode();
void display_welcome_banner();

int main() {
  toggle_ansi(try_enable_ansi());
  display_welcome_banner();

  auto state = ProgramState{};
  state.running = true;
  state.user_constants = default_math_consts;
  for (auto f : analysis_funcs_order) {
    state.enabled_analysis[f] = true;
  }

  std::string user_input;

  while (state.running) {
    std::cout
        << std::endl
        << style_italic(
               "Input an integer to analyze, an expression like '2*-(42+3)' to "
               "evaluate and analyze, or type '/info' for more details")
        << std::endl;
    std::cout << style_input_secondary("> ");

    // make sure this doesnt mess up on any exit TODO
    std::cout << "\033[38;5;34m" << std::flush;
    // std::in immediately returns on space
    std::getline(std::cin, user_input);
    std::cout << RESET_STYLE << std::flush;

    user_input = trim_string(user_input);

    auto cmd_res = execute_command(user_input, &state);

    if (cmd_res == CommandResult::QUIT) {
      break;
    } else if (cmd_res == CommandResult::MISSING_COMMAND) {
    } else if (cmd_res == CommandResult::INVALID_COMMAND) {
    } else if (cmd_res == CommandResult::AMBIGIOUS_COMMAND) {

    } else if ((cmd_res == CommandResult::SUCCESS) ||
               (cmd_res == CommandResult::FAILURE)) {
      continue;
    }

    switch (cmd_res) {
    case CommandResult::SUCCESS:
    case CommandResult::FAILURE:
    case CommandResult::QUIT:
      continue;

    case CommandResult::MISSING_COMMAND:
      std::cout << style_error("ERROR: Missing command.") << std::endl;
      continue;
    case CommandResult::INVALID_COMMAND:
      std::cout << style_error("ERROR: Invalid command.") << std::endl;
      continue;
    case CommandResult::AMBIGIOUS_COMMAND:
      std::cout << style_error("ERROR: Ambigious command.") << std::endl;
      continue;

    case CommandResult::NOT_COMMAND:
      break;
    }

    // Not a command, so we continue on expressions
    auto res = evaluate_expression(user_input, state.user_constants);
    if (handle_error(&res))
      continue;

    // TODO support exact ints

    auto evaled = std::get<double>(res);
    std::cout << style_info("Evaluator output: ")
              << style_output_secondary(std::to_string(evaled)) << std::endl;
    if (!fits_long(evaled)) {
      std::cout << style_error("ERROR: " + std::to_string(evaled) +
                               " is too large to be analyzed!")
                << std::endl;
      continue;
    }
    auto as_int = static_cast<int64_t>(evaled);
    std::cout << style_info("As integer: ")
              << style_output_secondary(std::to_string(as_int)) << std::endl;

    state.user_constants["ans"] = evaled;

    const auto analysis = analyze_number(as_int, state.enabled_analysis);
    state.history.push_back(analysis.num);

    for (auto af : analysis_funcs_order) {
      // if disabled yet somehow exists in logs, skip.
      // also includes sanity checks
      if (auto it = state.enabled_analysis.find(af);
          it != state.enabled_analysis.end() && !it->second &&
          analysis.logs.find(af) != analysis.logs.end())
        continue;

      std::cout << style_info(af) << ":" << std::endl;
      for (auto l : analysis.logs.find(af)->second) {
        std::cout << "    " << style_output_primary(l) << std::endl;
      }
      if (analysis.errors.find(af) != analysis.errors.end())
        for (auto l : analysis.errors.find(af)->second) {
          std::cout << style_error("ERROR: " + l) << std::endl;
        }
    }

    std::cout << std::endl;
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
  std::cout << "NUMBER THEORY ANALYZER" << std::endl << std ::endl;

  std::vector<std::string> logo_lines;

  logo_lines.push_back("       ++++++++++++                  \n");
  logo_lines.push_back("     +++++++     +++++++             \n");
  logo_lines.push_back("   +++++++          ++++++           \n");
  logo_lines.push_back(" +++++++     -      +++++++          \n");
  logo_lines.push_back("+++++++      -      +++++++          \n");
  logo_lines.push_back("+++++++  ++++-++++  +++++++          \n");
  logo_lines.push_back("+++++++      -      +++++++          \n");
  logo_lines.push_back(" +++++++     +     +++++++           \n");
  logo_lines.push_back("   +++++++       +++++++             \n");
  logo_lines.push_back("      +++++++++++++++++++++++++      \n");
  logo_lines.push_back("             +++++++       +++++++   \n");
  logo_lines.push_back("           +++++++           +++++++ \n");
  logo_lines.push_back("          +++++++   -+   -+   +++++++\n");
  logo_lines.push_back("          +++++++     ---     +++++++\n");
  logo_lines.push_back("          +++++++    -- -+    +++++++\n");
  logo_lines.push_back("          +++++++   -     +  +++++++ \n");
  logo_lines.push_back("            ++++++          +++++++  \n");
  logo_lines.push_back("              +++++++     +++++++    \n");
  logo_lines.push_back("                 +++++++++++++       \n");

  // TODO gradient
  std::cout << "\033[38;2;228;163;178m";
  for (auto l : logo_lines) {
    std::cout << l;
  }
  std::cout << RESET_STYLE << std::endl;
}
