#include "io_api.h"

namespace {
volatile bool* quit = nullptr;

void signal_handler(int) {
    if (!quit) return;
    *quit = true;
}
}

namespace io_api {
io_context::io_context(io_context&& rhs) noexcept {
    swap(*this, rhs);
}

io_context& io_context::operator=(io_context&& rhs) noexcept {
    swap(*this, rhs);
    return *this;
}

void io_context::exec() noexcept {
    bool quitf = false;
    quit = &quitf;
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    for (;;) {
        if (quitf) return;
        int nfd = p.wait(call_and_timeout());
        if (quitf) return;

        if (nfd < 0) {
            if (gerrno != EINTR)
                std::cerr << "poll_wait() failed, errno = " << errno << std::endl;
            return;
        }

        for (auto it = p.begin(nfd); it != p.end(nfd); ++it) {
            if (quitf) return;

            try {
                static_cast<io_unit *>(p.get_data(*it))->callback(poll::flag(*it));
            } catch (...) {
                // ignore
            }
        }
    }
}

timer& io_context::get_timer() noexcept {
    return tm;
}

int io_context::call_and_timeout() noexcept {
    if (tm.empty()) {
        return -1;
    }
    timer::clock_t::time_point now = timer::clock_t::now();
    static_assert(noexcept(tm.callback(now)));
    tm.callback(now);
    if (tm.empty()) {
        return -1;
    }
    return std::chrono::duration_cast<std::chrono::milliseconds>(tm.top() - now).count();
}

void swap(io_context& a, io_context& b) noexcept {
    std::swap(a.p, b.p);
}

io_context* io_unit::context() noexcept {
    return ctx_;
}

io_context const* io_unit::context() const noexcept {
    return ctx_;
}

void io_unit::reconfigure_events(poll::flag const& fl) {
    if (!ctx_) return;
    poll::event_info info(fd_, this, fl);
    ctx_->p.event_ctl(poll::MODIFY, info);
}

io_unit::io_unit(io_api::io_context *ctx, poll::flag const& fl, int fd, io_api::io_unit::callback_t callback)
        : ctx_(ctx) , fd_(fd), events_(fl), callback_(std::move(callback)) {
    poll::event_info info(fd_, this, fl);
    ctx_->p.event_ctl(poll::ADD, info);
}

io_unit::io_unit(io_api::io_unit&& rhs) noexcept {
    swap(*this, rhs);
}

io_unit& io_unit::operator=(io_api::io_unit&& rhs) noexcept {
    swap(*this, rhs);
    return *this;
}

io_unit::~io_unit() {
    close();
}

void io_unit::close() {
    if (!ctx_) return;
    poll::event_info info(fd_, this);
    ctx_->p.event_ctl(poll::REMOVE, info);
    ctx_ = nullptr;
}

void io_unit::callback(poll::flag const& events) {
    assert(callback_);
    callback_(events);
}

void io_unit::configure_callback(io_api::io_unit::callback_t fn) noexcept {
    callback_.swap(fn);
}

void swap(io_unit& a, io_unit& b) noexcept {
    assert(b.ctx_ != nullptr);
    std::swap(a.ctx_, b.ctx_);
    std::swap(a.events_, b.events_);
    std::swap(a.fd_, b.fd_);
    std::swap(a.callback_, b.callback_);
    a.reconfigure_events(a.events_);
    b.reconfigure_events(b.events_);
}
} // namespace io_api