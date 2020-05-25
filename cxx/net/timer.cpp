#include "timer.h"

void timer::add(timer_unit* e) {
    timers.insert({e->wpoint, e});
}

void timer::remove(timer_unit* e) noexcept {
    auto it = timers.find({e->wpoint, e});
    timers.erase(it);
}

bool timer::empty() const noexcept {
    return timers.empty();
}

timer::time_point_t timer::top() const noexcept {
    assert(!empty());
    return timers.begin()->second->wpoint;
}

timer_unit::timer_unit(timer_unit&&tu)
    : tr(tu.tr)
    , cb(tu.cb)
    , wpoint(tu.wpoint) {
    tr->remove(&tu);
    tr->add(this);
    tu.tr = nullptr;
}

timer_unit& timer_unit::operator=(timer_unit&&tu) {
    tu.tr->remove(&tu);
    std::swap(tr, tu.tr);
    std::swap(cb, tu.cb);
    std::swap(wpoint, tu.wpoint);
    tu.tr = nullptr;
    tr->add(this);
    return *this;
}

void timer::callback(time_point_t base) noexcept {
    auto it = timers.begin();

    while (it != timers.end() && it->first <= base) {
        it->second->tr = nullptr;

        try {
            it->second->callback();
        } catch (...) {

        }

        it = timers.erase(it);
    }
}

timer_unit::timer_unit(timer *timer, timer::time_point_t wpoint, timer_unit::callback_t callback)
        : tr(timer)
        , cb(std::move(callback))
        , wpoint(wpoint) {
    tr->add(this);
}

timer_unit::~timer_unit() {
    if (tr) {
        tr->remove(this);
    }
}

void timer_unit::reset(timer& t, time_point_t tp) {
    if (tr) {
        tr->remove(this);
    }

    wpoint = tp;
    tr = &t;
    tr->add(this);
}

void timer_unit::reset(time_point_t tp) {
    if (tr) {
        tr->remove(this);
    }

    wpoint = tp;
    tr->add(this);
}

void timer_unit::callback() {
    cb();
}