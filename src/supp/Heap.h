#pragma once

#include <cstddef>
#include <utility>

namespace supp {

// POD types only
template <typename Self>
class Heap {
 public:
    Heap() = default;

    template <typename T>
    void push(T&& arg) {
        self().pushBack(std::forward<T>(arg));
        siftUp(self().size() - 1);
    }

    auto pop() {
        return remove(0);
    }

    auto erase(size_t idx) {
        return remove(idx);
    }

    void fix(size_t idx) {
        if (!siftDown(idx)) {
            siftUp(idx);
        }
    }

    decltype(auto) front() {
        return self().at(0);
    }

    decltype(auto) front() const {
        return self().at(0);
    }

    void clear() {
        while (!self().empty()) {
            pop();
        }
    }

    bool empty() const {
        return self().size() == 0;
    }

 private:
    auto remove(size_t idx) {
        auto res = self().at(idx);
        auto&& s = self();
        size_t back = s.size() - 1;

        if (idx == back) {
            s.popBack();
        } else {
            s.move(back, idx);
            s.popBack();
            fix(idx);
        }

        return std::move(res);
    }

    void siftUp(size_t idx) {
        auto&& s = self();

        while (idx > 0) {
            auto p = (idx - 1) / 2;

            if (!s.compare(idx, p)) {  // idx >= p
                return;
            }

            s.swap(idx, p);
            idx = p;
        }
    }

    bool siftDown(size_t idx) {
        bool sift = false;
        Self& s = self();

        while (2 * idx + 1 < s.size()) {
            size_t v = 2 * idx + 1;
            size_t r = v + 1;

            if (r < s.size() && s.compare(r, v)) {
                v = r;
            }

            if (!s.compare(v, idx)) {  // lesser child >= parent
                return sift;
            }

            s.swap(idx, v);
            idx = v;
            sift = true;
        }

        return false;
    }

    Self& self() {
        return *static_cast<Self*>(this);
    }

    const Self& self() const {
        return *static_cast<const Self*>(this);
    }
};

}  // namespace supp
