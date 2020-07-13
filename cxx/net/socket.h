#ifndef NET_SOCKET_H
#define NET_SOCKET_H

#define SSOCK_MAX_LISTEN 20000

#include <functional>
#include <cstring>
#include <cassert>

#include "ipv4_error.h"
#include "unique_fd.h"
#include "address.h"
#include "io_api.h"
#include "handler.h"


namespace ipv4 {
typedef unique_fd<sock_fd_t, NET_INVALID_SOCKET, NET_SOCK_CLOSE> sock_ufd;

class basic_socket {
protected:
    sock_ufd fd_;

protected:
    explicit basic_socket(sock_ufd&&);

public:
    basic_socket() = default;

    explicit basic_socket(endpoint const& ep);

    basic_socket(basic_socket const&) = delete;

    basic_socket& operator=(basic_socket const&) = delete;

    basic_socket(basic_socket&&) noexcept = default;

    basic_socket& operator=(basic_socket&&) noexcept = default;

    int recv(NET_BUFF_PTR buff, size_t max_len) noexcept;

    int send(NET_BUFF_CPTR buff, size_t max_len) noexcept;

    void set_nonblock();
};


class socket : private basic_socket {
    friend class server_socket;

public:
    typedef handler<int> rw_callback_t;
    typedef handler<> con_callback_t;
    typedef std::function<void()> dc_callback_t;

private:
    typedef std::function<void()> callback_t;
    callback_t on_read_{};
    callback_t on_write_{};
    callback_t on_disconnect_{};

    io_api::io_unit unit_{};
    bool *destroyed_ = nullptr;

private:
    [[nodiscard]] poll::flag events_() const noexcept;

    [[nodiscard]] std::function<void(poll::flag const&)> configure_callback_() noexcept;

private:
    socket(io_api::io_context& ctx, int fd);

public:
    socket(io_api::io_context& ctx);

    ~socket();

    socket(socket const&) = delete;

    socket& operator=(socket const&) = delete;

    socket(socket&&) noexcept;

    socket& operator=(socket&&) noexcept;

    void connect(ipv4::endpoint const& ep, con_callback_t const& on_connect, dc_callback_t const& on_disconnect);

    void read(char* buff, size_t size, rw_callback_t const& on_read);

    void write(char* buff, size_t size, rw_callback_t on_write);

    void set_on_write(callback_t const& cb);

    void set_on_read(callback_t const& cb);

    void set_on_disconnect(callback_t const& cb);

    [[nodiscard]] bool has_on_disconnect() const noexcept;

    [[nodiscard]] bool has_on_read() const noexcept;

    [[nodiscard]] bool has_on_write() const noexcept;

    using basic_socket::recv;

    using basic_socket::send;

    friend void swap(socket&, socket&) noexcept;
};

class server_socket {
public:
    typedef std::function<void()> callback_t;
    typedef std::function<void()> dc_callback_t;
    typedef handler<ipv4::socket> con_callback_t;

private:
    sock_ufd fd_{};
    callback_t on_connected_{};
    io_api::io_context& ctx;
    io_api::io_unit unit_{};

public:
    server_socket(io_api::io_context& ctx);

    void bind(ipv4::endpoint const& ep);

    void accept(con_callback_t const& on_connect);
};
} // namespace ipv4

#endif // NET_SOCKET_H
