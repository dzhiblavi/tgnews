#ifndef NET_IPV4_EXCEPTION 
#define NET_IPV4_EXCEPTION 

#include <string>
#include <stdexcept>

#define IPV4_ERROR(X) ipv4::error(std::string("IPV4 ERROR: ") + X \
                        + " in file: " + __FILE__ \
                        + " in function: " + __func__ + "(...)" \
                        + " on line: " + std::to_string(__LINE__) \
                        + ": " + std::to_string(gerrno))

#define IPV4_EXC(X) throw IPV4_ERROR(X)

namespace ipv4 {
class error : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
    using std::runtime_error::what;
};
} // namespace ipv4

#endif // NET_IPV4_EXCEPTION 
