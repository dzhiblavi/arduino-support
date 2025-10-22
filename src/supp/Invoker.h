#pragma once

#include <utility>

namespace supp {

template <typename Class, auto Method>
class Invoker {
 public:
    Invoker(Class* self) : self_{self} {}

    template <typename... Args>
    void operator()(Args&&... args) {  // NOLINT
        (self_->*Method)(std::forward<Args>(args)...);
    }

 private:
    Class* self_;
};

}  // namespace supp
