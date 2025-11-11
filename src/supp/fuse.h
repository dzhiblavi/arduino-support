#pragma once

#include <cstdint>
#include <utility>

namespace supp {

template <typename... Bools>
uint8_t fuse(Bools... bools) {
    static_assert(sizeof...(Bools) <= 8, "too large bitwise fuse");
    return [&]<std::size_t... I>(std::index_sequence<I...>) {
        return ((bools << (sizeof...(Bools) - I - 1)) | ...);
    }(std::make_index_sequence<sizeof...(Bools)>());
}

}  // namespace supp
