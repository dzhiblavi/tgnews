#ifndef SERVER_STORAGE
#define SERVER_STORAGE

#include <cstdio>
#include <deque>
#include <mutex>

#include "net/socket.h"
#include "net/timer.h"


struct storage {
    typedef std::chrono::time_point<std::chrono::steady_clock, std::chrono::seconds> time_point_t;

    ipv4::socket* socket;
    std::function<void()> disconnect;
    std::deque<std::string> data;
    std::mutex m;
    int jobs = 0;
    bool ds = false;
    timer* tr;
    timer_unit tu{};

    time_point_t current_time() {
       return std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::steady_clock::now());
    }

    void on_write(int r) {
        if (r == data.back().size()) {
            data.pop_back();
        } else {
            data.back() = data.back().substr(r);
        }

        if (data.empty()) {
            socket->write(nullptr, 0, {});
            if (ds)
                disconnect();
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
    storage(ipv4::socket* cc, timer* tr, std::function<void()> const& disconnect)
            : socket(cc), tr(tr), disconnect(disconnect) {}

    void push(std::string const& value) {
        std::lock_guard<std::mutex> lg(m);
        data.push_front(value);
        set_on_write();
        --jobs;
    }

    void push(std::string&& value) {
        std::lock_guard<std::mutex> lg(m);
        data.push_front(std::move(value));
        set_on_write();
        --jobs;
    }

    void register_task() {
        std::lock_guard<std::mutex> lg(m);
        ++jobs;
    }

    void set_end_point(int seconds) {
        std::lock_guard<std::mutex> lg(m);

        tu = timer_unit(tr, current_time() + std::chrono::seconds(seconds), [this] {
            if (jobs == 0 && data.empty())
                disconnect();
        });
    }

    void set_disconnect_first() {
        std::lock_guard<std::mutex> lg(m);
        ds = true;
    }
};

#endif //SERVER_STORAGE
