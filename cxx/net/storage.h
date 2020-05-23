#ifndef SERVER_STORAGE
#define SERVER_STORAGE

#include <deque>
#include <mutex>

#include "socket.h"

struct storage {
    ipv4::socket* socket;
    std::function<void()> on_end;
    std::deque<std::string> data;
    std::mutex m;

    void on_write(int r) {
        if (r == data.back().size()) {
            data.pop_back();
        } else {
            data.back() = data.back().substr(r);
        }

        if (data.empty()) {
            socket->write(nullptr, 0, {});
            on_end();
        } else {
            set_on_write(true);
        }
    }

    void set_on_write(bool next = false) {
        if (data.size() == 1 || next) {
            socket->write(data.back().data(), data.back().size(), ipv4::handler<int>([this] (int r) { on_write(r); }));
        }
    }

private:
    void push_back(std::string const& value) {
        std::lock_guard<std::mutex> lg(m);
        data.push_back(value);
        set_on_write();
    }

    void push_back(std::string&& value) {
        std::lock_guard<std::mutex> lg(m);
        data.push_back(std::move(value));
        set_on_write();
    }

public:
    storage(ipv4::socket* cc, std::function<void()> const& on_end)
            : socket(cc), on_end(on_end) {}

    void push(std::string const& value) {
        std::lock_guard<std::mutex> lg(m);
        data.push_front(value);
        set_on_write();
    }

    void push(std::string&& value) {
        std::lock_guard<std::mutex> lg(m);
        data.push_front(std::move(value));
        set_on_write();
    }
};

#endif //SERVER_STORAGE