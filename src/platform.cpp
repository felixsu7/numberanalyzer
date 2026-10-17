#include "platform.hpp"

// TODO
#ifdef PLAT_WINDOWS

PLATFORM_ERROR platform_term_activate(PLATFORM_STATE *state) {

};

PLATFORM_ERROR platform_term_deactivate(PLATFORM_STATE *state) {

};

PLATFORM_ERROR platform_poll_event(uint64_t timeout, PlatformEvent *out) {};

uint64_t platform_monotime() {

};

PLATFORM_ERROR platform_beep(uint64_t duration) {

};

#endif

#ifdef PLAT_UNIX

#include <cstdlib>
#include <ctime>
#include <termios.h>

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

PLATFORM_ERROR platform_poll_event(uint64_t timeout_millis,
                                   PlatformEvent *out) {

};

uint64_t platform_monotime() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return static_cast<uint64_t>(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
};

PLATFORM_ERROR platform_beep(uint64_t duration_millis) {

};

#endif
