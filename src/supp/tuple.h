#pragma once

#include <utility>

namespace supp {

template <typename To, typename From>
To tuple_convert(From from) {
    return apply([](auto... vs) { return To{std::move(vs)...}; }, std::move(from));
}

template <auto Start, auto End, auto Inc, class F>
constexpr void constexprFor(F&& f) {
    if constexpr (Start < End) {
        f(std::integral_constant<decltype(Start), Start>());
        constexprFor<Start + Inc, End, Inc>(f);
    }
}

}  // namespace supp
