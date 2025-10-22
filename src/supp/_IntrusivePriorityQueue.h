#pragma once

#include "verify.h"

#include <utility>
#include <array>

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
class IntrusivePriorityQueue {  // NOLINT
 public:
    IntrusivePriorityQueue() = default;

    void push(T* val) {
        DASSERT(size_ < Cap);
        ++size_;

        val->heap_index_ = size_ - 1;
        buf_[size_ - 1] = val;

        siftUp(size_ - 1);
    }

    T* front() {
        DASSERT(!empty());
        return buf_[0];
    }

    const T* front() const {
        DASSERT(!empty());
        return buf_[0];
    }

    T* pop() {
        DASSERT(!empty());
        return remove(0);
    }

    bool erase(T* data) {
        if (!data->connected()) {
            return false;
        }

        DASSERT(buf_[data->heap_index_] == data);

        remove(data->heap_index_);
        return true;
    }

    void fixFront() {
        siftDown(0);
    }

    size_t size() const {
        return size_;
    }

    size_t capacity() const {
        return Cap;
    }

    bool empty() const {
        return size_ == 0;
    }

    // O(N)
    void clear() {
        while (!empty()) {
            remove(size_ - 1);
        }
    }

 private:
    T* remove(size_t i) {
        T* res = buf_[i];  // NOLINT
        res->heap_index_ = UINT_MAX;

        if (i == size_ - 1) {
            --size_;
        } else {
            move(size_ - 1, i);
            --size_;
            siftDown(i);
        }

        DASSERT(!res->connected());
        return res;
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

    void siftUp(size_t i) {
        while (i > 0 && compare(i, (i - 1) / 2)) {
            swap(i, (i - 1) / 2);
            i = (i - 1) / 2;
        }
    }

    void siftDown(size_t i) {
        while (2 * i + 1 < size_) {
            auto left = 2 * i + 1;
            auto right = 2 * i + 2;
            auto j = left;

            if (right < size_ && compare(right, left)) {
                j = right;
            }

            if (!compare(j, i)) {
                return;
            }

            swap(i, j);
            i = j;
        }
    }

    size_t size_ = 0;
    std::array<T*, Cap> buf_;
    [[no_unique_address]] Comp compare_;
};

}  // namespace supp
