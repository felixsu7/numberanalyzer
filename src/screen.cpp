#include "screen.hpp"
#include "platform.hpp"
#include <algorithm>
#include <cstddef>
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
inline static const std::string HIDE_CURSOR = "\033[?25l";
inline static const std::string SHOW_CURSOR = "\033[?25h";
inline static const std::string RESTORE_SCREEN = "\033[?47l";
inline static const std::string SAVE_SCREEN = "\033[?47h";
inline static const std::string ENABLE_ALT_BUFFER = "\033[?1049h";
inline static const std::string DISABLE_ALT_BUFFER = "\033[?1049l";
inline static const std::string CLEAR_SCREEN = "\033[2J";
inline static const std::string HOME_CURSOR = "\033[H";
inline static const std::string RESET_STYLE = "\033[0m";
inline static const std::string TERM_SYNC_BEGIN = "\033[?2026h";
inline static const std::string TERM_SYNC_END = "\033[?2026l";

static const int SCREEN_FPS = 30;

Screen::Screen(int w, int h) {
  _w = w;
  _h = h;
  resize(w, h);
  last_draw = 0;
}

void Screen::activate() {
  PLATFORM_ERROR err = platform_term_activate();
  if (err != PLATFORM_ERROR::NONE) {
    // report error
    return;
  }

  std::cout << ENABLE_ALT_BUFFER << HIDE_CURSOR << CLEAR_SCREEN << HOME_CURSOR
            << std::flush;

  activated = true;
}

void Screen::deactivate() {
  PLATFORM_ERROR err = platform_term_deactivate();
  if (err != PLATFORM_ERROR::NONE) {
    // report error
    return;
  }

  std::cout << RESET_STYLE << SHOW_CURSOR << DISABLE_ALT_BUFFER << std::flush;

  activated = false;
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
  if ((x < 0) || (x >= _w) || (y < 0) || (y >= _h)) {
    return;
  }
  back_framebuffer[static_cast<size_t>((y * _w) + x)] = c;
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

std::string cell_seq(Cell c, Cell &old) {
  // TODO: state tracking
  std::string seq;
  if (memcmp(&c, &old, sizeof(Cell)) != 0) {
    seq += "\033[38;2;";
    seq += std::to_string(c.foreground.red) + ';';
    seq += std::to_string(c.foreground.green) + ';';
    seq += std::to_string(c.foreground.blue) + 'm';
  }

  if (memcmp(&c, &old, sizeof(Cell)) != 0) {
    seq += "\033[48;2;";
    seq += std::to_string(c.background.red) + ';';
    seq += std::to_string(c.background.green) + ';';
    seq += std::to_string(c.background.blue) + 'm';
  }

  if (c.attrs != ATTR_NONE) {
    seq += "\033[";
    if (c.attrs & ATTR_BOLD) {
      seq += "1;";
    }
    if (c.attrs & ATTR_DIM) {
      seq += "2;";
    }
    if (c.attrs & ATTR_ITALIC) {
      seq += "3;";
    }
    if (c.attrs & ATTR_UNDERLINE) {
      seq += "4;";
    }
    if (c.attrs & ATTR_BLINK) {
      seq += "5;";
    }
    if (c.attrs & ATTR_REVERSE) {
      seq += "7;";
    }
    if (c.attrs & ATTR_HIDDEN) {
      seq += "8;";
    }
    if (c.attrs & ATTR_STRIKE) {
      seq += "9m;";
    }
    seq.pop_back();
    seq += 'm';
  }

  if (c.foreground.default_color && c.background.default_color) {
    seq += "\033[0m";
  }

  seq += static_cast<char32_t>(c.glyph);
  return seq;
}

void Screen::present() {
  // limit according to FPS here
  const uint64_t now = platform_monotime();
  if ((last_draw + static_cast<uint64_t>(1.0 / SCREEN_FPS) * 1000) > now) {
    std::transform(back_framebuffer.begin(), back_framebuffer.end(),
                   back_framebuffer.begin(),
                   [](Cell _) { return default_cell; });
    return;
  }

  std::string payload;
  payload += TERM_SYNC_BEGIN + HOME_CURSOR + RESET_STYLE;
  // push (back - front) diffs to term

  
  for (int y = 0; y < _h; y++) {
    Cell old_cell = default_cell;
    for (int x = 0; x < _w; x++) {
      Cell f = front_framebuffer[(y * _w) + x];
      Cell b = back_framebuffer[(y * _w) + x];

      // safe, all set to zero and no paddings on Cell.
      if (memcmp(&f, &b, sizeof(Cell)) != 0) {
        payload += move_cursor_seq(x, y);
        payload += cell_seq(b, b);
        old_cell = b;
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
  for (int xp = 0; xp < w; xp++) {
    put(x + xp, y, c);
  }
};
void Screen::draw_vline(int x, int y, int h, Cell c) {
  for (int yp = 0; yp < h; yp++) {
    put(x, y + yp, c);
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
    float t = (x - x1) / static_cast<float>(x2 - x1);
    int y = std::round(y1 + (y2 - y1) * t);
    if (steep) // if transposed, de−transpose
      put(x, y, c);
    else
      put(x, y, c);
  }
}
void Screen::draw_rect(int x, int y, int w, int h, Cell c) {
  for (int yp = 0; yp < h; yp++) {
    draw_hline(x, y + yp, w, c);
  }
};
void Screen::draw_text(int x, int y, const std::string text, Cell style) {
  // guessing this is unoptimal
  int text_length = static_cast<int>(text.length());
  for (int xp = 0; xp < text_length; xp++) {
    Cell c = style;
    c.glyph = text.at(static_cast<int>(xp));
    put(x + xp, y, c);
  }
};

int Screen::width() { return _w; }

int Screen::height() { return _h; }
