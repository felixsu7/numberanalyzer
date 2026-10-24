#include "platform.hpp"

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
