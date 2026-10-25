#include "commands.hpp"
#include "analysis.hpp"
#include "evaluator.hpp"
#include "styling.hpp"
#include "utils.hpp"
#include <iostream>

CommandResult cmd_info(const std::string _, ProgramState *state) {
  std::cout
      << style_info(
             "Here are available analysis functions, commands, functions and "
             "constants:")
      << std::endl;

  std::cout << std::endl << style_info("Analysis functions: ") << std::endl;
  for (auto af : analysis_funcs_order) {
    std::cout << " - " << (state->enabled[af] ? af : style_strikethrough(af))
              << (state->enabled[af] ? "" : " (Disabled)") << std::endl;
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

  std::cout << std::endl << style_info("Functions:");

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

  std::cout << std::endl << std::endl << style_info("Constants: ");

  first = true;
  for (auto [uc, val] : state->user_constants) {
    if (!first)
      std::cout << "; ";
    first = false;
    std::cout << style_bold(uc + ": " + std::to_string(val));
  }

  std::cout << std::endl;

  return CommandResult::SUCCESS;
}

// Handles invalid inputs, returns empty string if either key doesnt exist or
// ambigious.
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
  state->enabled[af] = true;

  std::cout << style_output_primary("Enabled analysis function: " + af)
            << std::endl;

  return CommandResult::SUCCESS;
}

CommandResult cmd_disable(const std::string input, ProgramState *state) {
  auto af = input_helper(analysis_funcs_order, input, "analysis functions");
  if (af.empty())
    return CommandResult::FAILURE;

  state->enabled[af] = false;

  std::cout << style_output_primary("Disabled analysis function: " + af)
            << std::endl;

  return CommandResult::SUCCESS;
}

CommandResult cmd_toggle(const std::string input, ProgramState *state) {
  auto af = input_helper(analysis_funcs_order, input, "analysis functions");
  if (af.empty())
    return CommandResult::FAILURE;

  state->enabled[af] = !state->enabled[af];

  auto af_state = std::string(state->enabled[af] ? "Enabled" : "Disabled");

  std::cout << style_output_primary(af_state + " analysis function: " + af)
            << std::endl;

  return CommandResult::SUCCESS;
}

CommandResult cmd_only(const std::string input, ProgramState *state) {
  auto af = input_helper(analysis_funcs_order, input, "analysis functions");
  if (af.empty())
    return CommandResult::FAILURE;

  for (auto inner_af : analysis_funcs_order) {
    state->enabled[inner_af] = false;
  }

  state->enabled[af] = true;

  std::cout << style_output_primary("Only enabled analysis function: " + af)
            << std::endl;

  return CommandResult::SUCCESS;
}

CommandResult cmd_enable_all(const std::string _, ProgramState *state) {
  for (auto af : analysis_funcs_order) {
    state->enabled[af] = true;
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
  size_t i = 0;
  while (is_alphanum(remaining_input[i]))
    i++;
  auto name = remaining_input.substr(0, i);
  remaining_input.erase(0, i);

  remaining_input = trim_string(remaining_input);
  if (remaining_input.empty()) {
    std::cout << style_error("ERROR: Missing expression!") << std::endl;
    return CommandResult::FAILURE;
  }

  auto res = evaluate_expression(remaining_input, state->user_constants);
  if (handle_error(&res))
    return CommandResult::FAILURE;
  auto val = std::get<double>(res);

  auto already_exists =
      state->user_constants.find(name) != state->user_constants.end();
  auto def_msg = std::string(already_exists ? "Redefined" : "Defined");

  state->user_constants[name] = val;

  std::cout << style_output_primary(def_msg + " " + name + " as " +
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

CommandResult cmd_exact_input(const std::string input, ProgramState *state) {}
CommandResult cmd_history(const std::string input, ProgramState *state) {}

CommandResult execute_command(const std::string input, ProgramState *state) {
  std::string remaining_input = input;
  while (remaining_input[0] == ' ')
    remaining_input.erase(0, 1);

  if (remaining_input[0] != '/') {
    return CommandResult::NOT_COMMAND;
  }

  size_t i = 1;
  while (i < remaining_input.size() && remaining_input[i] == ' ')
    i++;
  remaining_input.erase(0, i);

  if (remaining_input.empty()) {
    return CommandResult::MISSING_COMMAND;
  }

  i = 0;
  while (i < remaining_input.size() && remaining_input[i] != ' ')
    i++;
  auto input_command = remaining_input.substr(0, i);

  std::vector<std::string> command_keys;
  for (const auto &[k, _] : cmd_funcs)
    command_keys.push_back(k);

  auto idx = smart_find_index(command_keys, input_command);
  if (idx == -1) {
    return CommandResult::INVALID_COMMAND;
  } else if (idx == -2) {
    return CommandResult::AMBIGIOUS_COMMAND;
  }

  auto name = command_keys[static_cast<size_t>(idx)];
  auto it = cmd_funcs.find(name);
  return it->second.func(remaining_input.erase(0, i + 1), state);
};
