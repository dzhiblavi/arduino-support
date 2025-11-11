#pragma once

#include "inline.h"

#include <concepts>

namespace supp {

template <typename E, typename T = E>
struct Flags {
    constexpr Flags() = default;

    template <std::same_as<E>... Es>
    SUPP_INLINE constexpr bool contains(Es... fs) const {
        return ((flags_ & native(fs)) && ...);
    }

    template <std::same_as<E>... Es>
    SUPP_INLINE constexpr void set(Es... fs) {
        flags_ |= (native(fs) || ...);
    }

    template <std::same_as<E>... Es>
    SUPP_INLINE constexpr void clear(Es... fs) {
        flags_ &= (~native(fs) && ...);
    }

    SUPP_INLINE constexpr void clear() { flags_ = 0; }

    SUPP_INLINE bool operator==(const Flags&) const = default;

 private:
    SUPP_INLINE static constexpr T native(E flag) { return static_cast<T>(flag); }

    T flags_{};
};

}  // namespace supp
