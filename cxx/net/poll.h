#ifndef WEB_CRACKER_POLL_H
#define WEB_CRACKER_POLL_H

#ifdef WIN32
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00
#include <ws2tcpip.h>
#include <winsock2.h>
#include <mstcpip.h>
#include <windows.h>
#endif

#include <cstring>
#include <cassert>
#include <array>
#include <vector>
#include <map>
#include <queue>


#define NET_APPLE_KQUEUE

#if defined(__APPLE__)
#if defined(NET_APPLE_POLL)
#define NET_POLL_POLL
#include <sys/poll.h>
#elif defined(NET_APPLE_KQUEUE)
#define NET_POLL_KQUEUE
#include <sys/event.h>
#endif
#elif defined(__linux)
#define NET_POLL_EPOLL
#define NET_POLL_SIZE 1000
#include <sys/epoll.h>
#elif defined(WIN32)
#define NET_POLL_POLL
static inline int poll(pollfd *pfd, int nfds, int timeout) { return WSAPoll(pfd, nfds, timeout); }
#endif

#ifdef WIN32
typedef SOCKET sock_fd_t;
#define NET_SOCK_CLOSE closesocket
#define NET_BUFF_PTR char*
#define NET_BUFF_CPTR char const*
#define NET_INVALID_SOCKET INVALID_SOCKET
#elif defined(__linux) || defined(__APPLE__)
typedef int sock_fd_t;
#define NET_SOCK_CLOSE close
#define NET_BUFF_PTR void*
#define NET_BUFF_CPTR void const*
#define NET_INVALID_SOCKET -1
#endif

#include "unique_fd.h"

class poll {
public:
#ifdef WIN32
    typedef SOCKET sock_fd_t;
#elif defined(__linux) || defined(__APPLE__)
    typedef int sock_fd_t;
#endif

#if defined(NET_POLL_KQUEUE) || defined(NET_POLL_EPOLL)
    typedef int native_handle_t;
#endif

#if defined(NET_POLL_KQUEUE)
    typedef struct kevent event;
#elif defined(NET_POLL_EPOLL)
    typedef struct epoll_event event;
#elif defined(NET_POLL_POLL)
    typedef struct pollfd event;
#endif

public:
    enum CTL {
#if defined(NET_POLL_KQUEUE)
        ADD = EV_ADD,
        REMOVE = EV_DELETE,
        MODIFY,
#elif defined(NET_POLL_EPOLL)
        ADD = EPOLL_CTL_ADD,
        REMOVE = EPOLL_CTL_DEL,
        MODIFY = EPOLL_CTL_MOD,
#elif defined(NET_POLL_POLL)
        ADD,
        REMOVE,
        MODIFY,
#endif
    };

    struct flag {
    private:
#if defined(NET_POLL_EPOLL)
        uint32_t events_ = 0;
#elif defined(NET_POLL_KQUEUE)
        bool read_ = false;
        bool write_ = false;
        bool eof_ = false;
#elif defined(NET_POLL_POLL)
        short events_ = 0;
#endif

    public:
        flag() = default;

        explicit flag(event const&);

        flag(flag const& o);

        flag(bool, bool, bool);

        void set_read(bool);

        void set_write(bool);

        void set_eof(bool);

        [[nodiscard]] bool read() const;

        [[nodiscard]] bool write() const;

        [[nodiscard]] bool eof() const;

#if defined(NET_POLL_EPOLL)
        uint32_t events() const;
#elif defined(NET_POLL_POLL)
        [[nodiscard]] short events() const;
#endif
    };

    friend struct event_info;
    struct event_info {
    private:
        sock_fd_t fd_;
        void* data_;
        flag f_{};

#ifdef NET_POLL_KQUEUE
        void register_filters(int pfd, event* ev);
#endif

    public:
        event_info(int fd, void* data);

        event_info(int fd, void* data, flag const& f);

        event_info(event_info const& ev) = delete;

        event_info(event_info&& ev) = default;

        [[nodiscard]] sock_fd_t fd() const;

        flag& get_flag();

        [[nodiscard]] flag const& get_flag() const;

        void event_ctl_apply(CTL ctl, poll& poll);
    };

    struct ev_it {
        typedef event value_type;
        typedef event& reference;
        typedef std::ptrdiff_t difference_type;
        typedef event* pointer;
        typedef std::forward_iterator_tag iterator_category;

        friend class poll;

    private:
        event* base = nullptr;
        event* end = nullptr;

    private:
        ev_it(pointer base, pointer end);

    public:
        ev_it() = default;

        ev_it(ev_it const&) = default;

        ev_it& operator=(ev_it const&) = default;

        friend bool operator==(ev_it const& a, ev_it const& b);

        friend bool operator!=(ev_it const& a, ev_it const& b);

        reference operator*() const;

        pointer operator->() const;

        ev_it& operator++();

        const ev_it operator++(int);
    };

private:
#if defined(NET_POLL_EPOLL) || defined(NET_POLL_KQUEUE)
    unique_fd<sock_fd_t, NET_INVALID_SOCKET, NET_SOCK_CLOSE> qfd_;
    std::array<event, 10000> events_;
#endif

#if defined(NET_POLL_POLL)
    std::vector<event> events_;
    std::queue<std::pair<CTL, event_info>> q_;
    std::map<int, void*> data_;
#endif

public:
    poll();

    poll(poll&&) = default;

    poll& operator=(poll&&) = default;

    void event_ctl(CTL ctl, event_info& evi);

    int wait(int time);

    ev_it begin(int size);

    ev_it end(int size);

    void* get_data(event& e);

#if defined(NET_POLL_EPOLL) || defined(NET_POLL_KQUEUE)
    [[nodiscard]] native_handle_t native_handle() const noexcept;
#endif
};


#endif //WEB_CRACKER_POLL_H
