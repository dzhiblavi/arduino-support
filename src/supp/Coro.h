#pragma once

#include <type_traits>
#include <utility>

// https://github.com/chriskohlhoff/asio/blob/master/asio/include/asio/coroutine.hpp

namespace supp {

namespace detail {
class CoRef;
}  // namespace detail

class Coro {
 public:
    Coro() = default;

    bool isChild() const {
        return value_ < 0;
    }

    bool isParent() const {
        return !isChild();
    }

    bool isInitial() const {
        return value_ == 0;
    }

    bool isComplete() const {
        return value_ == -1;
    }

    void reset() {
        value_ = 0;
    }

    bool resetIfNotRunning() {
        if (isInitial()) {
            return true;
        }

        if (isComplete()) {
            reset();
            return true;
        }

        return false;
    }

 private:
    friend class detail::CoRef;
    int value_ = 0;
};

template <typename... Children>
void resetAll(Children&... children) {
    (children.reset(), ...);
}

template <typename F>
auto makeCoro(F&& f) {  // NOLINT
    using C = std::remove_reference_t<F>;

    struct Coroutine : Coro {
        Coroutine(F&& f) : func_{std::forward<F>(f)} {}  // NOLINT

        void operator()() {
            func_(this);
        }

     private:
        C func_;
    };

    return Coroutine(std::forward<F>(f));
}

template <typename F>
using CoroOf = decltype(makeCoro(std::declval<F&&>()));

namespace detail {

class CoRef {
 public:
    CoRef(Coro& c) : value_(c.value_) {}
    CoRef(Coro* c) : value_(c->value_) {}
    CoRef(const CoRef&) = default;

    ~CoRef() {
        if (!modified_) {
            value_ = -1;
        }
    }

    operator int() const {
        return value_;
    }

    int& operator=(int v) {  // NOLINT
        modified_ = true;
        return value_ = v;
    }

 private:
    CoRef& operator=(const CoRef&) = delete;

    int& value_;
    bool modified_ = false;
};

}  // namespace detail

}  // namespace supp

#define CORO_BODY_IMPL(c, onTerminate)             \
    switch (::supp::detail::CoRef _coro_value = c) \
    default:                                       \
        if (_coro_value) {                         \
            goto terminate_coroutine;              \
        terminate_coroutine:                       \
            _coro_value = -1;                      \
            goto bail_out_of_coroutine;            \
        bail_out_of_coroutine:                     \
            onTerminate;                           \
        } else /* fall-through */                  \
        case 0:

#define CORO_YIELD_IMPL(n)                                  \
    for (_coro_value = (n);;)                               \
        if (_coro_value == 0) {                             \
            case (n):;                                      \
                break;                                      \
        } else                                              \
            switch (_coro_value ? 0 : 1)                    \
                for (;;)                                    \
                /* fall-through */ case -1:                 \
                    if (_coro_value)                        \
                        goto terminate_coroutine;           \
                    else                                    \
                        for (;;)                            \
                        /* fall-through */ case 1:          \
                            if (_coro_value)                \
                                goto bail_out_of_coroutine; \
                            else /* fall-through */         \
                            case 0:
