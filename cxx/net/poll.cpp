#include "poll.h"

#include "ipv4_error.h"

namespace {
#if defined(NET_POLL_KQUEUE) || defined(NET_POLL_EPOLL)
int queue_create() {
    int fd;
#ifdef NET_POLL_KQUEUE
    fd = kqueue();
#elif defined(NET_POLL_EPOLL)
    fd = epoll_create(NET_POLL_SIZE);
#endif
    return fd;
}
#endif

#ifdef NET_POLL_EPOLL
void api_epoll_ctl(int efd, int op, int fd, epoll_event* event) {
    int r = epoll_ctl(efd, op, fd, event);
    if (r < 0)
        IPV4_EXC();
}

uint32_t setif(uint32_t events, uint32_t f, bool set) {
    return set ? events | f : events & ~f;
}
#endif

#ifdef NET_POLL_KQUEUE
void register_event(int pfd, poll::event* ev, int16_t filter) {
    ev->filter = filter;
    if (-1 == kevent(pfd, ev, 1, nullptr, 0, nullptr)) {
        // pass
    }
}
#endif

#ifdef NET_POLL_POLL
int sysapi_poll(poll::event* ev, int size, int wait) {
    int r = ::poll(ev, size, wait);
    if (r == -1 && gerrno != EINTR)
        IPV4_EXC();
    return r ? size : 0;
}

short setif(short events, short f, bool set) {
    return set ? events | f : events & ~f;
}
#endif
}

void* poll::get_data(event& e) {
#if defined(NET_POLL_KQUEUE)
    return e.udata;
#elif defined(NET_POLL_EPOLL)
    return e.data.ptr;
#elif defined(NET_POLL_POLL)
    assert(data_.find(e.fd) != data_.end());
    return data_[e.fd];
#endif
}

#if defined(NET_POLL_EPOLL) || defined(NET_POLL_KQUEUE)
poll::poll()
    : qfd_(queue_create()) {}
#elif defined(NET_POLL_POLL)
poll::poll() {}
#endif

#if defined(NET_POLL_EPOLL) || defined(NET_POLL_KQUEUE)
poll::native_handle_t poll::native_handle() const noexcept {
    return qfd_.native_handle();
}
#endif

poll::ev_it::ev_it(pointer base, pointer end)
    : base(base), end(end) {}

bool operator==(poll::ev_it const& a, poll::ev_it const& b) {
    return a.base == b.base;
}

bool operator!=(poll::ev_it const& a, poll::ev_it const& b) {
    return !(a == b);
}

poll::ev_it::reference poll::ev_it::operator*() const {
    return *base;
}

poll::ev_it::pointer poll::ev_it::operator->() const {
    return base;
}


#if defined(NET_POLL_EPOLL) || defined(NET_POLL_KQUEUE)
poll::ev_it& poll::ev_it::operator++() {
    ++base;
    return *this;
}
#elif defined(NET_POLL_POLL)
poll::ev_it& poll::ev_it::operator++() {
    ++base;
    while (base != end && !base->revents)
        ++base;
    return *this;
}
#endif

const poll::ev_it poll::ev_it::operator++(int) {
    ev_it nit(*this);
    ++*this;
    return nit;
}

poll::event_info::event_info(int fd, void *data)
    : fd_(fd), data_(data) {}

poll::event_info::event_info(int fd, void *data, const flag &f)
    : fd_(fd), data_(data), f_(f) {}

poll::flag& poll::event_info::get_flag() {
    return f_;
}

poll::flag const& poll::event_info::get_flag() const {
    return f_;
}

poll::sock_fd_t poll::event_info::fd() const {
    return fd_;
}

#if defined(NET_POLL_EPOLL)
poll::flag::flag(const poll::flag &o)
    : events_(o.events_) {}

void poll::event_info::event_ctl_apply(CTL ctl, poll& p) {
    event ev{get_flag().events(), data_};
    api_epoll_ctl(p.native_handle(), ctl, fd_, &ev);
}

poll::flag::flag(event const& ev)
    : events_(ev.events) {}

poll::flag::flag(bool r, bool w, bool e) {
    set_read(r);
    set_write(w);
    set_eof(e);
}

void poll::flag::set_read(bool set) {
    events_ = setif(events_, EPOLLIN, set);
}

void poll::flag::set_write(bool set) {
    events_ = setif(events_, EPOLLOUT, set);
}

void poll::flag::set_eof(bool set) {
    events_ = setif(events_, EPOLLRDHUP, set);
}

bool poll::flag::read() const {
    return events_ & EPOLLIN;
}

bool poll::flag::write() const {
    return events_ & EPOLLOUT;
}

bool poll::flag::eof() const {
    return events_ & EPOLLRDHUP;
}

uint32_t poll::flag::events() const {
    return events_;
}

#elif defined(NET_POLL_KQUEUE)
void poll::event_info::register_filters(int pfd, poll::event* ev) {
    ev->flags = f_.read() ? EV_ADD : EV_DELETE;
    register_event(pfd, ev, EVFILT_READ);

    ev->flags = f_.write() ? EV_ADD : EV_DELETE;
    register_event(pfd, ev, EVFILT_WRITE);
}

void poll::event_info::event_ctl_apply(CTL ctl, poll& p) {
    event e;
    switch (ctl) {
        case ADD:
            EV_SET(&e, fd_, 0, EV_ADD, 0, 0, data_);
            register_filters(p.native_handle(), &e);
            break;
        case REMOVE:
            EV_SET(&e, fd_, 0, EV_DELETE, 0, 0, data_);
            register_filters(p.native_handle(), &e);
            break;
        case MODIFY:
            event_ctl_apply(ADD, p);
            break;
    }
}

poll::flag::flag(const flag &o)
    : read_(o.read_)
    , write_(o.write_)
    , eof_(o.eof_) {}

poll::flag::flag(event const& ev) {
    if (ev.flags & EV_EOF) {
        eof_ = true;
        return;
    }

    switch (ev.filter) {
        case EVFILT_READ:
            read_ = true;
            break;
        case EVFILT_WRITE:
            write_ = true;
            break;
    }
}

poll::flag::flag(bool r, bool w, bool e) {
    set_read(r);
    set_write(w);
    set_eof(e);
}

void poll::flag::set_read(bool set) {
    read_ = set;
}

void poll::flag::set_write(bool set) {
    write_ = set;
}

void poll::flag::set_eof(bool set) {
    eof_ = set;
}

bool poll::flag::read() const {
    return read_;
}

bool poll::flag::write() const {
    return write_;
}

bool poll::flag::eof() const {
    return eof_;
}
#elif defined(NET_POLL_POLL)
void poll::event_info::event_ctl_apply(CTL ctl, poll &p) {
    if (ctl == ADD) {
        event ev{fd_, f_.events(), 0};
        p.events_.push_back(ev);
        p.data_[fd_] = data_;
        return;
    }

    auto it = p.events_.begin();
    for (; it != p.events_.end(); ++it)
        if (it->fd == fd_)
            break;

    assert(it != p.events_.end());
    if (ctl == REMOVE) {
        p.data_.erase(fd_);
        p.events_.erase(it);
    } else {
        it->events = f_.events();
        it->revents &= it->events;
        p.data_[fd_] = data_;
    }
}

poll::flag::flag(event const& ev)
    : events_(ev.revents) {}

poll::flag::flag(bool read, bool write, bool eof) {
    set_read(read);
    set_write(write);
    set_eof(eof);
}

void poll::flag::set_read(bool set) {
    events_ = setif(events_, POLLIN, set);
}

void poll::flag::set_write(bool set) {
    events_ = setif(events_, POLLOUT, set);
}

void poll::flag::set_eof(bool set) {
//    events_ = setif(events_, POLLHUP, set);
}

bool poll::flag::read() const {
    return events_ & POLLIN;
}

bool poll::flag::write() const {
    return events_ & POLLOUT;
}

bool poll::flag::eof() const {
    return events_ & POLLHUP;
}

short poll::flag::events() const {
    return events_;
}

poll::flag::flag(const poll::flag &o)
    : events_(o.events_) {}

#endif

#if defined(NET_POLL_POLL)
void poll::event_ctl(CTL ctl, event_info& evi) {
    q_.push({ctl, evi});
}
#elif defined(NET_POLL_KQUEUE) || defined(NET_POLL_EPOLL)
void poll::event_ctl(CTL ctl, event_info &evi) {
    evi.event_ctl_apply(ctl, *this);
}
#endif

int poll::wait(int wait) {
#if defined(NET_POLL_KQUEUE)
    timespec ts{ 0, std::max(0, wait / 1000000) };
    return kevent(native_handle(), nullptr, 0, events_.data(), events_.size(), wait != -1 ? &ts : nullptr);
#elif defined(NET_POLL_EPOLL)
    return epoll_wait(native_handle(), events_.data(), events_.size(), wait);
#elif defined(NET_POLL_POLL)
    while (!q_.empty()) {
        auto p = std::move(q_.front());
        q_.pop();
        p.second.event_ctl_apply(p.first, *this);
    }
    std::for_each(events_.begin(), events_.end(), [](event& ev) { ev.revents = 0; });
    return ::sysapi_poll(events_.data(), events_.size(), wait == -1 ? 1000 : std::min(1000, wait));
#endif
}

poll::ev_it poll::begin(int size) {
    auto it = events_.data();
#ifdef NET_POLL_POLL
    while (it != events_.data() + size && !it->revents)
        ++it;
#endif
    return ev_it(it, events_.data() + size);
}

poll::ev_it poll::end(int size) {
    return ev_it(events_.data() + size, events_.data() + size);
}

