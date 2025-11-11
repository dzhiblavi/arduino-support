#pragma once

#include "Heap.h"
#include "verify.h"

#include <array>
#include <utility>

#include <limits.h>
#include <stddef.h>

namespace supp {

class RandomAccessPriorityQueueNode {
 public:
    RandomAccessPriorityQueueNode() = default;

    bool connected() const { return heap_index_ != UINT_MAX; }

 private:
    size_t heap_index_{UINT_MAX};

    template <typename T, size_t Cap, typename Comp>
    friend class RandomAccessPriorityQueue;
};

template <typename T, size_t Cap, typename Comp>
class RandomAccessPriorityQueue : public Heap<RandomAccessPriorityQueue<T, Cap, Comp>> {
    using Base = Heap<RandomAccessPriorityQueue<T, Cap, Comp>>;

 public:
    RandomAccessPriorityQueue() = default;

    bool erase(T* val) {
        if (!val->connected()) {
            return false;
        }

        Base::erase(val->heap_index_);
        return true;
    }

    void fix(T* val) {
        DASSERT(val->connected());
        Base::fix(val->heap_index_);
    }

    size_t size() const { return size_; }

    constexpr size_t capacity() const { return Cap; }

 private:
    T* at(size_t i) {
        DASSERT(i < size_);
        return buf_[i];
    }

    const T* at(size_t i) const {
        DASSERT(i < size_);
        return buf_[i];
    }

    bool pushBack(T* val) {
        if (size_ == capacity()) {
            return false;
        }

        val->heap_index_ = size_;
        buf_[size_++] = val;
        return true;
    }

    void popBack() {
        DASSERT(size_ > 0);
        buf_[--size_]->heap_index_ = UINT_MAX;
    }

    bool compare(size_t i, size_t j) {
        DASSERT(i < size_);
        DASSERT(j < size_);
        return compare_(*buf_[i], *buf_[j]);
    }

    void swap(size_t i, size_t j) {
        DASSERT(i < size_);
        DASSERT(j < size_);
        std::swap(buf_[i], buf_[j]);
        buf_[i]->heap_index_ = i;
        buf_[j]->heap_index_ = j;
    }

    void replaceWithBack(size_t to) {
        DASSERT(to < size_);

        --size_;
        DASSERT(to != size_);

        buf_[to]->heap_index_ = UINT_MAX;
        buf_[to] = buf_[size_];
        buf_[to]->heap_index_ = to;
    }

    size_t size_ = 0;
    std::array<T*, Cap> buf_;
    [[no_unique_address]] Comp compare_;

    friend class Heap<RandomAccessPriorityQueue<T, Cap, Comp>>;
};

}  // namespace supp
