#pragma once

#include "verify.h"

#include <utility>

#include <stddef.h>

namespace supp {

struct IntrusiveListNode {
    IntrusiveListNode* next_ = nullptr;
    IntrusiveListNode* prev_ = nullptr;

    bool isLinked() {
        return prev_ != nullptr;
    }

    void unlink() {
        DASSERT(isLinked());
        auto* prev = std::exchange(prev_, nullptr);
        auto* next = std::exchange(next_, nullptr);
        prev->next_ = next;
        next->prev_ = prev;
    }
};

template <typename T>
class IntrusiveList {
 public:
    IntrusiveList() = default;

    IntrusiveList(const IntrusiveList&) = delete;
    IntrusiveList& operator=(const IntrusiveList&) = delete;

    IntrusiveList(IntrusiveList&& rhs) noexcept {
        s_.next_ = std::exchange(rhs.s_.next_, &rhs.s_);
        s_.prev_ = std::exchange(rhs.s_.prev_, &rhs.s_);
        DASSERT(rhs.empty());
    }

    IntrusiveList& operator=(IntrusiveList&& rhs) noexcept {
        if (this == &rhs) {
            return *this;
        }

        std::swap(s_.next_, rhs.s_.next_);
        std::swap(s_.prev_, rhs.s_.prev_);
        return *this;
    }

    bool empty() const {
        return s_.next_ == &s_;
    }

    T* front() {
        DASSERT(!empty());
        return static_cast<T*>(s_.next_);
    }

    T* back() {
        DASSERT(!empty());
        return static_cast<T*>(s_.prev_);
    }

    T* popFront() {
        DASSERT(!empty());
        auto* front = s_.next_;
        front->unlink();
        return static_cast<T*>(front);
    }

    void insertAfter(T* after, T* node) {
        linkAfter(after, node);
    }

    void pushFront(T* val) {
        linkAfter(s_.next_, val);
    }

    void pushBack(T* val) {
        linkAfter(s_.prev_, val);
    }

    // O(N)
    void clear() {
        while (!empty()) {
            popFront();
        }
    }

    // O(N)
    size_t size() const {
        size_t res = 0;
        const IntrusiveListNode* cur = s_.next_;
        while (cur != &s_) {
            ++res;
            DASSERT(cur != cur->next_);
            cur = cur->next_;
        }
        return res;
    }

 private:
    // a -> v
    void linkAfter(IntrusiveListNode* a, IntrusiveListNode* v) {
        auto* next = a->next_;
        a->next_ = v;
        v->prev_ = a;
        v->next_ = next;
        next->prev_ = v;
    }

    // sentinel node
    IntrusiveListNode s_{&s_, &s_};
};

}  // namespace supp
