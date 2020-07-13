#ifdef WIN32
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00
#include <ws2tcpip.h>
#include <winsock2.h>
#include <mstcpip.h>
#include <windows.h>

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

#elif defined(__linux) || defined(__APPLE__)
#include <sys/socket.h>

#define NET_POLL_POLL
static inline int poll(pollfd *pfd, int nfds, int timeout) { return WSAPoll(pfd, nfds, timeout); }

#define gerrno WSAGetLastError()
#define NET_EINPROGRESS WSAEINPROGRESS
#endif

#if defined(__linux) 
#include <arpa/inet.h>
#include <netdb.h>

#define gerrno errno
#define NET_EINPROGRESS EINPROGRESS

#define NET_POLL_EPOLL
#define NET_POLL_SIZE 1000000
#include <sys/epoll.h>

typedef int sock_fd_t;
#define NET_SOCK_CLOSE close
#define NET_BUFF_PTR void*
#define NET_BUFF_CPTR void const*
#define NET_INVALID_SOCKET -1
#endif

#if defined(__APPLE__)
#define NET_APPLE_KQUEUE

#include <arpa/inet.h>
#include <netdb.h>

#define gerrno errno
#define NET_EINPROGRESS EINPROGRESS

#if defined(NET_APPLE_POLL)
#define NET_POLL_POLL
#include <sys/poll.h>
#elif defined(NET_APPLE_KQUEUE)
#define NET_POLL_KQUEUE
#include <sys/event.h>
#endif

typedef int sock_fd_t;
#define NET_SOCK_CLOSE close
#define NET_BUFF_PTR void*
#define NET_BUFF_CPTR void const*
#define NET_INVALID_SOCKET -1
#endif

