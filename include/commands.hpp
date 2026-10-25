#pragma once

#include <functional>
#include <map>
#include <string>

enum class CommandResult {
  SUCCESS,
  FAILURE,
  QUIT,
  NOT_COMMAND,
  MISSING_COMMAND,
  INVALID_COMMAND,
  AMBIGUOUS_COMMAND,
};

struct ProgramState {
  bool running;
  bool auto_repeat;
  bool done_analysis;
  std::vector<int64_t> secondary_inputs;
  std::vector<int64_t> history;
  std::map<std::string, bool> enabled_analysis;
  std::map<std::string, double> user_constants;
};

using CommandFuncDef = CommandResult(const std::string, ProgramState *);

struct CommandDef {
  std::function<CommandFuncDef> func;
  std::string desc;
  std::string example;
};

CommandFuncDef cmd_info;
CommandFuncDef cmd_quit;
CommandFuncDef cmd_enable;
CommandFuncDef cmd_disable;
CommandFuncDef cmd_toggle;
CommandFuncDef cmd_only;
CommandFuncDef cmd_enable_all;
CommandFuncDef cmd_define_constant;
CommandFuncDef cmd_undefine_constant;
CommandFuncDef cmd_exact_input;
CommandFuncDef cmd_history;
CommandFuncDef cmd_toggle_styling;
CommandFuncDef cmd_set_iteration_limit;
CommandFuncDef cmd_toggle_auto_repeat;

static const std::map<std::string, CommandDef> cmd_funcs = {
    {"info",
     {cmd_info, "Displays available actions and the state of the program.",
      ""}},
    {"quit", {cmd_quit, "Exits the program.", ""}},
    {"enable",
     {cmd_enable, "Enables the analysis function matching the input.", "ev"}},
    {"disable",
     {cmd_disable, "Disables the analysis function matching the input.",
      "pos"}},
    {"toggle",
     {cmd_toggle, "Toggles the analysis function matching the input.", "sum"}},
    {"only",
     {cmd_only,
      "Enables only the analysis function matching the input; disabling "
      "everything else.",
      "factor"}},
    {"enableall",
     {cmd_enable_all, "Enables all of the analysis functions.", ""}},
    {"def",
     {cmd_define_constant, "(Re)defines a constant to be used.",
      "phi (1 + sqrt(5))/2"}},
    {"undef", {cmd_undefine_constant, "Undefines an existing constant.", "pi"}},
    {"exact",
     {cmd_exact_input, "Analyzes an exact integer; has better precision.",
      "-9223372036854775808"}},
    {"history",
     {cmd_history, "Views past analyzed numbers during this session.", ""}},
    {"styling",
     {cmd_toggle_styling, "Toggles color and styling of outputs.", ""}},
    {"iterations",
     {cmd_set_iteration_limit,
      "Sets the iteration limit of analysis functions (Currently only Collatz "
      "Conjecture Sequence).",
      "1000"}},
    {"autorepeat", {cmd_toggle_auto_repeat, "Toggles auto-repeat.", ""}}

};

CommandResult execute_command(const std::string input, ProgramState *state);
