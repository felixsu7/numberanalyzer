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
  AMBIGIOUS_COMMAND,
};

struct ProgramState {
  bool running;
  std::map<std::string, bool> enabled;
  std::map<std::string, long double> user_constants;
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

static const std::map<std::string, CommandDef> cmd_funcs = {
    {"info",
     {cmd_info, "Displays available actions and the state of the program.",
      ""}},
    {"quit", {cmd_quit, "Exits the program.", ""}},
    {"enable",
     {cmd_enable, "Enables the analysis function matching the input.", ""}},
    {"disable",
     {cmd_disable, "Disables the analysis function matching the input.", ""}},
    {"toggle",
     {cmd_toggle, "Toggles the analysis function matching the input.", ""}},
    {"only",
     {cmd_only,
      "Enables only the analysis function matching the input; disabling "
      "anything else.",
      ""}},
    {"enableall",
     {cmd_enable_all, "Enables all of the analysis functions.", ""}},
    {"def", {cmd_define_constant, "(Re)defines a constant to be used.", ""}},
    {"undef", {cmd_undefine_constant, "Undefines an existing constant.", ""}},
};

CommandResult execute_command(const std::string input, ProgramState *state);
