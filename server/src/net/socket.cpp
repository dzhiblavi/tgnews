#include <fcntl.h>
#include <vector>
#include "socket.h"

#if defined(__linux) || defined(__APPLE__)
#include <sys/filio.h>
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

    int r = connect(fd, reinterpret_cast<sockaddr const*>(&ad), sizeof ad);
    if (!r || errno == EAGAIN || gerrno == NET_EINPROGRESS) {
        return r;
    }

    IPV4_EXC();
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
    int s = accept(fd, nullptr, nullptr);
    if (s < 0) {
        IPV4_EXC();
    }
    set_nonblock(fd);
    return s;
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

poll::flag socket::events_() const noexcept {
    poll::flag fl{};
    fl.set_read(bool(on_read_));
    fl.set_write(bool(on_write_));
    fl.set_eof(bool(on_disconnect_));
    return fl;
}

std::function<void(poll::flag const&)> socket::configure_callback_() noexcept {
    return [this](poll::flag const& ev) {
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
        } catch (std::runtime_error const& re) {
            std::cerr << "ERROR: " << re.what() << std::endl;
        } catch (...) {
            std::cerr << "ERROR: " << std::strerror(errno) << std::endl;
        }
        destroyed_ = old_destroyed;
    };
}

socket::socket(io_api::io_context& ctx, ipv4::sock_ufd&& fd, callback_t const& on_disconnect)
        : socket(ctx, std::move(fd), on_disconnect, callback_t{}, callback_t{})
{}

socket::socket(io_api::io_context& ctx, ipv4::sock_ufd&& fd, callback_t on_disconnect
        , callback_t on_read, callback_t on_write)
        : basic_socket(std::move(fd))
        , on_disconnect_(std::move(on_disconnect))
        , on_read_(std::move(on_read))
        , on_write_(std::move(on_write))
        , unit_(&ctx, events_(), fd_.native_handle(), configure_callback_())
        , destroyed_(nullptr) {
    set_nonblock(this->fd_.native_handle());
}

socket::socket(io_api::io_context& ctx, endpoint const& ep, callback_t const& on_disconnect)
        : socket(ctx, ep, on_disconnect, callback_t{}, callback_t{})
{}

socket::socket(io_api::io_context& ctx, endpoint const& ep, callback_t const& on_disconnect
        , callback_t const& on_read
        , callback_t const& on_write)
        : socket(ctx, ipv4::sock_ufd(sock_create(AF_INET, SOCK_STREAM, 0)), {}, {}, {}) {
    if (!sock_connect(fd_.native_handle(), ep)) {
        set_all(on_disconnect, on_read, on_write);
    } else {
        set_on_disconnect([this, on_disconnect] {
            IPV4_EXC("connection failed: " + std::to_string(sock_geterr(fd_.native_handle())));
        });

        set_on_write([this, &ep, on_disconnect, on_read, on_write] {
            if (int r = sock_geterr(fd_.native_handle())) {
                IPV4_EXC("connection failed: " + std::to_string(r));
            }
            sock_connect(fd_.native_handle(), ep);
            set_all(on_disconnect, on_read, on_write);
        });
    }
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

void socket::set_on_disconnect(callback_t on_disconnect) {
    on_disconnect_.swap(on_disconnect);
    unit_.reconfigure_events(events_());
}

void socket::set_on_read(callback_t on_read) {
    on_read_.swap(on_read);
    unit_.reconfigure_events(events_());
}

void socket::set_on_write(callback_t on_write) {
    on_write_.swap(on_write);
    unit_.reconfigure_events(events_());
}

void socket::set_all(callback_t on_disconnect, callback_t on_read, callback_t on_write) {
    on_disconnect_.swap(on_disconnect);
    on_read_.swap(on_read);
    on_write_.swap(on_write);
    unit_.reconfigure_events(events_());
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

server_socket::server_socket(io_api::io_context& ctx, endpoint const& addr, callback_t on_connected)
        : fd_(sock_create(AF_INET, SOCK_STREAM, 0))
        , on_connected_(std::move(on_connected))
        , unit_(&ctx, poll::flag(true, false, false), fd_.native_handle(), [this](poll::flag const& ev) {
            if (ev.read()) {
                on_connected_();
            }
        }) {
    set_nonblock(fd_.native_handle());
    sock_enable_resuseaddr(fd_.native_handle());
    sock_bind(fd_.native_handle(), addr);
    sock_listen(fd_.native_handle(), SSOCK_MAX_LISTEN);
}

socket server_socket::accept(callback_t const& on_disconnect) {
    int fd = sock_accept(fd_.native_handle());
    return socket(*unit_.context(), ipv4::sock_ufd(fd), on_disconnect);
}

socket server_socket::accept(callback_t const& on_disconnect
        , callback_t const& on_read
        , callback_t const& on_write) {
    int fd = sock_accept(fd_.native_handle());
    return socket(*unit_.context(), ipv4::sock_ufd(fd), on_disconnect, on_read, on_write);
}
} // namespace ipv4