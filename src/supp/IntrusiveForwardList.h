#pragma once

#include "verify.h"

#include <cstddef>
#include <utility>

namespace supp {

template <typename Tag = void>
struct IntrusiveForwardListNode {
    IntrusiveForwardListNode* next_ = nullptr;

    bool linked() const {
        return next_ != nullptr;
    }
};

template <typename T, typename Tag = void>
class IntrusiveForwardList {
 public:
    IntrusiveForwardList() = default;

    IntrusiveForwardList(const IntrusiveForwardList&) = delete;
    IntrusiveForwardList& operator=(const IntrusiveForwardList&) = delete;

    IntrusiveForwardList(IntrusiveForwardList&& rhs) noexcept
        : head_{std::exchange(rhs.head_, nullptr)}
        , tail_{std::exchange(rhs.tail_, nullptr)} {
        DASSERT(rhs.empty());
    }

    IntrusiveForwardList& operator=(IntrusiveForwardList&& rhs) noexcept {
        if (this == &rhs) {
            return *this;
        }

        std::swap(head_, rhs.head_);
        std::swap(tail_, rhs.tail_);
        return *this;
    }

    bool empty() const {
        return head_ == nullptr;
    }

    T* front() {
        DASSERT(!empty());
        return static_cast<T*>(head_);
    }

    T* back() {
        DASSERT(!empty());
        return static_cast<T*>(tail_);
    }

    T* popFront() {
        DASSERT(!empty());

        auto* front = head_;

        head_ = head_->next_;
        front->next_ = nullptr;

        if (head_ == nullptr) {
            tail_ = nullptr;
        }

        return static_cast<T*>(front);
    }

    void pushFront(T* val) {
        DASSERT(val != tail_ && val != head_);
        static_cast<IntrusiveForwardListNode<Tag>*>(val)->next_ = head_;

        if (head_ == nullptr) {
            DASSERT(tail_ == nullptr);
            head_ = val;
            tail_ = val;
        } else {
            DASSERT(head_ && tail_);
            head_ = val;
        }
    }

    void pushBack(T* val) {
        DASSERT(val != tail_ && val != head_);
        static_cast<IntrusiveForwardListNode<Tag>*>(val)->next_ = nullptr;

        if (head_ == nullptr) {
            DASSERT(tail_ == nullptr);
            head_ = val;
            tail_ = val;
        } else {
            DASSERT(head_ && tail_);
            tail_->next_ = val;
            tail_ = val;
        }
    }

    void prepend(IntrusiveForwardList&& list) {  // NOLINT
        if (list.empty()) {
            return;
        }

        if (empty()) {
            *this = std::move(list);
            return;
        }

        list.tail_->next_ = head_;
        head_ = std::exchange(list.head_, nullptr);
        list.tail_ = nullptr;

        DASSERT(list.tail_ == nullptr);
        DASSERT(list.head_ == nullptr);
    }

    template <typename F>
    void iterate(F func) {
        auto* curr = head_;

        while (curr != nullptr) {
            func(static_cast<T&>(*curr));
            curr = curr->next_;
        }
    }

    template <typename F>
    void iterate(F func) const {
        auto* curr = head_;

        while (curr != nullptr) {
            func(static_cast<const T&>(*curr));
            curr = curr->next_;
        }
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
        const IntrusiveForwardListNode<Tag>* cur = head_;
        while (cur != nullptr) {
            ++res;
            DASSERT(cur != cur->next_);
            cur = cur->next_;
        }
        return res;
    }

 private:
    IntrusiveForwardListNode<Tag>* head_ = nullptr;
    IntrusiveForwardListNode<Tag>* tail_ = nullptr;
};

}  // namespace supp
