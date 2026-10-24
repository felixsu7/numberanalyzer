#include "screen.hpp"
#include "platform.hpp"
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

void cell_set_glyph(Cell *c, const char *s, size_t width) {
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
  if (c.cp[0] == 0) {
    c.cp[0] = ' ';
    c.cp[1] = 0;
  }

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
  // limit according to FPS here
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
  for (size_t y = 0; y < _h; y++) {
    int last_change_pos = 0;
    for (size_t x = 0; x < _w; x++) {
      Cell f = front_framebuffer[(y * _w) + x];
      Cell b = back_framebuffer[(y * _w) + x];

      // safe, all set to zero and no paddings on Cell.
      if (memcmp(&f, &b, sizeof(Cell)) != 0) {
        if (last_change_pos != x - 1)
          payload += move_cursor_seq(x, y);
        payload += cell_seq(b, old_cell);
        // x += b.width - 1;
        // for (size_t i = 0; i < b.width - 1; i++) {
        //   cell_set_glyph(&back_framebuffer[(y * _w) + x + i], " ", 1);
        // }
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

// doesnt support unicode
void Screen::draw_text(int x, int y, const std::string text, Cell style) {
  // guessing this is unoptimal
  int xp = 0;
  Cell c = style;
  c.width = 1;
  memset(&c.cp, 0, sizeof(c.cp));
  for (auto ch : text) {
    if (static_cast<unsigned char>(ch) & 0x80)
      continue;
    // TODO: unicode
    c.cp[0] = ch;
    put(x + xp, y, c);
    xp++;
  }
};

PlatformEvent Screen::event_poll(uint64_t timeout_millis) {
  PlatformEvent ev;
  platform_poll_event(timeout_millis, &ev);
  return ev;
}

int Screen::width() { return _w; }

int Screen::height() { return _h; }
