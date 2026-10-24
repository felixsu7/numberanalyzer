#pragma once
#include "platform.hpp"
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
  void cell_set_glyph(Cell *c, const char *s, size_t width);
};

inline const Cell default_cell =
    Cell{{' '}, 1, {0, 0, 0, false}, {0, 0, 0, false}, ATTR_NONE};

class Screen {
public:
  Screen(int w, int h);
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
