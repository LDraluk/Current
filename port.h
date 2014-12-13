// Cross-platform portability header.
//
// Ensures that one and only one of BRICKS_{POSIX,APPLE,CURL,ANDROID} is defined.
// Keeps the one provided externally. Defaults to environmental setting is none has been defined.

#ifndef BRICKS_PORT_H
#define BRICKS_PORT_H

#include <string>

#ifdef COUNT
#error "`COUNT` should not be defined for port.h"
#endif

#define COUNT (defined(BRICKS_POSIX) + defined(BRICKS_APPLE) + defined(BRICKS_CURL) + defined(BRICKS_ANDROID))

#if COUNT > 1
#error "More than one `BRICKS_*` architectures have been defined."
#elif COUNT == 0

#if defined(__linux)
#define BRICKS_POSIX
#elif defined(__APPLE__)
#define BRICKS_APPLE
#else
#error "Could not detect architecture. Please define one of the `BRICKS_*` macros explicitly."
#endif

#endif

#undef COUNT

#if defined(BRICKS_POSIX)
#define BRICKS_ARCH_UNAME std::string("Linux")
#elif defined(BRICKS_APPLE)
#define BRICKS_ARCH_UNAME std::string("Darwin")
#else
#error "Unknown architecture."
#endif

#endif
