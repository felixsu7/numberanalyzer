#pragma once

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
