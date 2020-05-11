#include "server.h"

namespace {
std::mutex errm;

void errlog() {}

template <typename A, typename... Args>
void errlog(A&& a, Args&&... args) {
    std::cerr << a << std::endl;
    errlog(std::forward<Args>(args)...);
}

template <typename... Args>
void errlog(int lvl, Args&&... args) {
    if (lvl <= ERRLOG_LVL) {
        std::lock_guard<std::mutex> lg(errm);
        errlog(std::forward<Args>(args)...);
    }
}
}


void server::on_connect(io_api::io_context& ctx) {
    errlog(2, __func__);
    auto* cc = new client_connection(ctx, this);
    clients.emplace(cc, std::unique_ptr<client_connection>(cc));
}

server::server(io_api::io_context &ctx, ipv4::endpoint const &ep)
    : socket(ctx, ep, [this, &ctx] { on_connect(ctx); }) {}



int http_buff::append(char *d, int size) {
    return 0;
}

bool http_buff::ready() const noexcept {
    return true;
}

http::request<true> http_buff::get_request() const {
    return {};
}


server::client_connection::client_connection(io_api::io_context &ctx, server *srv)
    : srv(srv)
    , socket(srv->socket.accept(
            [this] { on_disconnect(); },
            [this] { on_read(); },
            {}))
    , stor(&socket, [this] { on_write(); }) {}

void server::client_connection::on_disconnect() {
    errlog(2, __func__);
    srv->clients.erase(this);
}

void server::client_connection::on_read() {
    errlog(4, __func__);

    int r = socket.recv(buff, CLIENT_BUFF_SIZE);
    if (r < 0) {
        if (gerrno == EINTR)
            return;
        IPV4_EXC();
    }

    errlog(15, std::string(__func__) + ": " + std::string(buff, buff + r));
    req_buff.append(buff, r);

    if (req_buff.ready()) {
        thp.submit([request{req_buff.get_request()}] {
            // work on request; ; TODO
            errlog(0, "STUB: REQUEST WORK");
        });
    }
}

void server::client_connection::on_write() {
    errlog(4, __func__);

    std::string res = stor.get();

    int r = socket.send(res.c_str(), res.size());
    if (r < 0) {
        if (errno == EINTR)
            return;
        IPV4_EXC();
    }

    if (r < res.size())
        stor.push_back(res.substr(r, res.size() - r));
}

