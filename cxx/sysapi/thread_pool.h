#ifndef SERVER_THREADPOOL
#define SERVER_THREADPOOL

#include <array>
#include <thread>
#include <queue>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <functional>

struct fawait {
private:
    struct state {
        std::condition_variable cv;
        std::mutex m;
        bool finished = false;

        void await() {
            std::unique_lock<std::mutex> lg(m);
            cv.wait(lg, [this] {
                return finished;
            });
        }

        void on_finish() {
            {
                std::lock_guard<std::mutex> lg(m);
                finished = true;
            }
            cv.notify_all();
        }
    };

private:
    std::shared_ptr<state> st;

public:
    fawait() {
        st = std::make_shared<state>();
    }

    ~fawait() = default;

    fawait(fawait const&) = default;

    fawait& operator=(fawait const&) = default;

    void await() {
        st->await();
    }

    void on_finish() {
        st->on_finish();
    }
};

template <size_t N>
class thread_pool {
public:
    typedef std::function<void()> runnable;

private:
    std::array<std::thread, N> th;
    std::queue<std::pair<fawait, runnable>> queue;
    std::mutex m;
    std::condition_variable cv;
    bool finish = false;
    bool terminated = false;

public:
    thread_pool() {
        for (size_t i = 0; i < N; ++i) {
            th[i] = std::thread([this] {
                for (;;) {
                    std::unique_lock<std::mutex> lg(m);
                    cv.wait(lg, [this] {
                        return !queue.empty() || finish || terminated;
                    });

                    if (finish || (terminated && queue.empty())) {
                        break;
                    }

                    auto p = std::move(queue.front());
                    queue.pop();

                    lg.unlock();

                    try {
                        p.second();
                    } catch (...) {
                    }
                    p.first.on_finish();
                }
            });
        }
    }

    ~thread_pool() {
        if (terminated)
            return;
        {
            std::lock_guard<std::mutex> lg(m);
            finish = true;
            queue = std::queue<std::pair<fawait, runnable>>();
        }
        cv.notify_all();
        for (std::thread& t : th) {
            t.join();
        }
    }

    void await() {
        if (terminated)
            return;
        {
            std::lock_guard<std::mutex> lg(m);
            terminated = true;
        }
        cv.notify_all();
        for (std::thread& t : th) {
            t.join();
        }
    }

    fawait submit(runnable const& r) {
        fawait fa;
        {
            std::lock_guard<std::mutex> lg(m);
            queue.push({fa, r});
        }
        cv.notify_one();
        return fa;
    }

    fawait submit(runnable&& r) {
        fawait fa;
        {
            std::lock_guard<std::mutex> lg(m);
            queue.push({fa, std::move(r)});
        }
        cv.notify_one();
        return fa;
    }
};


#endif //SERVER_THREADPOOL