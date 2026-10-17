#ifndef PLATFORM_H
#define PLATFORM_H

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
#include <windows.h>
#elif defined(__unix__)
#define PLAT_UNIX
#include <unistd.h>
#else
#error "Unknown platform"
#endif

enum class PLATFORM_ERROR { NONE };

struct PLATFORM_STATE {};

// this must use alternative screen buffers, like save previous terminal state
// or something its all or nothing, so check first then return immediately if
// error
PLATFORM_ERROR platform_term_activate(PLATFORM_STATE *state);
PLATFORM_ERROR platform_term_deactivate(PLATFORM_STATE *state);

#endif // !PLATFORM_H
