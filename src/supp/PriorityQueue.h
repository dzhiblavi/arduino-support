#pragma once

#include "Heap.h"

#include <array>

#include <stddef.h>

namespace supp {

// POD types only
template <typename T, size_t Cap, typename Comp>
class PriorityQueue : public Heap<PriorityQueue<T, Cap, Comp>> {
 public:
    PriorityQueue() = default;

    void clear() {
        size_ = 0;
    }

    constexpr size_t capacity() const {
        return Cap;
    }

    size_t size() const {
        return size_;
    }

 private:
    const T& at(size_t i) const {
        return buf_[i];
    }

    T& at(size_t i) {
        return buf_[i];
    }

    bool compare(size_t i, size_t j) {
        return comp_(buf_[i], buf_[j]);
    }

    void swap(size_t i, size_t j) {
        std::swap(buf_[i], buf_[j]);
    }

    void replaceWithBack(size_t to) {
        --size_;
        buf_[to] = buf_[size_];
    }

    void pushBack(T val) {
        buf_[size_++] = val;
    }

    void popBack() {
        --size_;
    }

    size_t size_ = 0;
    std::array<T, Cap> buf_;
    [[no_unique_address]] Comp comp_{};

    friend class Heap<PriorityQueue<T, Cap, Comp>>;
};

}  // namespace supp
