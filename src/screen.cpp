#include "screen.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>
#include <utility>

static const Cell default_cell =
    Cell{' ', {0, 0, 0, true}, {0, 0, 0, true}, ATTR_NONE};

Screen::Screen(int w, int h) {
  _w = w;
  _h = h;
  resize(w, h);
}

void Screen::activate() {
  PLATFORM_ERROR err = platform_term_activate(&platform_state);
  if (err != PLATFORM_ERROR::NONE) {
    // report error
    return;
  }
}

void Screen::deactivate() {
  PLATFORM_ERROR err = platform_term_deactivate(&platform_state);
  if (err != PLATFORM_ERROR::NONE) {
    // report error
    return;
  }
}

void Screen::resize(int w, int h) {
  front_framebuffer.resize(w * h);
  // do we need to reserve?
  front_framebuffer.reserve(w * h);
  back_framebuffer.resize(w * h);
  back_framebuffer.reserve(w * h);
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
  back_framebuffer[(y * _w) + x] = c;
}

void term_sync_begin() {}

void term_sync_end() {}

void Screen::present() {
  // limit according to FPS here

  std::string payload;
  term_sync_begin();
  //
  // push (back - front) diffs to term
  for (int y = 0; y < _h; y++) {
    for (int x = 0; x < _w; x++) {
      // each cell diff
    }
  }

  std::cout << payload;
  std::flush(std::cout);
  term_sync_end();

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
  int text_length = text.length();
  for (int xp = 0; xp < text_length; xp++) {
    Cell c = style;
    c.glyph = text.at(xp);
    put(xp, y, c);
  }
};

int Screen::width() { return _w; }

int Screen::height() { return _h; }
