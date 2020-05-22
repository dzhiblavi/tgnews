#include <fcntl.h>
#include <vector>
#include "socket.h"

#if defined(__linux) || defined(__APPLE__)
#include <sys/ioctl.h>
#endif

namespace {
#ifdef WIN32
#define ioctl ioctlsocket
#endif

void set_nonblock(ipv4::sock_fd_t handle) {
    unsigned long int on = 1;
    if (0 > ioctl(handle, FIONBIO, &on)) {
        IPV4_EXC();
    }
}

void sock_enable_resuseaddr(ipv4::sock_fd_t fd) {
#ifdef WIN32
    char enable = 1;
#elif defined(__linux) || defined(__APPLE__)
    int enable = 1;
#endif
    int err = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    if (err < 0) {
        IPV4_EXC();
    }
}

ipv4::sock_fd_t sock_create(int domain, int type, int proto) {
    ipv4::sock_fd_t s = socket(domain, type, proto);
    if (s < 0) {
        IPV4_EXC();
    }
    return s;
}

int sock_recv(ipv4::sock_fd_t sockfd, NET_BUFF_PTR buff, size_t maxlen) {
    return recv(sockfd, buff, maxlen, 0);
}

int sock_send(ipv4::sock_fd_t sockfd, NET_BUFF_CPTR buff, size_t len) {
    return send(sockfd, buff, len, 0);
}

int sock_connect(int fd, ipv4::endpoint const& ep) {
    sockaddr_in ad{};

    ad.sin_family = AF_INET;
    ad.sin_port = ep.port();
    ad.sin_addr.s_addr = ep.net_addr();

    return connect(fd, reinterpret_cast<sockaddr const*>(&ad), sizeof ad);
}

void sock_bind(int fd, ipv4::endpoint const& ep) {
    sockaddr_in ad {};

    ad.sin_family = AF_INET;
    ad.sin_port = ep.port();
    ad.sin_addr.s_addr = ep.net_addr();

    if (bind(fd, reinterpret_cast<sockaddr const*>(&ad), sizeof ad) < 0) {
        IPV4_EXC();
    }
}

void sock_listen(int fd, int maxq) {
    if (listen(fd, maxq) < 0) {
        IPV4_EXC();
    }
}

int sock_accept(int fd) {
    return accept(fd, nullptr, nullptr);
}

int sock_geterr(int fd) {
#ifdef WIN32
    char option_value = 8;
    char* option_value_p = &option_value;
#elif defined(__linux) || defined(__APPLE__)
    int option_value = 228;
    void* option_value_p = (void*)&option_value;
#endif
    int option_len = 8;
    int r = getsockopt(fd, SOL_SOCKET, SO_ERROR, option_value_p,
                       reinterpret_cast<socklen_t *>(&option_len));
    if (r)
        IPV4_EXC("getsockopt(...) failed");
    return option_value;
}
}

namespace ipv4 {
int basic_socket::recv(NET_BUFF_PTR buff, size_t max_len) noexcept {
    return sock_recv(fd_.native_handle(), buff, max_len);
}

int basic_socket::send(NET_BUFF_CPTR buff, size_t max_len) noexcept {
    return sock_send(fd_.native_handle(), buff, max_len);
}

basic_socket::basic_socket(ipv4::sock_ufd&& fd)
        : fd_(std::move(fd)) {}

basic_socket::basic_socket(endpoint const& ep)
        : fd_(sock_create(AF_INET, SOCK_STREAM, 0)) {
    sock_connect(fd_.native_handle(), ep);
}

void basic_socket::set_nonblock() {
    ::set_nonblock(fd_.native_handle());
}


poll::flag socket::events_() const noexcept {
    poll::flag fl{};
    fl.set_read(bool(on_read_));
    fl.set_write(bool(on_write_));
    fl.set_eof(bool(on_disconnect_));
    return fl;
}

std::function<void(poll::flag const&)> socket::configure_callback_() noexcept {
    return [this](poll::flag const& ev) {
        std::cerr << "CALLBACK" << std::endl;
        bool cur_destroyed = false;
        bool* old_destroyed = destroyed_;
        destroyed_ = &cur_destroyed;
        try {
            if (ev.eof()) {
                if (this->on_disconnect_)
                    this->on_disconnect_();
                if (cur_destroyed)
                    return;
            }

            if (ev.read() && on_read_) {
                this->on_read_();
                if (cur_destroyed)
                    return;
            }
            if (ev.write() && on_write_) {
                this->on_write_();
            }
        } catch (...) {
            std::cerr << "ERROR: " << std::strerror(errno) << std::endl;
            std::terminate();
        }
        destroyed_ = old_destroyed;
    };
}

socket::socket(io_api::io_context& ctx, int fd)
    : basic_socket(sock_ufd(fd))
    , destroyed_(nullptr) {
    set_nonblock();
}

socket::~socket() {
    if (destroyed_ != nullptr) {
        *destroyed_ = true;
    }
}

socket::socket(ipv4::socket&& b) noexcept
        : basic_socket(std::move(b.fd_))
        , on_disconnect_(std::move(b.on_disconnect_))
        , on_read_(std::move(b.on_read_))
        , on_write_(std::move(b.on_write_))
        , unit_(std::move(b.unit_))
        , destroyed_(nullptr) {
    std::swap(destroyed_, b.destroyed_);
    unit_.configure_callback(configure_callback_());
}

socket& socket::operator=(ipv4::socket&& rhs) noexcept {
    if (this == &rhs) {
        return *this;
    }

    swap(*this, rhs);
    return *this;
}

void socket::set_on_disconnect(callback_t const& on_disconnect) {
    on_disconnect_ = on_disconnect;
    unit_.reconfigure_events(events_());
}

void socket::set_on_read(callback_t const& on_read) {
    on_read_ = on_read;
    unit_.reconfigure_events(events_());
}

void socket::set_on_write(callback_t const& on_write) {
    on_write_ = on_write;
    unit_.reconfigure_events(events_());
}

void socket::connect(const endpoint &ep, const con_callback_t &on_connect, const socket::dc_callback_t &on_disconnect) {
    if (!sock_connect(fd_.native_handle(), ep)) {
        set_on_disconnect(on_disconnect);
        on_connect.success();
    } else {
        set_on_disconnect([&, this] {
            unit_.close();
            on_connect.fail(IPV4_ERROR("Disconnected"));
        });

        set_on_write([&, this] {
            if (int r = sock_geterr(fd_.native_handle())) {
                unit_.close();
                on_connect.fail(IPV4_ERROR(std::to_string(r)));
            }
            set_on_disconnect(on_disconnect);
            on_connect.success();
        });
    }}

void socket::read(char* buff, size_t size, rw_callback_t const& on_read) {
    if (on_read) {
        set_on_read([&, this] {
            int r = recv(buff, size);
            if (r < 0) {
                if (gerrno == EINTR) {
                    return;
                } else {
                    on_read.fail(IPV4_ERROR());
                }
            }
            on_read.success(r);
        });
    } else {
        set_on_read({});
    }
}

void socket::write(char* buff, size_t size, rw_callback_t on_write) {
    if (on_write) {
        set_on_write([&, this] {
            int r = send(buff, size);
            if (r < 0) {
                if (errno == EINTR) {
                    return;
                } else {
                    on_write.fail(IPV4_ERROR());
                }
            }
            on_write.success(r);
        });
    } else {
        set_on_write({});
    }
}

bool socket::has_on_disconnect() const noexcept {
    return bool(on_disconnect_);
}

bool socket::has_on_read() const noexcept {
    return bool(on_read_);
}

bool socket::has_on_write() const noexcept {
    return bool(on_write_);
}

void swap(socket& a, socket& b) noexcept {
    swap(a.fd_, b.fd_);
    std::swap(a.on_disconnect_, b.on_disconnect_);
    std::swap(a.on_read_, b.on_read_);
    std::swap(a.on_write_, b.on_write_);
    swap(a.unit_, b.unit_);
    std::swap(a.destroyed_, b.destroyed_);
    a.unit_.configure_callback(a.configure_callback_());
    b.unit_.configure_callback(b.configure_callback_());
}

server_socket::server_socket(io_api::io_context &ctx)
        : ctx(ctx) {}

void server_socket::bind(const ipv4::endpoint &ep) {
    fd_ = sock_ufd(sock_create(AF_INET, SOCK_STREAM, 0));
    set_nonblock(fd_.native_handle());
    sock_enable_resuseaddr(fd_.native_handle());
    sock_bind(fd_.native_handle(), ep);
    sock_listen(fd_.native_handle(), SSOCK_MAX_LISTEN);
}

void server_socket::accept(const con_callback_t &on_connect) {
    std::cerr << "socket : " << fd_.native_handle() << std::endl;
    unit_ = io_api::io_unit(&ctx, poll::flag(true, false, false), fd_.native_handle(), [&, this] (poll::flag const& ev) {
        if (ev.read()) {
            try {
                int fd = sock_accept(fd_.native_handle());
                socket s(ctx, fd);
                on_connect.success(std::move(s));
            } catch (std::runtime_error& re) {
                on_connect.fail(re);
            }
        }
    });
}
} // namespace ipv4