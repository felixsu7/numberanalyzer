#include "commands.hpp"
#include "analysis.hpp"
#include "evaluator.hpp"
#include "styling.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <iostream>

CommandResult cmd_info(const std::string _, ProgramState *state) {
  std::cout
      << style_info(
             "Here are available analysis functions, commands, functions and "
             "constants:")
      << std::endl;

  std::cout << std::endl << style_info("Analysis functions: ") << std::endl;
  for (auto af : analysis_funcs_order) {
    std::cout << " - "
              << (state->enabled_analysis[af] ? af : style_strikethrough(af))
              << (state->enabled_analysis[af] ? "" : " (Disabled)")
              << std::endl;
  }

  std::cout << std::endl << style_info("Commands: ") << std::endl;
  for (auto [cf, def] : cmd_funcs) {
    std::cout << " - /" << style_bold(cf) << ": " << style_italic(def.desc)
              << std::endl;
    if (!def.example.empty()) {
      std::cout << style_italic("    Example: /" + cf + " " + def.example)
                << std::endl;
    }
  }

  std::cout << std::endl << style_info("Functions:") << " ";

  bool first = true;
  for (auto [mf, def] : math_funcs) {
    if (!first)
      std::cout << ", ";
    first = false;

    std::string args;
    bool first_arg = true;
    for (char i = 0; i < def.arity; i++) {
      if (!first_arg)
        args += ", ";
      first_arg = false;
      args.push_back('x' + i);
    }
    std::cout << style_bold(mf + "(" + args + ")");
  }

  std::cout << std::endl << std::endl << style_info("Constants:") << " ";

  first = true;
  for (auto [uc, val] : state->user_constants) {
    if (!first)
      std::cout << "; ";
    first = false;
    std::cout << style_bold(uc + ": " + std::to_string(val));
  }

  std::cout << std::endl
            << std::endl
            << style_info("Iteration limit (can be changed by /iterations): " +
                          std::to_string(get_iteration_limit()));

  std::cout << std::endl
            << std::endl
            << style_info("Auto-repeat is " +
                          std::string(state->auto_repeat ? "on" : "off") + ".");

  std::cout << std::endl;

  return CommandResult::SUCCESS;
}

// Handles invalid inputs, returns empty string if either key doesnt exist or
// ambiguous.
std::string input_helper(const std::vector<std::string> items,
                         const std::string key, const std::string what) {
  auto trimmed_key = trim_string(key);
  if (trimmed_key.empty()) {
    std::cout << style_error("ERROR: Missing required input.") << std::endl;
    return "";
  }

  auto idx = smart_find_index(items, trimmed_key);

  if (idx == -1) {
    std::cout << style_error("ERROR: Input '" + trimmed_key +
                             "' does not match any known " + what + ".")
              << std::endl;
    return "";
  }
  if (idx == -2) {
    std::cout << style_error("ERROR: Input '" + trimmed_key +
                             "' matches two or more known " + what +
                             ", please narrow the input down.")
              << std::endl;
    return "";
  }

  return items[static_cast<size_t>(idx)];
}

CommandResult cmd_quit(const std::string _, ProgramState *state) {
  state->running = false;
  return CommandResult::QUIT;
}

CommandResult cmd_enable(const std::string input, ProgramState *state) {
  auto af = input_helper(analysis_funcs_order, input, "analysis functions");
  if (af.empty())
    return CommandResult::FAILURE;
  state->enabled_analysis[af] = true;

  std::cout << style_output_primary("Enabled analysis function: " + af)
            << std::endl;

  return CommandResult::SUCCESS;
}

CommandResult cmd_disable(const std::string input, ProgramState *state) {
  auto af = input_helper(analysis_funcs_order, input, "analysis functions");
  if (af.empty())
    return CommandResult::FAILURE;

  state->enabled_analysis[af] = false;

  std::cout << style_output_primary("Disabled analysis function: " + af)
            << std::endl;

  return CommandResult::SUCCESS;
}

CommandResult cmd_toggle(const std::string input, ProgramState *state) {
  auto af = input_helper(analysis_funcs_order, input, "analysis functions");
  if (af.empty())
    return CommandResult::FAILURE;

  state->enabled_analysis[af] = !state->enabled_analysis[af];

  auto af_state =
      std::string(state->enabled_analysis[af] ? "Enabled" : "Disabled");

  std::cout << style_output_primary(af_state + " analysis function: " + af)
            << std::endl;

  return CommandResult::SUCCESS;
}

CommandResult cmd_only(const std::string input, ProgramState *state) {
  auto af = input_helper(analysis_funcs_order, input, "analysis functions");
  if (af.empty())
    return CommandResult::FAILURE;

  for (auto inner_af : analysis_funcs_order) {
    state->enabled_analysis[inner_af] = false;
  }

  state->enabled_analysis[af] = true;

  std::cout << style_output_primary("Only enabled analysis function: " + af)
            << std::endl;

  return CommandResult::SUCCESS;
}

CommandResult cmd_enable_all(const std::string _, ProgramState *state) {
  for (auto af : analysis_funcs_order) {
    state->enabled_analysis[af] = true;
  }

  std::cout << style_output_primary("Enabled all analysis functions!")
            << std::endl;

  return CommandResult::SUCCESS;
}

CommandResult cmd_define_constant(const std::string input,
                                  ProgramState *state) {
  if (input.empty()) {
    std::cout << style_error("ERROR: Missing constant name!") << std::endl;
    return CommandResult::FAILURE;
  }

  std::string remaining_input = input;
  auto consumed = consume_front_string(remaining_input);
  if (remaining_input.empty()) {
    std::cout << style_error("ERROR: Missing expression!") << std::endl;
    return CommandResult::FAILURE;
  }

  auto res = evaluate_expression(consumed.second, state->user_constants);
  if (handle_error(&res))
    return CommandResult::FAILURE;
  auto val = std::get<double>(res);

  auto already_exists =
      state->user_constants.find(consumed.first) != state->user_constants.end();
  auto def_msg = std::string(already_exists ? "Redefined" : "Defined");

  state->user_constants[consumed.first] = val;

  std::cout << style_output_primary(def_msg + " " + consumed.first + " as " +
                                    std::to_string(val));

  return CommandResult::SUCCESS;
}

CommandResult cmd_undefine_constant(const std::string input,
                                    ProgramState *state) {
  std::vector<std::string> constant_keys;
  for (const auto &[k, _] : state->user_constants)
    constant_keys.push_back(k);

  auto name = input_helper(constant_keys, input, "constants");
  if (name.empty())
    return CommandResult::FAILURE;

  state->user_constants.erase(name);

  std::cout << style_output_primary("Undefined constant: " + name) << std::endl;

  return CommandResult::SUCCESS;
}

CommandResult cmd_exact_input(const std::string input, ProgramState *state) {
  char *end;
  int64_t exact = std::strtoll(input.c_str(), &end, 10);
  if (end == input.c_str()) {
    auto err = static_cast<EvaluationResult>(
        EvaluationError{0, "Expected number", ""});
    handle_error(&err);
    return CommandResult::FAILURE;
  }

  const auto analysis = analyze_number(exact, state->enabled_analysis);
  state->history.push_back(analysis.num);
  state->done_analysis = true;

  for (auto af : analysis_funcs_order) {
    // if disabled yet somehow exists in logs, skip.
    // also includes sanity checks
    if (auto it = state->enabled_analysis.find(af);
        it != state->enabled_analysis.end() && !it->second &&
        analysis.logs.find(af) != analysis.logs.end())
      continue;

    std::cout << style_info(af) << ":" << std::endl;

    if (analysis.logs.find(af) != analysis.logs.end())
      for (auto l : analysis.logs.find(af)->second) {
        std::cout << "    " << style_output_primary(l) << std::endl;
      }
    if (analysis.errors.find(af) != analysis.errors.end())
      for (auto l : analysis.errors.find(af)->second) {
        std::cout << "    " << style_error("ERROR: " + l) << std::endl;
      }
  }

  std::cout << std::endl;

  return CommandResult::SUCCESS;
}

CommandResult cmd_history(const std::string _, ProgramState *state) {
  std::cout << style_info("History (" + std::to_string(state->history.size()) +
                          " entries):")
            << " ";

  if (state->history.size() > 0) {
    for (size_t i = 0; i < state->history.size(); i++) {
      std::cout << style_output_primary(
          std::to_string(state->history[state->history.size() - i - 1]));
      if (i != state->history.size() - 1)
        std::cout << ", ";
    }
  } else {
    std::cout << style_error("Empty");
  }

  std::cout << std::endl;
  return CommandResult::SUCCESS;
}

CommandResult cmd_toggle_styling(const std::string _, ProgramState *) {
  if (is_ansi_enabled())
    std::cout << RESET_STYLE << std::flush;

  toggle_ansi(!is_ansi_enabled());

  if (is_ansi_enabled()) {
    std::cout << style_output_primary("Enabled colors and styling of outputs!")
              << std::endl;
  } else {
    std::cout << "Disabled colors and styling of outputs!" << std::endl;
  }

  return CommandResult::SUCCESS;
}

CommandResult cmd_set_iteration_limit(const std::string input,
                                      ProgramState *_) {
  char *end;
  int64_t n = std::strtoll(input.c_str(), &end, 10);
  if (end == input.c_str()) {
    std::cout << style_error("Missing number.") << std::endl;
    return CommandResult::FAILURE;
  }

  set_iterations_limit(static_cast<uint>(uabs(n)));

  std::cout << style_output_primary("Set iteration limit to " +
                                    std::to_string(n) + " iterations!")
            << std::endl;

  return CommandResult::SUCCESS;
}

CommandResult cmd_toggle_auto_repeat(const std::string _, ProgramState *state) {
  state->auto_repeat = !state->auto_repeat;

  if (state->auto_repeat) {
    std::cout << style_output_primary(
                     "Enabled auto-repeat! The program no longer asks if you "
                     "want to analyze another number.")
              << std::endl;
  } else {
    std::cout << style_output_primary("Disabled auto-repeat!") << std::endl;
  }

  return CommandResult::SUCCESS;
}

CommandResult execute_command(const std::string input, ProgramState *state) {
  std::string remaining_input = input;
  if (remaining_input[0] != '/') {
    return CommandResult::NOT_COMMAND;
  }
  remaining_input.erase(0, 1);

  auto consumed = consume_front_string(remaining_input);
  if (consumed.first.empty()) {
    return CommandResult::MISSING_COMMAND;
  }

  std::vector<std::string> command_keys;
  for (const auto &[k, _] : cmd_funcs)
    command_keys.push_back(k);

  auto idx = smart_find_index(command_keys, consumed.first);
  if (idx == -1) {
    return CommandResult::INVALID_COMMAND;
  } else if (idx == -2) {
    return CommandResult::AMBIGUOUS_COMMAND;
  }

  auto name = command_keys[static_cast<size_t>(idx)];
  auto it = cmd_funcs.find(name);
  return it->second.func(consumed.second, state);
};
