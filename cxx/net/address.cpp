#include "address.h"
#include "ipv4_error.h"

namespace {
uint32_t try_resolve(std::string const& host) {
    hostent* hst = gethostbyname(host.c_str());

    if (hst == nullptr || hst->h_addr == nullptr) {
        IPV4_EXC("failed to resolve: " + host);
    }

    uint32_t ret = 0;
    memcpy((char*)&ret, hst->h_addr, hst->h_length);
    return ret;
}
}

namespace ipv4 {
address::address(uint32_t addr) noexcept
        : addr_(addr)
{}

address::address(std::string const& host)
        : addr_(try_resolve(host))
{}

address address::any() noexcept {
    return address(INADDR_ANY);
}

uint32_t address::net_addr() const noexcept {
    return addr_;
}

std::string address::to_string() const {
#ifdef WIN32
    in_addr addr{};
    addr.S_un.S_addr = net_addr();
#elif defined(__linux) || defined(__APPLE__)
    in_addr addr{net_addr()};
#endif
    return inet_ntoa(addr);
}

address address::resolve(std::string const& host) {
    return address(host);
}

std::list<address> address::getaddrinfo(std::string const& hostname) {
    addrinfo hints{}, *result = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    std::list<address> ret;
    int r = ::getaddrinfo(hostname.c_str(), nullptr, &hints, &result);

    if (0 != r) {
        IPV4_EXC("address::getaddrinfo failed: " + std::string(gai_strerror(r)));
    }

    for (addrinfo* nd = result; nd != nullptr; nd = nd->ai_next) {
        ret.emplace_back(reinterpret_cast<sockaddr_in *>(nd->ai_addr)->sin_addr.s_addr);
    }

    freeaddrinfo(result);
    return ret;
}


endpoint::endpoint(uint32_t addr, uint16_t port) noexcept
        : addr_(addr)
        , port_(htons(port))
{}

endpoint::endpoint(std::string const& host, uint16_t port)
        : addr_(try_resolve(host))
        , port_(htons(port))
{}

endpoint::endpoint(address const& addr, uint16_t port) noexcept
        : addr_(addr.net_addr())
        , port_(htons(port))
{}

uint32_t endpoint::net_addr() const noexcept {
    return addr_;
}

uint16_t endpoint::port() const noexcept {
    return port_;
}

uint16_t endpoint::host_port() const noexcept {
    return ntohs(port());
}

std::string endpoint::to_string() const {
    return address(net_addr()).to_string() + ":" + std::to_string(host_port());
}

std::ostream& operator<<(std::ostream& os, address const& addr) {
    os << addr.to_string();
    return os;
}

std::ostream& operator<<(std::ostream& os, endpoint const& ep) {
    os << ep.to_string();
    return os;
}
} // namespace ipv4
