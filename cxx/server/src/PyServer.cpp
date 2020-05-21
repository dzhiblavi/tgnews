#include "PyServer.h"

#include <utility>

PyServer::PyServer(io_api::io_context &ctx, const ipv4::endpoint &ep)
    : ctx(ctx)
    , serv_addr(ep) {
    std::cerr << "PyServer booted" << std::endl;
}

void PyServer::submit_request(const std::string &s) {
    auto c = new connection(ctx, serv_addr, s, this);
    con.emplace(c, std::unique_ptr<connection>(c));
}

PyServer::connection::connection(io_api::io_context& ctx, ipv4::endpoint const &ep, std::string  message, PyServer *serv)
    : socket(ctx, ep,
            [this] {
                this->serv->con.erase(this);
            }, {},
            [this] { on_write(); })
    , message(std::move(message))
    , serv(serv) {}

void PyServer::connection::on_write() {
    int r = socket.send(message.c_str(), message.size());
    if (r < 0) {
        if (errno == EINTR)
            return;
        IPV4_EXC();
    }

    if (r < message.size()) {
        message = message.substr(r);
    } else {
        serv->con.erase(this);
    }
}
