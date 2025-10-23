#pragma once

#define _STRINGIZE2(x) #x
#define _STRINGIZE1(x) _STRINGIZE2(x)

#include <logging/log.h>  // LFATAL

#include <cstdlib>  // abort

#define _VERIFY(expr, ...)                                             \
    [&] {                                                              \
        auto&& retval__ = (expr);                                      \
        if (!static_cast<bool>(retval__)) [[unlikely]] {               \
            LFATAL("Verify failed expr='" #expr "': ", ##__VA_ARGS__); \
            abort();                                                   \
        }                                                              \
        return retval__;                                               \
    }()

#define VERIFY(...) _VERIFY(__VA_ARGS__)

#if defined(DEBUG) || defined(SUPP_FORCE_DASSERT)

#define DASSERT(expr, ...) (void)_VERIFY(expr, ##__VA_ARGS__)

#else

#define DASSERT(...)

#endif
