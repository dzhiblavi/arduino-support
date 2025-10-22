#pragma once

#include "Heap.h"

#include <array>
#include <utility>

#include <limits.h>
#include <stddef.h>

namespace supp {

class IntrusivePriorityQueueNode {
 public:
    IntrusivePriorityQueueNode() = default;

    bool connected() const {
        return heap_index_ != UINT_MAX;
    }

 private:
    size_t heap_index_{UINT_MAX};

    template <typename T, size_t Cap, typename Comp>
    friend class IntrusivePriorityQueue;
};

template <typename T, size_t Cap, typename Comp>
class IntrusivePriorityQueue : public Heap<size_t, T*, IntrusivePriorityQueue<T, Cap, Comp>> {
 public:
    IntrusivePriorityQueue() = default;

    size_t size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

    constexpr size_t capacity() const {
        return Cap;
    }

 private:
    constexpr size_t root() const {
        return 0;
    }

    constexpr bool isRoot(size_t i) const {
        return i == 0;
    }

    constexpr size_t parent(size_t i) const {
        return (i - 1) / 2;
    }

    constexpr size_t left(size_t i) const {
        return 2 * i + 1;
    }

    constexpr size_t right(size_t i) const {
        return 2 * i + 2;
    }

    size_t back() const {
        return size_ - 1;
    }

    T* at(size_t i) {
        return buf_[i];
    }

    const T* at(size_t i) const {
        return buf_[i];
    }

    void pushBack(T* val) {
        buf_[size_++] = val;
    }

    void popBack() {
        --size_;
    }

    bool hasLeft(size_t i) {
        return left(i) < size_;
    }

    bool hasRight(size_t i) {
        return right(i) < size_;
    }

    bool compare(size_t i, size_t j) {
        return compare_(*buf_[i], *buf_[j]);
    }

    void swap(size_t i, size_t j) {
        std::swap(buf_[i], buf_[j]);
        buf_[i]->heap_index_ = i;
        buf_[j]->heap_index_ = j;
    }

    void move(size_t from, size_t to) {
        buf_[to] = buf_[from];
        buf_[to]->heap_index_ = to;
    }

    size_t size_ = 0;
    std::array<T*, Cap> buf_;
    [[no_unique_address]] Comp compare_;

    friend class Heap<size_t, T*, IntrusivePriorityQueue<T, Cap, Comp>>;
};

}  // namespace supp
