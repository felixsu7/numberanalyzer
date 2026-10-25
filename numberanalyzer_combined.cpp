

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

AnalysisFuncDef num_sign;
AnalysisFuncDef num_even_odd;
AnalysisFuncDef num_divisbility;
AnalysisFuncDef num_factors;
AnalysisFuncDef num_factors_amount;
AnalysisFuncDef num_prime_composite;
AnalysisFuncDef num_factors_sum;
AnalysisFuncDef num_perfect;
AnalysisFuncDef num_collatz;
AnalysisFuncDef num_prime_factors;
AnalysisFuncDef num_perfect_square_cube;
AnalysisFuncDef num_is_triangular;
// I guess this is also considered as 12. Triangular Number
AnalysisFuncDef num_nth_triangular;

static const std::map<std::string, AnalysisFuncDef *> analysis_funcs = {
    {"Positive/Negative/Zero", num_sign},
    {"Even/Odd", num_even_odd},
    {"Divisibility", num_divisbility},
    {"Factors", num_factors},
    {"Number of Factors", num_factors_amount},
    {"Prime or composite", num_prime_composite},
    {"Sum of Factors", num_factors_sum},
    {"Perfect Number", num_perfect},
    {"Collatz Conjecture Sequence", num_collatz},
    {"Prime Factorization", num_prime_factors},
    {"Perfect Square/Cube", num_perfect_square_cube},
    {"Triangular Number", num_is_triangular},
    {"nth Triangular Number", num_nth_triangular},
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

NumberAnalysis analyze_number(int64_t num,
                              const std::map<std::string, bool> enabled);

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
      "anything else.",
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
     {cmd_history, "Views past analyzed numbers during this session.", ""}}};

CommandResult execute_command(const std::string input, ProgramState *state);

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
};

using EvaluationResult = std::variant<double, EvaluationError>;

EvaluationResult
evaluate_expression(const std::string expr,
                    const std::map<std::string, double> user_constants);

#if defined(_WIN32)
#define PLAT_WINDOWS 1
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif
#include <cstdint>
#include <windows.h>
#elif defined(__unix__)
#define PLAT_UNIX
#include <termios.h>
#include <unistd.h>
#else
#error "Unknown platform"
#endif

enum class PLATFORM_ERROR {
  NONE,
  UNIX_INIT,
  UNIX_TEARDOWN,
  UNIX_INPUT,
  WINDOWS_INIT,
  WINDOWS_TEARDOWN,
  WINDOWS_INPUT
};

enum class PLATFORM_EVENT_TYPE { KEY, MOUSE, RESIZE, QUIT, EERROR };

enum class PLATFORM_EVENT_MOUSE_KEY {
  NONE,
  MOVE,
  LEFT,
  RIGHT,
  MIDDLE,
  SCROLL_DOWN,
  SCROLL_UP
};

// yes its big and i think its a problem
struct PlatformEvent {
  PLATFORM_EVENT_TYPE type;

  bool shift;
  bool alt;
  bool control;

  char key;

  PLATFORM_EVENT_MOUSE_KEY mouse_key;
  // points to a character, not exact pixel values
  // only valid when mouse_key is move
  uint16_t mouse_x;
  uint16_t mouse_y;

  // if both zero, it hasnt changed
  uint16_t new_term_width;
  uint16_t new_term_height;

  PLATFORM_ERROR error;
};

// begins and ends the TUI mode
// just only do the platform specific stuff then ansi stuff on
// screen.activate only
PLATFORM_ERROR platform_term_activate();
PLATFORM_ERROR platform_term_deactivate();

// is buffered, so a long string always come fine or there will be no missed
// events, should handle pastes properly as well
PLATFORM_ERROR platform_poll_event(uint64_t timeout_millis, PlatformEvent *out);

// returns a monotonic milli timestamp starting from a certain point
// so this is not starting from unix epoch
uint64_t platform_monotime();

#include <array>
#include <cstdint>
#include <string>
#include <vector>

struct Color {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  bool active; // if false, ignore colors and use default term colors
};

enum CellAttr : uint32_t {
  ATTR_NONE = 0,
  ATTR_BOLD = 1 << 0,
  ATTR_DIM = 1 << 1,
  ATTR_ITALIC = 1 << 2,
  ATTR_UNDERLINE = 1 << 3,
  ATTR_BLINK = 1 << 4,
  ATTR_REVERSE = 1 << 5,
  ATTR_HIDDEN = 1 << 6,
  ATTR_STRIKE = 1 << 7,
};

struct Cell {
  std::array<char, 16> cp;
  int width;
  Color fg;
  Color bg;
  CellAttr attrs;
};

void cell_set_glyph(Cell *c, const char *s, int width);

inline const Cell default_cell =
    Cell{{' '}, 1, {0, 0, 0, false}, {0, 0, 0, false}, ATTR_NONE};

class Screen {
public:
  Screen(int w, int h);
  ~Screen();
  PLATFORM_ERROR activate();
  PLATFORM_ERROR deactivate();
  void put(int x, int y, Cell c);
  void resize(int w, int h);
  void present();
  void clear();
  // reports the current screen dimensions, not the posssibly new term
  // dimensions
  int width();
  int height();

  void move_cursor(int x, int y);
  void toggle_cursor(bool state);

  PlatformEvent event_poll(uint64_t timeout_millis);

  void draw_hline(int x, int y, int w, Cell c);
  void draw_vline(int x, int y, int h, Cell c);
  void draw_line(int x1, int y1, int x2, int y2, Cell c);
  void draw_rect(int x, int y, int w, int h, Cell c);
  void draw_text(int x, int y, std::string text, Cell style);

private:
  bool activated;
  int _w, _h;
  std::vector<Cell> front_framebuffer;
  std::vector<Cell> back_framebuffer;
  uint64_t last_draw;
};

#include <string>

const std::string RESET_STYLE = "\033[0m";
void toggle_ansi(bool state);
bool try_enable_ansi();

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

bool is_error(EvaluationResult res);
bool handle_error(EvaluationResult *res);

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

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>

static const std::string endl = "\n";
static std::stringstream cout = std::stringstream();

// The order of definitions was based on when they were submitted

// It should be noted that I modified their submissions, mainly on checks, the
// mathematical basis is kept exactly as they intended.

// Originally, this accepted two inputs.
// However, I turned it into analysis of what numbers does it divide cleanly
// with. Checking from 1 to N.
void num_divisbility(int64_t num) {
  if (num == 0) {
    cout << "Zero divided by any number is always zero." << endl;
    return;
  };

  // Regular abs is erroring out in large enough negative values, so we handle
  // negatives seperately, check utils.cpp
  cout << num << " is divisible by ";
  for (uint64_t d = 1; d <= uabs(num); d++) {
    if (uabs(num) % d == 0) {
      cout << d << ", ";
    }
  }

  cout << endl;
}

void num_sign(int64_t num) {
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

void num_prime_factors(int64_t num) {
  auto prime_factors = _primefactors(num);

  // TODO real ba?
  if (prime_factors.empty()) {
    cout << num << " has no prime factors." << endl;
    return;
  }

  for (auto pf : prime_factors) {
    cout << pf << ", ";
  }

  cout << endl;
}

void num_perfect_square_cube(int64_t num) {
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

void num_even_odd(int64_t num) {
  if (num % 2 == 0) {
    cout << num << " is an even number." << endl;
  } else {
    cout << num << " is an odd number." << endl;
  }
}

void num_factors_sum(int64_t num) {
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

void num_factors_amount(int64_t num) {
  int64_t factorCount = 0;

  if (num == 0) {
    cout << "Zero has infinitely many factors.";
  } else {
    for (int divisor = 1; divisor <= abs(num); divisor++) {
      if (num % divisor == 0) {
        factorCount += 2;
      }
    }

    cout << num << " has " << factorCount << " factor(s)." << endl;
  }
}

void num_prime_composite(int64_t num) {
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

void num_collatz(int64_t num) {
  int64_t Colture = num;

  // I added this check because it is infinitely looping on negative and zeroes.
  if (num <= 0) {
    cout << "ERROR: Input cannot be negative or zero." << endl;
    return;
  }

  // Without limiting to N iterations, this would just lag the entire program.
  const int COLLATZ_ITERATIONS_LIMIT = 100;
  int iteration = 0;

  while (Colture != 1 && iteration < COLLATZ_ITERATIONS_LIMIT) {
    cout << Colture << ", ";

    if (Colture % 2 == 0) {
      Colture = Colture / 2;
    } else {
      Colture = (Colture * 3) + 1;
    }

    iteration++;
  }

  if (iteration >= COLLATZ_ITERATIONS_LIMIT) {
    cout << "..., ";
  }

  cout << 1 << endl;
}

void num_perfect(int64_t num) {
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

std::string num_suffix(int64_t num) {
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

void num_nth_triangular(int64_t num) {
  if (num < 0) {
    cout << "ERROR: Input cannot be negative." << endl;
    return;
  }

  // Triangular number: n(n+1)/2
  // TODO: change th to proper suffix
  cout << num << num_suffix(num) << " triangular number is "
       << (num * (num + 1)) / 2 << endl;
}

void num_is_triangular(int64_t num) {
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

void num_factors(int64_t num) { cout << "NOT IMPLEMENTED YET." << endl; }

void dump_logs(const std::string name, string_vec_map *logs) {
  std::string line;
  while (std::getline(cout, line, '\n')) {
    (*logs)[name].push_back(line);
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
    dump_logs(name, &analysis.logs);
  }

  // TODO: process errors

  return analysis;
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

  for (auto af : analysis_funcs_order) {
    // if disabled yet somehow exists in logs, skip.
    // also includes sanity checks
    if (auto it = state->enabled_analysis.find(af);
        it != state->enabled_analysis.end() && !it->second &&
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
    return CommandResult::AMBIGIOUS_COMMAND;
  }

  auto name = command_keys[static_cast<size_t>(idx)];
  auto it = cmd_funcs.find(name);
  return it->second.func(consumed.second, state);
};

#include <cstdlib>
#include <string>
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
    if (is_error(left_res))
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
      if (is_error(right_res))
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
          return EvaluationError{op_pos, "Division by zero", "0"};
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
      if (is_error(sub_res))
        return sub_res;

      if (!consume(')')) {
        return EvaluationError{pos, "Missing ending parenthesis", ")"};
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
            if (is_error(arg_res))
              return arg_res;
            args.push_back(std::get<double>(arg_res));
          } while (peek() == ',' && pos++);
        if (!consume(')')) {
          return EvaluationError{pos, "Missing ending parenthesis", ")"};
        };

        auto it = math_funcs.find(id);
        if (it == math_funcs.end())
          return EvaluationError{start, "Unknown function", id};

        if (args.size() != it->second.arity)
          return EvaluationError{
              start, "Function " + id + " got wrong amount of arguments",
              std::to_string(args.size())};

        double val = it->second.func(args);
        if (!std::isfinite(val))
          return EvaluationError{start, "Function domain error", id};

        return val;
      } else {
        // Constants
        // TODO take in a map of consts
        auto it = user_constants.find(id);
        if (it == user_constants.end())
          return EvaluationError{start, "Unknown constant", id};
        return it->second;
      }
    }

    if (consume('-')) {
      auto neg_res = expression(4);
      if (is_error(neg_res))
        return neg_res;
      return -std::get<double>(neg_res);
    }

    // Integers, decimals, and somehow hexidecimals for free.
    char *end;
    double decimal = std::strtod(source.c_str() + pos, &end);
    if (end == source.c_str() + pos)
      return EvaluationError{pos, "Expected number", ""};

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
                             std::string(1, source[pos])};

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

#ifdef PLAT_WINDOWS

HANDLE console_input = INVALID_HANDLE_VALUE;
HANDLE console_output = INVALID_HANDLE_VALUE;
DWORD original_input_mode = 0;
DWORD original_output_mode = 0;

PLATFORM_ERROR platform_term_activate() {
  console_input = GetStdHandle(STD_INPUT_HANDLE);

  if (console_input == nullptr || console_input == INVALID_HANDLE_VALUE ||
      console_output == nullptr || console_output == INVALID_HANDLE_VALUE ||
      !GetConsoleMode(console_input, &original_input_mode) ||
      !GetConsoleMode(console_output, &original_output_mode)) {
    return PLATFORM_ERROR::WINDOWS_INIT;
  }

  DWORD input_mode =
      original_input_mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT |
                              ENABLE_PROCESSED_INPUT | ENABLE_QUICK_EDIT_MODE);
  input_mode |=
      ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
  if (!SetConsoleMode(console_input, input_mode))
    return PLATFORM_ERROR::WINDOWS_INIT;

  console_output = GetStdHandle(STD_OUTPUT_HANDLE);

  if (!SetConsoleMode(console_output, original_output_mode |
                                          ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
    SetConsoleMode(console_input, original_input_mode);
    console_input = INVALID_HANDLE_VALUE;
    return PLATFORM_ERROR::WINDOWS_INIT;
  }

  return PLATFORM_ERROR::NONE;
}

PLATFORM_ERROR platform_term_deactivate() {
  bool restore_failed = false;

  if (SetConsoleMode(console_input, original_input_mode)) {
    console_input = INVALID_HANDLE_VALUE;
  } else {
    restore_failed = true;
  }

  if (SetConsoleMode(console_output, original_output_mode)) {
    console_output = INVALID_HANDLE_VALUE;
  } else {
    restore_failed = true;
  }

  return restore_failed ? PLATFORM_ERROR::WINDOWS_TEARDOWN
                        : PLATFORM_ERROR::NONE;
}

PLATFORM_ERROR platform_poll_event(uint64_t timeout_millis,
                                   PlatformEvent *out) {
  const ULONGLONG started = GetTickCount64();
  for (;;) {
    DWORD wait_millis = 0;
    if (timeout_millis != 0) {
      const ULONGLONG elapsed = GetTickCount64() - started;
      if (elapsed >= timeout_millis)
        return PLATFORM_ERROR::NONE;
      const uint64_t remaining = timeout_millis - elapsed;
      wait_millis = remaining >= static_cast<uint64_t>(INFINITE)
                        ? INFINITE - 1
                        : static_cast<DWORD>(remaining);
    }

    const DWORD wait_result = WaitForSingleObject(console_input, wait_millis);
    if (wait_result == WAIT_TIMEOUT) {
      if (timeout_millis == 0 || GetTickCount64() - started >= timeout_millis) {
        return PLATFORM_ERROR::NONE;
      }
      continue;
    }
    if (wait_result != WAIT_OBJECT_0)
      return PLATFORM_ERROR::WINDOWS_INIT;

    INPUT_RECORD record;
    DWORD records_read = 0;
    if (!ReadConsoleInputW(console_input, &record, 1, &records_read) ||
        records_read == 0) {
      return PLATFORM_ERROR::WINDOWS_INIT;
    }

    PlatformEvent event{};
    if (record.EventType == KEY_EVENT) {
      const KEY_EVENT_RECORD &key = record.Event.KeyEvent;
      if (!key.bKeyDown)
        continue;

      event.type = PLATFORM_EVENT_TYPE::KEY;
      event.glyph = key.uChar.UnicodeChar;
      event.shift = (key.dwControlKeyState & SHIFT_PRESSED) != 0;
      event.alt =
          (key.dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED)) != 0;
      event.control = (key.dwControlKeyState &
                       (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) != 0;
      *out = event;
      return PLATFORM_ERROR::NONE;
    }

    if (record.EventType == MOUSE_EVENT) {
      const MOUSE_EVENT_RECORD &mouse = record.Event.MouseEvent;
      event.type = PLATFORM_EVENT_TYPE::MOUSE;
      event.mouse_x = mouse.dwMousePosition.X < 0
                          ? 0
                          : static_cast<uint16_t>(mouse.dwMousePosition.X);
      event.mouse_y = mouse.dwMousePosition.Y < 0
                          ? 0
                          : static_cast<uint16_t>(mouse.dwMousePosition.Y);

      if ((mouse.dwEventFlags & MOUSE_WHEELED) != 0) {
        const SHORT wheel_delta =
            static_cast<SHORT>(HIWORD(mouse.dwButtonState));
        event.mouse_key = wheel_delta > 0
                              ? PLATFORM_EVENT_MOUSE_KEY::SCROLL_UP
                              : PLATFORM_EVENT_MOUSE_KEY::SCROLL_DOWN;
      } else if ((mouse.dwEventFlags & MOUSE_MOVED) != 0) {
        event.mouse_key = PLATFORM_EVENT_MOUSE_KEY::MOVE;
      } else {
        if ((mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) != 0)
          event.mouse_key = PLATFORM_EVENT_MOUSE_KEY::LEFT;
        else if ((mouse.dwButtonState & RIGHTMOST_BUTTON_PRESSED) != 0)
          event.mouse_key = PLATFORM_EVENT_MOUSE_KEY::RIGHT;
        else if ((mouse.dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED) != 0)
          event.mouse_key = PLATFORM_EVENT_MOUSE_KEY::MIDDLE;
        else
          event.mouse_key = PLATFORM_EVENT_MOUSE_KEY::NONE;
      }

      *out = event;
      return PLATFORM_ERROR::NONE;
    }

    if (record.EventType == WINDOW_BUFFER_SIZE_EVENT) {
      const COORD size = record.Event.WindowBufferSizeEvent.dwSize;
      event.type = PLATFORM_EVENT_TYPE::RESIZE;
      event.new_term_width = size.X < 0 ? 0 : static_cast<uint16_t>(size.X);
      event.new_term_height = size.Y < 0 ? 0 : static_cast<uint16_t>(size.Y);
      *out = event;
      return PLATFORM_ERROR::NONE;
    }
  }
}

uint64_t platform_monotime() { return static_cast<uint64_t>(GetTickCount64()); }

#endif

#ifdef PLAT_UNIX

#include <cstdlib>
#include <ctime>
#include <poll.h>
#include <termios.h>
#include <unistd.h>

static termios orig_termstate;

// Reference: https://linux.die.net/man/3/tcgetattr

PLATFORM_ERROR platform_term_activate() {
  int tty = open("/dev/tty", O_RDWR | O_CLOEXEC);
  if (tty == -1) {
    // perhaps be more specific????
    return PLATFORM_ERROR::UNIX_INIT;
  }
  if (tcgetattr(tty, &orig_termstate) == -1)
    return PLATFORM_ERROR::UNIX_INIT;

  termios t = orig_termstate;
  t.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  t.c_oflag &= ~(OPOST); /* optional, see above */
  t.c_cflag |= CS8;
  // t.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  t.c_lflag &= ~(ECHO | ICANON | IEXTEN);
  t.c_cc[VMIN] = 0;
  t.c_cc[VTIME] = 0;

  if (tcsetattr(tty, TCSAFLUSH, &t) == -1)
    return PLATFORM_ERROR::UNIX_INIT;

  // normally we would setup atexit here but we let the app code do that i guess

  return PLATFORM_ERROR::NONE;
};

PLATFORM_ERROR platform_term_deactivate() {
  // TODO: question, when do we swap to the original alternative screen buffer?
  // on app or here?
  int tty = open("/dev/tty", O_RDWR | O_CLOEXEC);
  if (tty == -1) {
    return PLATFORM_ERROR::UNIX_TEARDOWN;
  }
  if (tcsetattr(tty, TCSAFLUSH, &orig_termstate) == -1)
    return PLATFORM_ERROR::UNIX_TEARDOWN;

  return PLATFORM_ERROR::NONE;
};

// TODO better
PLATFORM_ERROR platform_poll_event(uint64_t timeout_millis,
                                   PlatformEvent *out) {
  memset(out, 0, sizeof(PlatformEvent));
  char c = 0;
  struct pollfd pfd = {};
  pfd.fd = STDIN_FILENO;
  pfd.events = POLLIN;
  int n = poll(&pfd, 1, timeout_millis);
  if (n < 0) {
    if (errno == EINTR) {
      return PLATFORM_ERROR::NONE;
    }
    return PLATFORM_ERROR::UNIX_INPUT;
  }
  if (n == 0) {
    return PLATFORM_ERROR::NONE;
  }
  if (pfd.revents & (POLLERR | POLLNVAL)) {
    return PLATFORM_ERROR::UNIX_INPUT;
  }

  if (pfd.revents & (POLLIN | POLLHUP)) {
    ssize_t r = read(STDIN_FILENO, &c, 1);
    if (r == 1) {
      out->key = c;
    } else if (r == 0) {
      return PLATFORM_ERROR::UNIX_INPUT;
    } else if (errno != EINTR) {
      return PLATFORM_ERROR::UNIX_INPUT;
    }
  }

  return PLATFORM_ERROR::NONE;
};

uint64_t platform_monotime() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return static_cast<uint64_t>(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
};

#endif


#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

// TODO: support unicode outout

// ANSI Sequences Reference:
// https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797

// kung mabait ka compiler syempre iooptimize mo yan properly and not one by one
// <3
static const std::string HIDE_CURSOR = "\033[?25l";
static const std::string SHOW_CURSOR = "\033[?25h";
static const std::string RESTORE_SCREEN = "\033[?47l";
static const std::string SAVE_SCREEN = "\033[?47h";
static const std::string ENABLE_ALT_BUFFER = "\033[?1049h";
static const std::string DISABLE_ALT_BUFFER = "\033[?1049l";
static const std::string CLEAR_SCREEN = "\033[2J";
static const std::string HOME_CURSOR = "\033[H";
static const std::string TERM_SYNC_BEGIN = "\033[?2026h";
static const std::string TERM_SYNC_END = "\033[?2026l";

static const int SCREEN_FPS = 30;

Screen::Screen(int w, int h) {
  _w = w;
  _h = h;
  resize(w, h);
  last_draw = 0;
}

// TODO make this work
Screen::~Screen() {
  if (activated) {
    deactivate();
  }
}

PLATFORM_ERROR Screen::activate() {
  PLATFORM_ERROR err = platform_term_activate();
  if (err != PLATFORM_ERROR::NONE) {
    // report error
    return err;
  }

  std::cout << ENABLE_ALT_BUFFER << HIDE_CURSOR << CLEAR_SCREEN << HOME_CURSOR
            << std::flush;

  activated = true;

  return PLATFORM_ERROR::NONE;
}

PLATFORM_ERROR Screen::deactivate() {
  PLATFORM_ERROR err = platform_term_deactivate();
  if (err != PLATFORM_ERROR::NONE) {
    // report error
    return err;
  }

  std::cout << RESET_STYLE << SHOW_CURSOR << DISABLE_ALT_BUFFER << std::flush;

  activated = false;

  return PLATFORM_ERROR::NONE;
}

void Screen::resize(int w, int h) {
  front_framebuffer.resize(static_cast<size_t>(w * h));
  // do we need to reserve?
  front_framebuffer.reserve(static_cast<size_t>(w * h));
  back_framebuffer.resize(static_cast<size_t>(w * h));
  back_framebuffer.reserve(static_cast<size_t>(w * h));
  clear();
}

void Screen::clear() {
  std::transform(front_framebuffer.begin(), front_framebuffer.end(),
                 front_framebuffer.begin(),
                 [](Cell _) { return default_cell; });

  std::transform(back_framebuffer.begin(), back_framebuffer.end(),
                 back_framebuffer.begin(), [](Cell _) { return default_cell; });

  // we dont explicitly present here
}

void Screen::put(int x, int y, Cell c) {
  // reject out of bounds
  if (((x + c.width - 1) < 0) || ((x + c.width - 1) >= _w) || (y < 0) ||
      (y >= _h)) {
    return;
  }

  back_framebuffer[static_cast<size_t>((y * _w) + x)] = c;

  for (int i = 1; i < c.width; i++) {
    Cell spacer = c;
    spacer.cp = {" "};
    back_framebuffer[static_cast<size_t>((y * _w) + x + i)] = spacer;
  }
}

std::string move_cursor_seq(int x, int y) {
  return "\033[" + std::to_string(y + 1) + ";" + std::to_string(x + 1) + "H";
}

std::string toggle_cursor_seq(bool state) {
  if (state) {
    return SHOW_CURSOR;
  } else {
    return HIDE_CURSOR;
  }
}

std::string style_delta_seq(CellAttr attrs, CellAttr old, CellAttr style,
                            std::string set_seq, std::string reset_seq) {
  if ((attrs & style) == (old & style))
    return "";

  if (attrs & style) {
    return set_seq;
  } else {
    return reset_seq;
  }
}

void cell_set_glyph(Cell *c, const char *s, int width) {
  if (*s == 0) {
    cell_set_glyph(c, " ", 1);
    return;
  }

  memset(&c->cp, 0, sizeof(c->cp));
  for (size_t i = 0; i < 16; i++) {
    if (s[i] == 0)
      break;
    c->cp[i] = s[i];
  }
  c->width = width;
}

std::string cell_seq(Cell c, Cell old) {
  std::string seq;
  bool reset = false;

  // We immediately intercept degenerate glyphs.
  if (c.cp[0] == 0) {
    c.cp[0] = ' ';
    c.cp[1] = 0;
  }

  // We can't reset fg or bg individually, only reset all.
  if ((!c.fg.active && old.fg.active) || (!c.bg.active && old.bg.active)) {
    seq += "\033[0m";
    reset = true;
  }

  if (c.fg.active && (reset || (memcmp(&c.fg, &old.fg, sizeof(c.fg)) != 0))) {
    seq += "\033[38;2;";
    seq += std::to_string(c.fg.red) + ';';
    seq += std::to_string(c.fg.green) + ';';
    seq += std::to_string(c.fg.blue) + 'm';
  }

  if (c.bg.active && (reset || (memcmp(&c.bg, &old.bg, sizeof(c.bg)) != 0))) {
    seq += "\033[48;2;";
    seq += std::to_string(c.bg.red) + ';';
    seq += std::to_string(c.bg.green) + ';';
    seq += std::to_string(c.bg.blue) + 'm';
  }

  if ((c.attrs != old.attrs) || (reset && c.attrs != ATTR_NONE)) {
    seq += "\033[";
    CellAttr old_attrs = old.attrs;
    if (reset)
      old_attrs = ATTR_NONE;
    // Same reset seq, should consider
    seq += style_delta_seq(c.attrs, old_attrs, ATTR_BOLD, "1;", "22;");
    seq += style_delta_seq(c.attrs, old_attrs, ATTR_DIM, "2;", "22;");
    seq += style_delta_seq(c.attrs, old_attrs, ATTR_ITALIC, "3;", "23;");
    seq += style_delta_seq(c.attrs, old_attrs, ATTR_UNDERLINE, "4;", "24;");
    seq += style_delta_seq(c.attrs, old_attrs, ATTR_BLINK, "5;", "25;");
    seq += style_delta_seq(c.attrs, old_attrs, ATTR_REVERSE, "7;", "27;");
    seq += style_delta_seq(c.attrs, old_attrs, ATTR_HIDDEN, "8;", "28;");
    seq += style_delta_seq(c.attrs, old_attrs, ATTR_STRIKE, "9;", "29;");

    // replace last semicolon with m
    seq.pop_back();
    seq += 'm';
  }

  for (size_t i = 0; i < 16; i++) {
    if (c.cp[i] == 0)
      break;
    seq.push_back(c.cp[i]);
  }
  return seq;
}

void Screen::present() {
  // Limit according to FPS
  const uint64_t now = platform_monotime();
  if ((last_draw + static_cast<uint64_t>(1.0 / SCREEN_FPS) * 1000) > now) {
    std::transform(back_framebuffer.begin(), back_framebuffer.end(),
                   back_framebuffer.begin(),
                   [](Cell _) { return default_cell; });
    return;
  }

  std::string payload;
  // std::cout << std::endl << "============ NEW FRAME ==========" << std::endl;
  payload += TERM_SYNC_BEGIN + HOME_CURSOR + RESET_STYLE;

  // push (back - front) diffs to term
  Cell old_cell = default_cell;
  for (int y = 0; y < _h; y++) {
    int last_change_pos = 0;
    for (int x = 0; x < _w; x++) {
      Cell f = front_framebuffer[static_cast<size_t>((y * _w) + x)];
      Cell b = back_framebuffer[static_cast<size_t>((y * _w) + x)];

      // safe, all set to zero and no paddings on Cell.
      // TODO: confirm if thats still the case
      if (memcmp(&f, &b, sizeof(Cell)) != 0) {
        // Don't emit a cursor move if we are just moving to the right, saves a
        // lot.
        if (last_change_pos != x - 1)
          payload += move_cursor_seq(x, y);
        payload += cell_seq(b, old_cell);
        old_cell = b;
        last_change_pos = y * _w + x;
      }
    }
  }

  payload += TERM_SYNC_END;

  std::cout << payload << std::flush;
  last_draw = now;

  std::swap(front_framebuffer, back_framebuffer);
  std::transform(back_framebuffer.begin(), back_framebuffer.end(),
                 back_framebuffer.begin(), [](Cell _) { return default_cell; });
}

void Screen::draw_hline(int x, int y, int w, Cell c) {
  // perhaps this can optimized to something like memset?
  // its gonna be used by alot of functions
  for (int dx = 0; dx < w; dx++) {
    put(x + dx, y, c);
  }
};

void Screen::draw_vline(int x, int y, int h, Cell c) {
  for (int dy = 0; dy < h; dy++) {
    put(x, y + dy, c);
  }
};

void Screen::draw_line(int x1, int y1, int x2, int y2, Cell c) {
  // classic bresenham has issues so we adapt this
  // https://haqr.eu/tinyrenderer/bresenham/
  bool steep = std::abs(x1 - x2) < std::abs(y1 - y2);
  if (steep) { // if the line is steep, we transpose the image
    std::swap(x1, y1);
    std::swap(x2, y2);
  }
  if (x1 > x2) { // make it left−to−right
    std::swap(x1, x2);
    std::swap(y1, y2);
  }
  for (int x = x1; x <= x2; x++) {
    double t = static_cast<double>(x - x1) / static_cast<double>(x2 - x1);
    int y = static_cast<int>(std::round(y1 + (y2 - y1) * t));
    if (steep) // if transposed, de−transpose
      put(x, y, c);
    else
      put(x, y, c);
  }
}

void Screen::draw_rect(int x, int y, int w, int h, Cell c) {
  for (int dy = 0; dy < h; dy++) {
    draw_hline(x, y + dy, w, c);
  }
};

// doesnt support unicode btw.
void Screen::draw_text(int x, int y, const std::string text, Cell style) {
  // guessing this is unoptimal
  int dx = 0;
  Cell c = style;
  c.width = 1;
  memset(&c.cp, 0, sizeof(c.cp));

  for (auto ch : text) {
    if (static_cast<unsigned char>(ch) & 0x80)
      continue;

    c.cp[0] = ch;
    put(x + dx, y, c);
    dx++;
  }
};

PlatformEvent Screen::event_poll(uint64_t timeout_millis) {
  PlatformEvent ev;
  platform_poll_event(timeout_millis, &ev);
  return ev;
}

int Screen::width() { return _w; }

int Screen::height() { return _h; }

static bool ANSI_ENABLED = true;

void toggle_ansi(bool state) { ANSI_ENABLED = state; }

bool try_enable_ansi() {
  // TODO
  return true;
}

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
#include <cmath>
#include <iostream>

bool fits_long(double d) {
  constexpr double lo = static_cast<double>(std::numeric_limits<long>::min());
  constexpr double hi = -lo;
  return d >= lo && d < hi && d == std::trunc(d);
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
int smart_find_index(const std::vector<std::string> items,
                     const std::string key) {
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
          // second+ match, ambigious
          match = false;
          match_idx = -2;
          break;
        }
      }
    }
  }

  return match_idx;
}

bool is_error(EvaluationResult res) {
  return std::holds_alternative<EvaluationError>(res);
}

bool handle_error(EvaluationResult *res) {
  if (auto err = std::get_if<EvaluationError>(res)) {
    std::cout << style_error("ERROR: " + err->message + " (" + err->offending +
                             ") at position " + std::to_string(err->pos) + ".")
              << std::endl;
    return true;
  }
  return false;
}
