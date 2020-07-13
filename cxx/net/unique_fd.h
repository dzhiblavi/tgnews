#ifndef NET_UNIQUE_FD_H
#define NET_UNIQUE_FD_H

#include <unistd.h>
#include <algorithm>
#include <iostream>

template <typename T, T Default, int(*cls)(T)>
class unique_fd {
    T fd = Default;

public:
    unique_fd() noexcept = default;

    explicit unique_fd(T fd) noexcept
        : fd(fd) {}

    ~unique_fd() {
        cls(fd);
    }

    unique_fd(unique_fd const&) = delete;

    unique_fd& operator=(unique_fd const&) = delete;

    unique_fd(unique_fd&& uf) noexcept
        : fd(uf.fd) {
        uf.fd = Default;
    }

    unique_fd& operator=(unique_fd&& uf) noexcept {
        if (this != &uf)
            std::swap(fd, uf.fd);
        return *this;
    }

    [[nodiscard]] T native_handle() const noexcept {
        return fd;
    }

    friend void swap(unique_fd& u1, unique_fd& u2) noexcept {
        std::swap(u1.fd, u2.fd);
    }
};

#endif // NET_UNIQUE_FD_H
