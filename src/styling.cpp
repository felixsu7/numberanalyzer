#include "styling.hpp"

static bool ANSI_ENABLED = true;

void toggle_ansi(bool state) { ANSI_ENABLED = state; }

bool try_enable_ansi() {
  // intentionally excluded from NO_PLATFORM_SPECIFIC
#if defined(_WIN32)
  // TODO;
  return false;
#elif defined(__unix__)
  // Assume always yes.
  // TODO maybe NO_COLOR env var?
  return true;
#endif
  return false;
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
