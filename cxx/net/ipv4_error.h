#ifndef WEB_CRACKER_IPV4_EXCEPTION_H
#define WEB_CRACKER_IPV4_EXCEPTION_H

#include <string>
#include <stdexcept>

#if defined(__linux) || defined(__APPLE__)
#define gerrno errno
#define NET_EINPROGRESS EINPROGRESS
#elif defined(WIN32)
#define gerrno WSAGetLastError()
#define NET_EINPROGRESS WSAEINPROGRESS
#endif

#define IPV4_EXC(X) throw ipv4::error(std::string("IPV4 ERROR: ") + X \
                        + " in function: " + __func__ + "(...)" \
                        + " on line: " + std::to_string(__LINE__) \
                        + ": " + std::to_string(gerrno))

namespace ipv4 {
class error : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
    using std::runtime_error::what;
};
} // namespace ipv4

#endif //WEB_CRACKER_IPV4_EXCEPTION_H