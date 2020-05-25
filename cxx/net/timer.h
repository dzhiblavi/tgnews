#ifndef NET_TIMER_H
#define NET_TIMER_H

#include <functional>
#include <chrono>
#include <set>
#include <cassert>

class timer_unit;

class timer {
public:
    typedef std::chrono::steady_clock clock_t;
    typedef clock_t::time_point time_point_t;

private:
    std::set<std::pair<time_point_t, timer_unit*>> timers;

public:
    timer() noexcept = default;

    void add(timer_unit* e);
    void remove(timer_unit* e) noexcept;

    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] time_point_t top() const noexcept;
    void callback(time_point_t base) noexcept;
};

class timer_unit {
public:
    friend class timer;
    typedef std::function<void()> callback_t;
    typedef timer::time_point_t time_point_t;

private:
    timer* tr = nullptr;
    callback_t cb{};
    time_point_t wpoint{};

public:
    timer_unit() = default;
    timer_unit(timer* timer, time_point_t wpoint, callback_t callback);
    ~timer_unit();

    timer_unit(timer_unit&& tu);
    timer_unit& operator=(timer_unit&&);

    void reset(timer& t, time_point_t tp);
    void reset(time_point_t tp);
    void callback();
};

#endif // NET_TIMER_H