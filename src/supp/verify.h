#pragma once

#define _STRINGIZE2(x) #x
#define _STRINGIZE1(x) _STRINGIZE2(x)

#include <logging/log.h>  // LFATAL

#include <cstdlib>  // abort

#if defined(DEBUG) || defined(SUPP_FORCE_DASSERT)

#define VERIFY(expr, ...)                                                         \
    [&] {                                                                         \
        auto&& retval__ = (expr);                                                 \
        if (!static_cast<bool>(retval__)) [[unlikely]] {                          \
            LFATAL(F("Verify failed expr='"), F(#expr), F("': "), ##__VA_ARGS__); \
            abort();                                                              \
        }                                                                         \
        return retval__;                                                          \
    }()

#define DASSERT(expr, ...) (void)VERIFY(expr, ##__VA_ARGS__)

#else

#define VERIFY(expr, ...) expr
#define DASSERT(...)

#endif
