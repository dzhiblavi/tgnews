#ifndef NET_IO_API_H
#define NET_IO_API_H

#include <functional>
#include <array>
#include <iostream>
#include <cassert>
#include <csignal>

#include "ipv4_error.h"
#include "unique_fd.h"
#include "timer.h"
#include "poll.h"

namespace io_api {
class io_context {
    friend struct io_unit;

private:
    class poll p;

private:
    int call_and_timeout() noexcept;
    timer tm;

public:
    io_context() = default;
    io_context(io_context const&) = delete;
    io_context& operator=(io_context const&) = delete;

    io_context(io_context&&) noexcept;
    io_context& operator=(io_context&&) noexcept;

    void exec() noexcept;
    timer& get_timer() noexcept;
    friend void swap(io_context&, io_context&) noexcept;
};

struct io_unit {
public:
    typedef std::function<void(poll::flag const&)> callback_t;

private:
    io_context* ctx_ = nullptr;
    poll::flag events_;
    int fd_ = -1;
    callback_t callback_{};

public:
    io_unit(io_context* ctx, poll::flag const& events, int fd, callback_t callback);

    ~io_unit();

    io_unit(io_unit const&) = delete;

    io_unit& operator=(io_unit const&) = delete;

    io_unit(io_unit&&) noexcept;

    io_unit& operator=(io_unit&&) noexcept;

    [[nodiscard]] io_context* context() noexcept;

    [[nodiscard]] io_context const* context() const noexcept;

    void reconfigure_events(poll::flag const&);

    void configure_callback(callback_t) noexcept;

    void callback(poll::flag const&);

    friend void swap(io_unit&, io_unit&) noexcept;
};
} // namespace io_api

#endif //WEB_CRACKER_IO_API_H