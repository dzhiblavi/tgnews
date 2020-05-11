#ifndef SERVER_STORAGE
#define SERVER_STORAGE

#include <deque>
#include <mutex>

#include "socket.h"

template <typename T>
struct storage {
    ipv4::socket* socket;
    std::function<void()> on_write;
    std::deque<T> data;
    std::mutex m;

    void set_on_write() {
        if (!socket->has_on_write())
            socket->set_on_write(on_write);
    }

public:
    storage(ipv4::socket* cc, std::function<void()>&& ow)
            : socket(cc), on_write(std::move(ow)) {}

    void push_back(T const& value) {
        std::lock_guard<std::mutex> lg(m);
        data.push_back(value);
        set_on_write();
    }

    void push_back(T&& value) {
        std::lock_guard<std::mutex> lg(m);
        data.push_back(std::move(value));
        set_on_write();
    }

    void push_front(T const& value) {
        std::lock_guard<std::mutex> lg(m);
        data.push_front(value);
        set_on_write();
    }

    void push_front(T&& value) {
        std::lock_guard<std::mutex> lg(m);
        data.push_front(std::move(value));
        set_on_write();
    }

    T get() {
        std::lock_guard<std::mutex> lg(m);
        assert(!data.empty());
        T r = std::move(data.back());
        data.pop_back();
        if (data.empty())
            socket->set_on_write({});
        return r;
    }
};

#endif //SERVER_STORAGE