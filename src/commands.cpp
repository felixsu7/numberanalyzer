#include "commands.hpp"
#include "analysis.hpp"
#include "evaluator.hpp"
#include <iostream>

CommandResult cmd_info(const std::string _, ProgramState *state) {
  std::cout << "Here are available analysis functions, commands, functions and "
               "constants:"
            << std::endl;

  std::cout << std::endl << "Analysis functions: " << std::endl;
  for (size_t i = 0; i < analysis_funcs_order.size(); i++) {
    auto name = analysis_funcs_order[i];
    std::cout << " - " << name << (state->enabled[name] ? "" : " (Disabled)");
    // last index
    if (i != analysis_funcs_order.size() - 1) {
      std::cout << std::endl;
    }
  }

  std::cout << std::endl << std::endl << "Commands: " << std::endl;
  for (auto [name, def] : cmd_funcs) {
    std::cout << " - /" << name << ": " << def.desc << std::endl;
    if (!def.example.empty()) {
      std::cout << "    Example: /" << name << " " << def.example << std::endl;
    }
  }

  std::cout << std::endl
            << "Functions (numbers inside indicate the amount of arguments it "
               "takes):";

  for (auto [name, def] : math_funcs) {
    std::cout << " " << name << "(" << def.arity << ")";
  }

  std::cout << std::endl << std::endl << "Constants:";

  for (auto [name, val] : state->user_constants) {
    std::cout << " " << name << ": " << val << ";";
  }

  std::cout << std::endl;

  return CommandResult::SUCCESS;
}

std::string lower_case_string(const std::string s) {
  std::string res = s;
  std::transform(res.begin(), res.end(), res.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return res;
}

int smart_find_index(const std::vector<std::string> items,
                     const std::string key) {
  auto lowered_key = lower_case_string(key);
  auto lowered_items = items;
  std::transform(lowered_items.begin(), lowered_items.end(),
                 lowered_items.begin(),
                 [](const std::string s) { return lower_case_string(s); });

  std::string current_key;
  size_t key_idx = 0;
  int match_idx = -1;
  bool match = false;

  while (!match && key_idx < key.size()) {
    current_key.push_back(lowered_key[key_idx++]);
    match_idx = -1;
    match = false;

    for (size_t i = 0; i < lowered_items.size(); i++) {
      // is current_key a prefix of i?
      auto res = std::mismatch(current_key.begin(), current_key.end(),
                               lowered_items[i].begin());

      if (res.first == current_key.end()) {
        if (!match) {
          match = true;
          match_idx = static_cast<int>(i);
        } else {
          match = false;
          match_idx = -1;
          break;
        }
      }
    }
  }

  return match_idx;
}

CommandResult cmd_quit(const std::string _, ProgramState *state) {
  state->running = false;
  return CommandResult::QUIT;
}

CommandResult cmd_enable(const std::string input, ProgramState *state) {
  auto idx = smart_find_index(analysis_funcs_order, input);
  if (idx == -1) {
    std::cout << "ERROR: Input must exactly match one analysis function!"
              << std::endl;
    return CommandResult::FAILURE;
  }

  auto name = analysis_funcs_order[static_cast<size_t>(idx)];

  state->enabled[name] = true;

  std::cout << "Enabled analysis function: " << name << std::endl;

  return CommandResult::SUCCESS;
}

CommandResult cmd_disable(const std::string input, ProgramState *state) {
  auto idx = smart_find_index(analysis_funcs_order, input);
  if (idx == -1) {

    std::cout << "ERROR: Input must exactly match one analysis function!"
              << std::endl;
    return CommandResult::FAILURE;
  }

  auto name = analysis_funcs_order[static_cast<size_t>(idx)];

  state->enabled[name] = false;

  std::cout << "Disable analysis function: " << name << std::endl;

  return CommandResult::SUCCESS;
}

CommandResult cmd_toggle(const std::string input, ProgramState *state) {
  auto idx = smart_find_index(analysis_funcs_order, input);
  if (idx == -1) {
    std::cout << "ERROR: Input must exactly match one analysis function!"
              << std::endl;
    return CommandResult::FAILURE;
  }

  auto name = analysis_funcs_order[static_cast<size_t>(idx)];

  state->enabled[name] = !state->enabled[name];

  std::cout << (state->enabled[name] ? "Enabled" : "Disabled")
            << " analysis function: " << name << std::endl;

  return CommandResult::SUCCESS;
}

CommandResult cmd_only(const std::string input, ProgramState *state) {
  auto idx = smart_find_index(analysis_funcs_order, input);
  if (idx == -1) {
    std::cout << "ERROR: Input must exactly match one analysis function!"
              << std::endl;
    return CommandResult::FAILURE;
  }

  auto name = analysis_funcs_order[static_cast<size_t>(idx)];

  for (auto f : analysis_funcs_order) {
    state->enabled[f] = false;
  }

  state->enabled[name] = true;

  std::cout << "Only enabled analysis function: " << name << std::endl;

  return CommandResult::SUCCESS;
}

CommandResult cmd_enable_all(const std::string _, ProgramState *state) {
  for (auto f : analysis_funcs_order) {
    state->enabled[f] = true;
  }

  std::cout << "Enabled all analysis functions!" << std::endl;

  return CommandResult::SUCCESS;
}

static bool is_num(char c) { return c >= '0' && c <= '9'; }
static bool is_alpha(char c) {
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}
static bool is_whitespace(char c) {
  return (c == ' ') || (c == '\n') || (c == '\t');
}
static bool is_alphanum(char c) { return is_num(c) || is_alpha(c); }

CommandResult cmd_define_constant(const std::string input,
                                  ProgramState *state) {
  if (input.empty()) {
    std::cout << "ERROR: Missing name!" << std::endl;
    return CommandResult::FAILURE;
  }

  std::string remaining_input = input;
  size_t i = 0;
  while (is_alphanum(remaining_input[i]))
    i++;
  auto name = remaining_input.substr(0, i);
  remaining_input.erase(0, i);

  while (is_whitespace(input[0]))
    remaining_input.erase(0);

  if (remaining_input.empty()) {
    std::cout << "ERROR: Missing expression!" << std::endl;
    return CommandResult::FAILURE;
  }

  auto evaluation = evaluate_expression(remaining_input, state->user_constants);

  if (std::holds_alternative<EvaluationError>(evaluation)) {
    auto err = std::get<EvaluationError>(evaluation);

    std::cout << "ERROR: " << err.message << " (" << err.offending
              << ") at position " << err.pos << std::endl;

    return CommandResult::FAILURE;
  }

  auto val = std::get<long double>(evaluation);

  auto already_exists =
      state->user_constants.find(name) != state->user_constants.end();

  state->user_constants[name] = val;

  std::cout << (already_exists ? "Redefined" : "Defined") << " " << name
            << " as " << val;

  return CommandResult::SUCCESS;
}

CommandResult cmd_undefine_constant(const std::string input,
                                    ProgramState *state) {
  std::vector<std::string> constant_keys;
  for (const auto &[k, _] : state->user_constants)
    constant_keys.push_back(k);

  auto idx = smart_find_index(constant_keys, input);
  if (idx == -1) {
    std::cout << "ERROR: Input must exactly match one constant!" << std::endl;
    return CommandResult::FAILURE;
  }

  auto name = constant_keys[static_cast<size_t>(idx)];

  state->user_constants.erase(name);

  std::cout << "Undefined constant: " << name << std::endl;

  return CommandResult::SUCCESS;
}

CommandResult execute_command(const std::string input, ProgramState *state) {
  std::string remaining_input = input;
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
  auto command_name = remaining_input.substr(0, i);
  auto it = cmd_funcs.find(command_name);

  if (it == cmd_funcs.end()) {
    return CommandResult::INVALID_COMMAND;
  }

  return it->second.func(remaining_input.erase(0, i + 1), state);
};
