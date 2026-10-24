#pragma once

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
