#ifndef NET_HANDLER_H 
#define NET_HANDLER_H

#include <functional>
#include <iostream>
#include <stdexcept>
#include <type_traits>

#include "ipv4_error.h"
#include "basic_include.h"

template <typename R, typename F, typename... Args>
struct h_is_invocable_r {
#if defined(__APPLE__)
    static const bool value = std::__invokable_r<R, F, Args...>::value;
#else
    static const bool value = std::is_invokable_r<R, F, Args...>::value;
#endif
};

namespace ipv4 {
template <typename... Args>
struct handler {
    using raw_args = typename std::remove_reference<Args...>::type;
    using on_success_fn_t = std::function<void(raw_args&&)>;
    using on_fail_fn_t = std::function<void(std::runtime_error)>;

    friend class socket;
    friend class server_socket;

private:
    on_success_fn_t on_suc;
    on_fail_fn_t on_fail;

private:
    void success(raw_args args) const {
        on_suc(std::move(args));
    }

    void fail(const std::runtime_error& re) const {
        if (on_fail) {
            on_fail(re);
        } else {
            throw re;
        }
    }

public:
    handler() = default;

    handler(on_success_fn_t&& fun)
        : on_suc(std::move(fun))
        , on_fail({}) {}

    handler(on_success_fn_t&& fun, on_fail_fn_t&& fail)
        : on_suc(std::move(fun))
        , on_fail(std::move(fail)) {}

    operator bool() const noexcept {
        return bool(on_suc);
    }
};

template <>
struct handler<> {
    using on_success_fn_t = std::function<void()>;
    using on_fail_fn_t = std::function<void(std::runtime_error)>;

    friend class socket;
    friend class server_socket;

private:
    on_success_fn_t on_suc;
    on_fail_fn_t on_fail;

private:
    void success() const {
        on_suc();
    }

    void fail(const std::runtime_error& re) const {
        if (on_fail) {
            on_fail(re);
        } else {
            std::cerr << "Suspended failure: " <<  re.what() << std::endl;
        }
    }

public:
    handler() = default;

    handler(on_success_fn_t&& fun)
        : on_suc(std::move(fun))
        , on_fail({}) {}

    handler(on_success_fn_t&& fun, on_fail_fn_t&& fail)
        : on_suc(std::move(fun))
        , on_fail(std::move(fail)) {}

    operator bool() const noexcept {
        return bool(on_suc);
    }
};
} // namespace ipv4

#endif
