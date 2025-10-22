#pragma once

#include <array>
#include <utility>

namespace supp {

template <typename T, size_t Cap>
class CircularBuffer {
 public:
    CircularBuffer() = default;

    void push(T value) {
        DASSERT(!full());
        buf_[end()] = std::move(value);
        ++size_;
    }

    T pop() {
        DASSERT(!empty());
        T res{std::move(buf_[head_])};
        head_ = (head_ + 1) % Cap;
        --size_;
        return res;
    }

    const T& front() {
        return buf_[head_];
    }

    size_t size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

    bool full() const {
        return size_ == Cap;
    }

    void clear() {
        head_ = size_ = 0;
    }

 private:
    size_t end() const {
        return (head_ + size_) % Cap;
    }

    size_t head_ = 0;
    size_t size_ = 0;
    std::array<T, Cap> buf_;
};

}  // namespace supp
