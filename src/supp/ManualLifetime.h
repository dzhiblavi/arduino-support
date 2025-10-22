#pragma once

#include <supp/verify.h>

#include <utility>

#include <stdint.h>

namespace supp {

template <typename T>
struct ManualLifetime {  // NOLINT
 public:
    ManualLifetime() = default;

    ~ManualLifetime() {
        if (initialized_) {
            release();
        }
    }

    ManualLifetime(const ManualLifetime&) = delete;
    ManualLifetime& operator=(const ManualLifetime&) = delete;

    ManualLifetime(ManualLifetime&& other) noexcept {  // NOLINT
        if (other.initialized_) {
            emplace(std::move(other.get()));
            other.release();
        }
    }

    [[nodiscard]] bool initialized() const noexcept {
        return initialized_;
    }

    void emplace(T value) {
        DASSERT(!initialized_);
        new (getPtr()) T(std::move(value));
        initialized_ = true;
    }

    template <typename... Args>
    void emplace(Args&&... args) {
        DASSERT(!initialized_);
        new (getPtr()) T(std::forward<Args>(args)...);
        initialized_ = true;
    }

    T get() && noexcept {
        DASSERT(initialized_);
        return std::move(*getPtr());
    }

    T& get() & noexcept {
        DASSERT(initialized_);
        return *getPtr();
    }

 private:
    T* getPtr() {
        return reinterpret_cast<T*>(&storage_);
    }

    void release() {
        DASSERT(initialized_);
        initialized_ = false;
        getPtr()->~T();
    }

    alignas(T) uint8_t storage_[sizeof(T)] /* uninitialized */;
    bool initialized_ = false;
};

}  // namespace supp
