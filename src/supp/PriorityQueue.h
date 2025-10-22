#pragma once

#include "verify.h"

#include <array>
#include <utility>

#include <stddef.h>

namespace supp {

// POD types only
template <typename T, size_t Cap, typename Comp>
class PriorityQueue {
 public:
    PriorityQueue() = default;

    void push(T val) {
        DASSERT(size_ < Cap);
        ++size_;
        buf_[size_ - 1] = val;
        siftUp(size_ - 1);
    }

    const T& front() const {
        DASSERT(!empty());
        return buf_[0];
    }

    T pop() {
        DASSERT(!empty());
        T out = buf_[0];
        buf_[0] = buf_[size_ - 1];
        --size_;
        siftDown(0);
        return out;
    }

    void fixFront() {
        siftDown(0);
    }

    size_t size() const {
        return size_;
    }

    bool empty() const {
        return size() == 0;
    }

    void clear() {
        size_ = 0;
    }

 private:
    void siftUp(size_t i) {
        while (i > 0 && comp_(buf_[i], buf_[(i - 1) / 2])) {
            std::swap(buf_[i], buf_[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }

    void siftDown(size_t i) {
        while (2 * i + 1 < size_) {
            auto left = 2 * i + 1;
            auto right = 2 * i + 2;
            auto j = left;
            if (right < size_ && comp_(buf_[right], buf_[left])) {
                j = right;
            }

            if (!comp_(buf_[j], buf_[i])) {
                return;
            }

            std::swap(buf_[i], buf_[j]);
            i = j;
        }
    }

    size_t size_ = 0;
    std::array<T, Cap> buf_;
    [[no_unique_address]] Comp comp_{};
};

}  // namespace supp
