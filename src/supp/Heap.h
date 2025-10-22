#pragma once

namespace supp {

template <typename I, typename T, typename Self>
class Heap {
 public:
    Heap() = default;

    void push(T arg) {
        self().pushBack(arg);
        siftUp(self().back());
    }

    T pop() {
        return remove(self().root());
    }

    void erase(I idx) {
        return remove(idx);
    }

    void fix(I idx) {
        if (!siftDown(idx)) {
            siftUp(idx);
        }
    }

    auto front() {
        return self().at(self().root());
    }

    auto front() const {
        return self().at(self().root());
    }

    void clear() {
        while (!self().empty()) {
            pop();
        }
    }

 private:
    T remove(I idx) {
        T res = self().at(idx);
        auto&& s = self();
        I back = s.back();

        if (idx == back) {
            s.popBack();
        } else {
            s.move(back, idx);
            s.popBack();
            fix(idx);
        }

        return res;
    }

    void siftUp(I idx) {
        auto&& s = self();

        while (!s.isRoot(idx)) {
            auto p = s.parent(idx);

            if (!s.compare(idx, p)) {  // idx >= p
                return;
            }

            s.swap(idx, p);
            idx = p;
        }
    }

    bool siftDown(I idx) {
        bool sift = false;
        auto&& s = self();

        while (s.hasLeft(idx)) {
            auto v = s.left(idx);  // the lesser child

            if (s.hasRight(idx)) {
                if (auto r = s.right(idx); s.compare(r, v)) {  // right < left = v
                    v = r;
                }
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
