

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

#include <cstdint>
#include <map>
#include <string>
#include <vector>

using string_vec_map = std::map<std::string, std::vector<std::string>>;

using AnalysisFuncDef = void(int64_t);

struct NumberAnalysis {
  int64_t num;
  string_vec_map logs;
  string_vec_map errors;
};

AnalysisFuncDef analysis_sign;
AnalysisFuncDef analysis_even_odd;
AnalysisFuncDef analysis_divisbility;
AnalysisFuncDef analysis_factors;
AnalysisFuncDef analysis_factors_amount;
AnalysisFuncDef analysis_prime_composite;
AnalysisFuncDef analysis_factors_sum;
AnalysisFuncDef analysis_perfect;
AnalysisFuncDef analysis_collatz;
AnalysisFuncDef analysis_prime_factors;
AnalysisFuncDef analysis_perfect_square_cube;
AnalysisFuncDef analysis_is_triangular;
// I guess this is also considered as 12. Triangular Number
AnalysisFuncDef analysis_nth_triangular;

static const std::map<std::string, AnalysisFuncDef *> analysis_funcs = {
    {"Positive/Negative/Zero", analysis_sign},
    {"Even/Odd", analysis_even_odd},
    {"Divisibility", analysis_divisbility},
    {"Factors", analysis_factors},
    {"Number of Factors", analysis_factors_amount},
    {"Prime or composite", analysis_prime_composite},
    {"Sum of Factors", analysis_factors_sum},
    {"Perfect Number", analysis_perfect},
    {"Collatz Conjecture Sequence", analysis_collatz},
    {"Prime Factorization", analysis_prime_factors},
    {"Perfect Square/Cube", analysis_perfect_square_cube},
    {"Triangular Number", analysis_is_triangular},
    {"nth Triangular Number", analysis_nth_triangular},
};

// Theres no ordered maps.
static const std::vector<std::string> analysis_funcs_order = {
    "Positive/Negative/Zero",
    "Even/Odd",
    "Divisibility",
    "Factors",
    "Number of Factors",
    "Prime or composite",
    "Sum of Factors",
    "Perfect Number",
    "Collatz Conjecture Sequence",
    "Prime Factorization",
    "Perfect Square/Cube",
    "Triangular Number",
    "nth Triangular Number",
};

void set_iterations_limit(uint n);
uint get_iteration_limit();

void set_secondary_inputs(const std::vector<int64_t> v);

NumberAnalysis analyze_number(int64_t num,
                              const std::map<std::string, bool> enabled);

void handle_secondary_input_dialogue(ProgramState *state);

#include <functional>
#include <map>
#include <random>
#include <string>
#include <variant>
#include <vector>

using MathFuncArgs = const std::vector<double> &;

struct MathFuncDef {
  size_t arity;
  std::function<double(MathFuncArgs)> func;
};

static const std::map<std::string, MathFuncDef> math_funcs{
    {"sin", {1, [](MathFuncArgs a) { return std::sin(a[0]); }}},
    {"cos", {1, [](MathFuncArgs a) { return std::cos(a[0]); }}},
    {"tan", {1, [](MathFuncArgs a) { return std::tan(a[0]); }}},
    {"csc", {1, [](MathFuncArgs a) { return 1.0 / std::sin(a[0]); }}},
    {"sec", {1, [](MathFuncArgs a) { return 1.0 / std::cos(a[0]); }}},
    {"cot", {1, [](MathFuncArgs a) { return 1.0 / std::tan(a[0]); }}},
    {"asin", {1, [](MathFuncArgs a) { return std::asin(a[0]); }}},
    {"acos", {1, [](MathFuncArgs a) { return std::acos(a[0]); }}},
    {"atan", {1, [](MathFuncArgs a) { return std::atan(a[0]); }}},

    {"sinh", {1, [](MathFuncArgs a) { return std::sinh(a[0]); }}},
    {"cosh", {1, [](MathFuncArgs a) { return std::cosh(a[0]); }}},
    {"tanh", {1, [](MathFuncArgs a) { return std::tanh(a[0]); }}},
    {"asinh", {1, [](MathFuncArgs a) { return std::asinh(a[0]); }}},
    {"acosh", {1, [](MathFuncArgs a) { return std::acosh(a[0]); }}},
    {"atanh", {1, [](MathFuncArgs a) { return std::atanh(a[0]); }}},

    {"sqrt", {1, [](MathFuncArgs a) { return std::sqrt(a[0]); }}},
    {"cbrt", {1, [](MathFuncArgs a) { return std::cbrt(a[0]); }}},
    {"ln", {1, [](MathFuncArgs a) { return std::log(a[0]); }}},
    {"pow", {2, [](MathFuncArgs a) { return std::pow(a[0], a[1]); }}},

    {"floor", {1, [](MathFuncArgs a) { return std::floor(a[0]); }}},
    {"ceil", {1, [](MathFuncArgs a) { return std::ceil(a[0]); }}},
    {"abs", {1, [](MathFuncArgs a) { return std::fabs(a[0]); }}},
    {"min", {2, [](MathFuncArgs a) { return std::min(a[0], a[1]); }}},
    {"max", {2, [](MathFuncArgs a) { return std::max(a[0], a[1]); }}},

    {"rand",
     {0,
      [](MathFuncArgs _) {
        std::mt19937_64 rng{std::random_device{}()};
        std::uniform_real_distribution<double> dist(0.0L, 1.0L);

        return dist(rng);
      }}},
    {"randr",
     {2,
      [](MathFuncArgs a) {
        std::mt19937_64 rng{std::random_device{}()};
        std::uniform_real_distribution<double> dist(a[0], a[1]);

        return dist(rng);
      }}},
    // Not really an int.
    {"randint",
     {2,
      [](MathFuncArgs a) {
        std::mt19937_64 rng{std::random_device{}()};
        std::uniform_real_distribution<double> dist(a[0], a[1]);

        return std::ceil(dist(rng));
      }}},

};

// https://en.wikipedia.org/wiki/List_of_mathematical_constants
static const std::map<std::string, double> default_math_consts{
    {"ans", 0},
    {"pi", 3.14159265358979323846},
    {"e", 2.71828182845904523536},
    {"tau", 6.28318530717958647692},
    {"phi", 1.61803398874989484820},
    {"sixseven", 67.67},

};

struct EvaluationError {
  size_t pos;
  std::string message;
  std::string offending;
  std::string expr;
};

using EvaluationResult = std::variant<double, EvaluationError>;

EvaluationResult
evaluate_expression(const std::string expr,
                    const std::map<std::string, double> user_constants);

#include <string>

const std::string RESET_STYLE = "\033[0m";
void toggle_ansi(bool state);
bool try_enable_ansi();
bool is_ansi_enabled();

using simple_style_func = std::string(std::string s);

simple_style_func style_bold;
simple_style_func style_italic;
simple_style_func style_underlined;
simple_style_func style_strikethrough;

simple_style_func style_info;
simple_style_func style_error;
simple_style_func style_input_primary;
simple_style_func style_input_secondary;
simple_style_func style_output_primary;
simple_style_func style_output_secondary;

#include <string>
#include <utility>
#include <vector>

bool fits_long(double d);
unsigned long uabs(long x);

bool is_num(char c);
bool is_alpha(char c);
bool is_whitespace(char c);
bool is_alphanum(char c);

// Trims whitespace from start and end of s, and just erases every unicode
std::string trim_string(const std::string s);
std::string lower_case_string(const std::string s);
std::pair<std::string, std::string> consume_front_string(const std::string s);

// Returns the index into items of the closest match to the key.
// Returns -1 if not found, or -2 if found two or more matches.
int smart_find_index(const std::vector<std::string> items,
                     const std::string key);

bool is_eval_error(EvaluationResult res);
bool handle_eval_error(EvaluationResult *res, bool report_expr);

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>

void display_welcome_banner();

int main() {
  toggle_ansi(try_enable_ansi());
  display_welcome_banner();

  auto state = ProgramState{};
  state.running = true;
  state.auto_repeat = false;
  state.done_analysis = false;
  state.user_constants = default_math_consts;
  for (auto f : analysis_funcs_order) {
    state.enabled_analysis[f] = true;
  }

  std::string user_input;

  while (state.running) {
    // We place the check here because /exact also does analysis and that goes a
    // different route.
    if (state.done_analysis && !state.auto_repeat) {
      while (true) {
        std::cout << style_output_secondary(
                         "Do you want to analyze another number? (y/n)")
                  << std::endl;
        std::cout << style_input_secondary("> ");
        std::getline(std::cin, user_input);
        user_input = lower_case_string(trim_string(user_input));

        if (user_input.empty())
          continue;
        if (user_input[0] == 'y') {
          state.done_analysis = false;
          break;
        } else if (user_input[0] == 'n') {
          state.running = false;
          break;
        } else {
          std::cout << style_error("Please type in only either 'y' or 'n'.")
                    << std::endl;
          continue;
        }
      }
      continue;
    }

    state.done_analysis = false;

    std::cout
        << std::endl
        << style_italic(
               "Input an integer to analyze, an expression like '2*-(42+3)' to "
               "evaluate and analyze, or type '/info' for more details")
        << std::endl;
    std::cout << style_input_secondary("> ");

    // make sure this doesnt mess up on any exit TODO
    if (is_ansi_enabled())
      std::cout << "\033[38;5;34m" << std::flush;
    // std::in immediately returns on space
    std::getline(std::cin, user_input);
    if (is_ansi_enabled())
      std::cout << RESET_STYLE << std::flush;

    user_input = trim_string(user_input);

    if (user_input.empty()) {
      continue;
    }

    auto cmd_res = execute_command(user_input, &state);

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
    case CommandResult::AMBIGUOUS_COMMAND:
      std::cout << style_error("ERROR: Ambiguous command.") << std::endl;
      continue;

    case CommandResult::NOT_COMMAND:
      break;
    }

    // Not a command, so we continue on expressions
    auto res = evaluate_expression(user_input, state.user_constants);
    if (handle_eval_error(&res, false))
      continue;

    auto evaled = std::get<double>(res);
    std::cout << style_info("Evaluator output:") << " "
              << style_output_secondary(std::to_string(evaled)) << std::endl;
    if (!fits_long(evaled)) {
      std::cout << style_error("ERROR: " + std::to_string(evaled) +
                               " is too large to be analyzed!")
                << std::endl;
      continue;
    }
    auto as_int = static_cast<int64_t>(evaled);
    std::cout << style_info("As integer:") << " "
              << style_output_secondary(std::to_string(as_int)) << std::endl;

    state.user_constants["ans"] = evaled;

    handle_secondary_input_dialogue(&state);
    const auto analysis = analyze_number(as_int, state.enabled_analysis);
    state.history.push_back(analysis.num);
    state.done_analysis = true;

    bool at_least_one = false;
    for (auto af : analysis_funcs_order) {
      // if disabled yet somehow exists in logs, skip.
      // also includes sanity checks
      if (auto it = state.enabled_analysis.find(af);
          it != state.enabled_analysis.end() && !it->second &&
          analysis.logs.find(af) == analysis.logs.end())
        continue;

      std::cout << style_info(af) << ":" << std::endl;

      if (analysis.logs.find(af) != analysis.logs.end())
        for (auto l : analysis.logs.find(af)->second) {
          std::cout << "    " << style_output_primary(l) << std::endl;
          at_least_one = true;
        }
      if (analysis.errors.find(af) != analysis.errors.end())
        for (auto l : analysis.errors.find(af)->second) {
          std::cout << "    " << style_error("ERROR: " + l) << std::endl;
          at_least_one = true;
        }
    }

    if (!at_least_one)
      std::cout << std::endl
                << style_info("No enabled analysis functions...") << std::endl;

    std::cout << std::endl;
  }

  std::cout << style_info("Exiting number theory analyzer! Goodbye!")
            << std::endl;

  return 0;
}

void display_welcome_banner() {
  std::vector<std::string> logo_lines;

  logo_lines.push_back("        ++++++++++++                 \n");
  logo_lines.push_back("    +++++++     +++++++              \n");
  logo_lines.push_back("  +++++++          ++++++            \n");
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
  if (is_ansi_enabled())
    std::cout << "\033[38;2;228;163;178m\033[1m";

  std::cout << "NUMERICAL PROPERTY AND DIVISIBILITY ANALYZER"
            << " by " << "The 8-Bit Group!" << std::endl
            << std ::endl;
  for (auto l : logo_lines) {
    std::cout << l;
  }

  if (is_ansi_enabled())
    std::cout << RESET_STYLE << std::endl;
}

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
  if (handle_eval_error(&res, false))
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
        EvaluationError{0, "Expected number", "", ""});
    handle_eval_error(&err, false);
    return CommandResult::FAILURE;
  }

  // TODO: extract these out
  handle_secondary_input_dialogue(state);
  const auto analysis = analyze_number(exact, state->enabled_analysis);
  state->history.push_back(analysis.num);
  state->done_analysis = true;

  bool at_least_one = false;
  for (auto af : analysis_funcs_order) {
    // if disabled yet somehow exists in logs, skip.
    // also includes sanity checks
    if (auto it = state->enabled_analysis.find(af);
        it != state->enabled_analysis.end() && !it->second &&
        analysis.logs.find(af) == analysis.logs.end())
      continue;

    std::cout << style_info(af) << ":" << std::endl;

    if (analysis.logs.find(af) != analysis.logs.end())
      for (auto l : analysis.logs.find(af)->second) {
        std::cout << "    " << style_output_primary(l) << std::endl;
        at_least_one = true;
      }
    if (analysis.errors.find(af) != analysis.errors.end())
      for (auto l : analysis.errors.find(af)->second) {
        std::cout << "    " << style_error("ERROR: " + l) << std::endl;
        at_least_one = true;
      }
  }

  if (!at_least_one)
    std::cout << std::endl
              << style_info("No enabled analysis functions...") << std::endl;

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

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

static const std::string endl = "\n";
static std::stringstream cout = std::stringstream();

static uint ITERATION_LIMIT = 100;

void set_iterations_limit(uint n) { ITERATION_LIMIT = n; }

uint get_iteration_limit() { return ITERATION_LIMIT; }

// this is just horrible practice in general.
static std::vector<int64_t> SECONDARY_INPUTS;

void set_secondary_inputs(const std::vector<int64_t> v) {
  SECONDARY_INPUTS = v;
}

// The order of definitions was based on when they were submitted

// It should be noted that I modified their submissions, mainly on checks, the
// mathematical basis is kept exactly as they intended.

// Originally, this accepted two inputs.
// However, I turned it into analysis of what numbers does it divide cleanly
// with. Checking from 1 to N.
// UPDATE: special yarn?
void analysis_divisbility(int64_t num) {
  if (num == 0) {
    cout << "Zero divided by any number is always zero." << endl;
    return;
  };

  if (SECONDARY_INPUTS.empty()) {
    cout << "ERROR: No selected integers set." << endl;
    return;
  }

  bool at_least_one = false;

  cout << num << " is divisible by ";
  for (auto d : SECONDARY_INPUTS) {
    if (d == 0)
      continue;
    if (num % d == 0) {
      at_least_one = true;
      cout << d << ", ";
    }
  }

  if (!at_least_one)
    cout << "none of the selected integers.";

  cout << endl;
}

void analysis_sign(int64_t num) {
  if (num < 0) {
    cout << num << " is negative." << endl;
  } else if (num == 0) {
    cout << num << " is zero." << endl;
  } else {
    cout << num << " is positive." << endl;
  }
}

// Submitted to me in this form, making a wrapper wasn't difficult anyways.
std::vector<int64_t> _primefactors(int64_t n) {
  std::vector<int64_t> factors;
  if (n < 2)
    return factors;

  // step 1, divide n to 2 while n is even
  while (n % 2 == 0) {
    factors.push_back(2);
    n /= 2;
  }
  // step 2, iterate through odd numbers starting from 3 to the quotient of n
  // and i, then check if it is divisible to i, then divide it to i
  int i = 3;
  while (i <= n / i) {
    if (n % i == 0) {
      factors.push_back(i);
      n /= i;
    } else {
      i += 2;
    }
  }
  // step 3, if the remaining number is more than 1, it is another factor of the
  // number
  if (n > 1) {
    factors.push_back(n);
  }
  return factors;
}

void analysis_prime_factors(int64_t num) {
  auto prime_factors = _primefactors(num);

  if (prime_factors.empty()) {
    cout << num << " has no prime factors." << endl;
    return;
  }

  std::vector<int64_t> deduped_pfs;
  for (auto pf : prime_factors) {
    if (std::find(deduped_pfs.begin(), deduped_pfs.end(), pf) ==
        deduped_pfs.end())
      deduped_pfs.push_back(pf);
  }

  for (auto pf : deduped_pfs) {
    cout << pf << ", ";
  }

  cout << endl;
}

void analysis_perfect_square_cube(int64_t num) {
  // I added this check.
  if (num < 0) {
    cout << "ERROR: Input cannot be negative." << endl;
    return;
  }

  int64_t iskwerot = static_cast<int64_t>(round(sqrt(num)));
  int64_t kyubrot = static_cast<int64_t>(round(cbrt(num)));

  if (num >= 0 && iskwerot * iskwerot == num) {
    cout << num << " is a perfect square." << endl;
  } else {
    cout << num << " is not a perfect square." << endl;
  }

  if (kyubrot * kyubrot * kyubrot == num) {
    cout << num << " is a perfect cube." << endl;
  } else {
    cout << num << " is not a perfect cube." << endl;
  }
}

void analysis_even_odd(int64_t num) {
  if (num % 2 == 0) {
    cout << num << " is an even number." << endl;
  } else {
    cout << num << " is an odd number." << endl;
  }
}

void analysis_factors_sum(int64_t num) {
  int64_t sum = 0;

  if (num < 0) {
    cout << "ERROR: Input cannot be negative." << endl;
    return;
  }

  for (int64_t x = 1; x <= num; x++) {
    if (num % x == 0) {
      // cout << x << ", ";
      sum += x;
    }
  }
  cout << num << " has sum of positive factors of " << sum << "." << endl;
}

void analysis_factors_amount(int64_t num) {
  int64_t factorCount = 0;

  if (num == 0) {
    cout << "Zero has infinitely many factors.";
  } else {
    for (int divisor = 1; divisor <= abs(num); divisor++) {
      if (num % divisor == 0) {
        factorCount += 1;
      }
    }

    cout << num << " has " << factorCount << " factor(s)." << endl;
  }
}

void analysis_prime_composite(int64_t num) {
  int64_t divisor = 0;

  // How about 1, 0, and the negatives?
  if (num <= 0) {
    cout << "ERROR: " << num << " is not a natural number (1+).";
  } else if (num <= 1) {
    cout << "One is neither prime nor composite.";
  } else {
    for (divisor = 2; divisor < num; divisor++) {
      if (num % divisor == 0) {
        break;
      }
    }

    if (divisor == num) {
      cout << num << " is a prime number.";
    } else {
      cout << num << " is a composite number.";
    }
  }
}

void analysis_collatz(int64_t num) {
  int64_t Colture = num;

  // I added this check because it is infinitely looping on negative and zeroes.
  if (num <= 0) {
    cout << "ERROR: Input cannot be negative or zero." << endl;
    return;
  }

  uint iteration = 0;

  while (Colture != 1 && iteration < ITERATION_LIMIT) {
    cout << Colture << ", ";

    if (Colture % 2 == 0) {
      Colture = Colture / 2;
    } else {
      Colture = (Colture * 3) + 1;
    }

    iteration++;
  }

  if (iteration >= ITERATION_LIMIT) {
    cout << "..., ";
  }

  cout << 1 << endl;
}

void analysis_perfect(int64_t num) {
  // Numbers <= 1 cannot be perfect numbers (the smallest perfect number is 6)
  if (num <= 1) {
    cout << num << " is not a perfect number." << endl;
    return;
  }

  int64_t divisor_sum = 1; // 1 is always a proper divisor
  // Stop at sqrt to avoid checking duplicate divisor pairs
  for (int64_t i = 2; i <= std::sqrt(num); i++) {
    if (num % i == 0) {
      divisor_sum += i; // Add smaller divisor
      if (i != num / i) {
        divisor_sum += num / i; // Add matching larger divisor pair
      }
    }
  }

  if (divisor_sum == num) {
    cout << num << " is a perfect number." << endl;
  } else {
    cout << num << " is not a perfect number" << endl;
  }
}

std::string analysis_suffix(int64_t num) {
  auto ones = num % 10;
  if (ones == 1) {
    return "st";
  } else if (ones == 2) {
    return "nd";
  } else if (ones == 3) {
    return "rd";
  } else {
    return "th";
  }
}

void analysis_nth_triangular(int64_t num) {
  if (num < 0) {
    cout << "ERROR: Input cannot be negative." << endl;
    return;
  }

  // Triangular number: n(n+1)/2
  cout << num << analysis_suffix(num) << " triangular number is "
       << (num * (num + 1)) / 2 << "." << endl;
}

void analysis_is_triangular(int64_t num) {
  if (num < 0) {
    cout << num << " is not a triangular number." << endl;
    return;
  }

  // A number is a triangular number if 8x + 1 is a perfect square.
  int64_t val = 8 * num + 1;
  int64_t root = static_cast<int64_t>(std::sqrt(val));

  if (root * root == val) {
    cout << num << " is a triangular number." << endl;
  } else {
    cout << num << " is not a triangular number." << endl;
  }
}

void analysis_factors(int64_t num) {
  if (num == 0) {
    cout << "Zero has infinite amount of factors." << endl;
    return;
  }

  std::vector<int64_t> factors;
  auto absolute = uabs(num);

  for (uint64_t x = 1; x <= absolute; x++) {
    if (absolute % x == 0) {
      cout << x << ", ";
    }
  }
}

std::string clean_line(const std::string s) {
  auto trimmed = trim_string(s);
  if (trimmed.back() == ',')
    trimmed.pop_back();
  return trimmed;
}

void dump_logs(const std::string name, string_vec_map *logs,
               string_vec_map *errors) {
  const std::string ERROR_MARKER = "ERROR: ";
  std::string line;

  while (std::getline(cout, line, '\n')) {
    auto res =
        std::mismatch(ERROR_MARKER.begin(), ERROR_MARKER.end(), line.begin());
    // prefixed with ERROR:
    if (res.first == ERROR_MARKER.end()) {
      (*errors)[name].push_back(clean_line(line.erase(0, ERROR_MARKER.size())));
    } else {
      (*logs)[name].push_back(clean_line(line));
    }
  }

  cout.str("");
  cout.clear();
}

NumberAnalysis analyze_number(int64_t num,
                              const std::map<std::string, bool> enabled) {
  cout.str("");
  cout.clear();
  auto analysis = NumberAnalysis{};
  analysis.num = num;

  for (auto [name, func] : analysis_funcs) {
    // Skip functions that are not enabled.
    if (auto it = enabled.find(name); it != enabled.end() && !it->second)
      continue;
    func(num);
    dump_logs(name, &analysis.logs, &analysis.errors);
  }

  return analysis;
}

// idk whats a better place to place this
// just bad code all around
void handle_secondary_input_dialogue(ProgramState *state) {
  set_secondary_inputs({});
  if (!state->enabled_analysis["Divisibility"])
    return;

  std::cout
      << std::endl
      << style_info(
             "Since the Divisibility analysis function is enabled, please "
             "input "
             "selected integers to be tested against; separated by semicolons "
             "(';'). "
             "Type in exactly "
             "'q' to not input anything.")
      << std::endl;

  while (true) {
    std::string user_input;
    std::cout << style_input_secondary("> ");
    if (is_ansi_enabled())
      std::cout << "\033[38;5;34m" << std::flush;
    // std::in immediately returns on space
    std::getline(std::cin, user_input);
    if (is_ansi_enabled())
      std::cout << RESET_STYLE << std::flush;

    user_input = lower_case_string(trim_string(user_input));
    if (user_input.empty())
      continue;
    if (user_input == "q") {
      std::cout << style_output_primary("No selected integers set, continuing.")
                << std::endl;
      break;
    }

    std::vector<int64_t> collected;
    std::string expr;
    while (!user_input.empty() || !expr.empty()) {
      while (!user_input.empty() && user_input[0] != ';') {
        expr.push_back(user_input[0]);
        user_input.erase(0, 1);
      }
      if (!user_input.empty() && user_input[0] == ';') {
        user_input.erase(0, 1);
      }

      auto res = evaluate_expression(expr, state->user_constants);
      expr = "";
      if (handle_eval_error(&res, true))
        continue;

      auto evaled = std::get<double>(res);
      if (!fits_long(evaled)) {
        std::cout << style_error("ERROR: " + std::to_string(evaled) +
                                 " is too large to be analyzed!")
                  << std::endl;
        continue;
      }
      auto as_int = static_cast<int64_t>(evaled);

      collected.push_back(as_int);
    }

    if (collected.empty()) {
      std::cout << style_error("ERROR: No parsable inputs, try again or 'q' to "
                               "stop immediately.")
                << std::endl;
      continue;
    } else {
      std::cout << style_info("Selected Integers:") << " ";
      bool first = true;
      for (auto si : collected) {
        if (!first)
          std::cout << ", ";
        first = false;
        std::cout << style_input_secondary(std::to_string(si));
      }
      std::cout << std::endl;
    }

    state->secondary_inputs = collected;
    set_secondary_inputs(state->secondary_inputs);
    break;
  }
};

#include <cstdlib>
#include <string>
#include <utility>
#include <variant>
#include <vector>

// Based on https://nicolovaligi.com/articles/precedence-climbing
// Also https://craftinginterpreters.com/parsing-expressions.html
// Instead of an AST though, we immediately evaluate.
// Also, it's a double first since trig functions would otherwise just
// return 0 almost always.
// We could use long doubles, but now that isn't portable.

int prec(char c) {
  switch (c) {
  case '+':
  case '-':
    return 1;
  case '*':
  case '/':
    return 2;
  case '^':
    return 4;
  default:
    return 0;
  }
}

class Evaluator {
  std::string source;
  size_t pos = 0;

  EvaluationResult expression(int min) {
    auto left_res = primary();
    if (is_eval_error(left_res))
      return left_res;
    auto left = std::get<double>(left_res);

    while (true) {
      char op = peek();
      int pr = prec(op);
      if (pr < min)
        return left;
      size_t op_pos = pos++;

      // Exponentation is the only right-associative operator we have, so
      // ternary is fine.
      auto right_res = expression(op == '^' ? pr : pr + 1);
      if (is_eval_error(right_res))
        return right_res;
      auto right = std::get<double>(right_res);

      switch (op) {
      case '+':
        left += right;
        break;
      case '-':
        left -= right;
        break;
      case '*':
        left *= right;
        break;
      case '/':
        if (right == 0)
          return EvaluationError{op_pos, "Division by zero", "0", ""};
        left /= right;
        break;
      case '^':
        left = std::pow(left, right);
        break;
      }
    }
  }

  EvaluationResult primary() {
    char c = peek();

    if (c == '(') {
      pos++;
      auto sub_res = expression(1);
      if (is_eval_error(sub_res))
        return sub_res;

      if (!consume(')')) {
        return EvaluationError{pos, "Missing ending parenthesis", ")", ""};
      };

      return std::get<double>(sub_res);
    }

    if (is_alpha(c)) {
      size_t start = pos;
      while (is_alphanum(source[pos]))
        pos++;
      std::string id = source.substr(start, pos - start);

      if (consume('(')) {
        // Functions
        std::vector<double> args;
        if (peek() != ')')
          do {
            auto arg_res = expression(1);
            if (is_eval_error(arg_res))
              return arg_res;
            args.push_back(std::get<double>(arg_res));
          } while (peek() == ',' && pos++);
        if (!consume(')')) {
          return EvaluationError{pos, "Missing ending parenthesis", ")", ""};
        };

        auto it = math_funcs.find(id);
        if (it == math_funcs.end())
          return EvaluationError{start, "Unknown function", id, ""};

        if (args.size() != it->second.arity)
          return EvaluationError{
              start, "Function " + id + " got wrong amount of arguments",
              std::to_string(args.size()), ""};

        double val = it->second.func(args);
        if (!std::isfinite(val))
          return EvaluationError{start, "Function domain error", id, ""};

        return val;
      } else {
        // Constants
        auto it = user_constants.find(id);
        if (it == user_constants.end())
          return EvaluationError{start, "Unknown constant", id, ""};
        return it->second;
      }
    }

    if (consume('-')) {
      auto neg_res = expression(4);
      if (is_eval_error(neg_res))
        return neg_res;
      return -std::get<double>(neg_res);
    }

    // Integers, decimals, and somehow hexidecimals for free.
    char *end;
    double decimal = std::strtod(source.c_str() + pos, &end);
    if (end == source.c_str() + pos)
      return EvaluationError{pos, "Expected number", "", ""};

    // Moves to position after last digit
    pos = static_cast<size_t>(end - source.c_str());
    return decimal;
  }

  char peek() {
    // Whitespace is meaningless always, so ignore it here.
    while (is_whitespace(source[pos]))
      pos++;
    return source[pos];
  }

  bool consume(char c) {
    if (peek() == c) {
      pos++;
      return true;
    }
    return false;
  }

public:
  std::map<std::string, double> user_constants;
  EvaluationResult evaluate_expression(std::string expr) {
    source = expr;
    pos = 0;
    auto res = expression(1);

    // Must consume all.
    if (peek() != '\0')
      return EvaluationError{pos, "Unexpected character",
                             std::string(1, source[pos]), expr};

    // na para bang christmas gift
    if (auto err = std::get_if<EvaluationError>(&res)) {
      err->expr = expr;
      res = *err;
    }

    return res;
  }
};

EvaluationResult
evaluate_expression(std::string expr,
                    std::map<std::string, double> user_constants) {
  Evaluator p;
  p.user_constants = user_constants;
  return p.evaluate_expression(expr);
}

#if defined(_WIN32)
#include <windows.h>
#endif

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

static bool ANSI_ENABLED = true;

void toggle_ansi(bool state) { ANSI_ENABLED = state; }

bool try_enable_ansi() {
#if defined(_WIN32)
  HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD mode;
  if (h == INVALID_HANDLE_VALUE || !GetConsoleMode(h, &mode))
    return 0; /* not a console (redirected/piped) */
  if (mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING)
    return 1;
  return SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
#elif defined(__unix__)
  // Assume always yes.
  // TODO maybe NO_COLOR env var?
  return true;
#endif
  return false;
}

bool is_ansi_enabled() { return ANSI_ENABLED; }

std::string style_bold(std::string s) {
  if (!ANSI_ENABLED)
    return s;
  return "\033[1m" + s + RESET_STYLE;
}
std::string style_italic(std::string s) {
  if (!ANSI_ENABLED)
    return s;
  return "\033[3m" + s + RESET_STYLE;
}
std::string style_underlined(std::string s) {
  if (!ANSI_ENABLED)
    return s;
  return "\033[4m" + s + RESET_STYLE;
}
std::string style_strikethrough(std::string s) {
  if (!ANSI_ENABLED)
    return s;
  return "\033[9m" + s + RESET_STYLE;
}

std::string style_info(std::string s) {
  if (!ANSI_ENABLED)
    return s;
  return style_underlined(s) + RESET_STYLE;
}
std::string style_error(std::string s) {
  if (!ANSI_ENABLED)
    return s;
  return "\033[38;5;31m" + style_underlined(style_bold(s)) + RESET_STYLE;
}
std::string style_input_primary(std::string s) {
  if (!ANSI_ENABLED)
    return s;
  return "\033[38;5;34m" + style_italic(s) + RESET_STYLE;
}
std::string style_input_secondary(std::string s) {
  if (!ANSI_ENABLED)
    return s;
  return "\033[38;5;32m" + style_italic(s) + RESET_STYLE;
}
std::string style_output_primary(std::string s) {
  if (!ANSI_ENABLED)
    return s;
  return "\033[38;5;33m" + style_bold(s) + RESET_STYLE;
}
std::string style_output_secondary(std::string s) {
  if (!ANSI_ENABLED)
    return s;
  return "\033[38;5;36m" + style_bold(s) + RESET_STYLE;
}

#include <algorithm>
#include <iostream>

bool fits_long(double d) {
  constexpr double lo = static_cast<double>(std::numeric_limits<long>::min());
  constexpr double hi = -lo;
  return d >= lo && d < hi;
}
unsigned long uabs(long x) {
  return x < 0 ? 0 - static_cast<unsigned long>(x)
               : static_cast<unsigned long>(x);
}

bool is_num(char c) { return c >= '0' && c <= '9'; }
bool is_alpha(char c) {
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}
bool is_whitespace(char c) { return (c == ' ') || (c == '\n') || (c == '\t'); }
bool is_alphanum(char c) { return is_num(c) || is_alpha(c); }

std::string trim_string(const std::string s) {
  std::string res = s;
  while (!res.empty() && is_whitespace(res.front()))
    res.erase(0, 1);
  while (!res.empty() && is_whitespace(res.back()))
    res.erase(res.size() - 1, 1);
  // Unicode content has their most significant bit set.
  for (size_t i = 0; i < res.size(); i++) {
    if (res[i] & 0x80)
      res.replace(i, 1, "?");
  }
  return res;
}

std::string lower_case_string(const std::string s) {
  std::string res = s;
  std::transform(res.begin(), res.end(), res.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return res;
}

std::pair<std::string, std::string> consume_front_string(const std::string s) {
  std::string remaining = s;
  size_t i = 0;
  while (is_whitespace(remaining[i]))
    i++;
  remaining.erase(0, i);

  i = 0;
  while (is_alphanum(remaining[i]))
    i++;
  auto consumed = remaining.substr(0, i);

  while (is_whitespace(remaining[i]))
    i++;
  remaining.erase(0, i);

  return {consumed, remaining};
}

// Returns the index into items of the closest match to the key.
// Returns -1 if not found, or -2 if found two or more matches.
// Not really smart, but it works well.
int smart_find_index(const std::vector<std::string> items,
                     const std::string key) {
  // if exact match was found, return that immediately.
  if (auto it = std::find(items.begin(), items.end(), key); it != items.end()) {
    return static_cast<int>(std::distance(items.begin(), it));
  }

  auto transformed_key = lower_case_string(key);
  // Remove every whitespace
  for (size_t i = 0; i < transformed_key.size(); i++) {
    if (is_whitespace(transformed_key[i]))
      transformed_key.erase(i, 1);
  }
  auto transformed_items = items;
  // Here too.
  std::transform(transformed_items.begin(), transformed_items.end(),
                 transformed_items.begin(), [](const std::string s) {
                   std::string res = s;
                   for (size_t i = 0; i < res.size(); i++) {
                     if (is_whitespace(res[i]))
                       res.erase(i, 1);
                   }
                   return lower_case_string(res);
                 });

  std::string current_key;
  size_t key_idx = 0;
  int match_idx = -1;
  bool match = false;

  while (!match && key_idx < key.size()) {
    current_key.push_back(transformed_key[key_idx++]);
    match_idx = -1;
    match = false;

    for (size_t i = 0; i < transformed_items.size(); i++) {
      // is current_key a prefix of i?
      auto res = std::mismatch(current_key.begin(), current_key.end(),
                               transformed_items[i].begin());

      if (res.first == current_key.end()) {
        // first match of current key
        if (!match) {
          match = true;
          match_idx = static_cast<int>(i);
        } else {
          // second+ match, ambiguous
          match = false;
          match_idx = -2;
          break;
        }
      }
    }
  }

  return match_idx;
}

bool is_eval_error(EvaluationResult res) {
  return std::holds_alternative<EvaluationError>(res);
}

bool handle_eval_error(EvaluationResult *res, bool report_expr) {
  if (auto err = std::get_if<EvaluationError>(res)) {
    std::string expr_report;
    if (report_expr) {
      expr_report = " ('" + err->expr + "')";
    }
    std::cout << style_error("ERROR: " + err->message + " (" + err->offending +
                             ") at position " + std::to_string(err->pos) + "." +
                             expr_report)
              << std::endl;
    return true;
  }
  return false;
}
