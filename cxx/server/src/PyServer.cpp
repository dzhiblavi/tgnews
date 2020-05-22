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
    : message(std::move(message))
    , serv(serv) {
    socket.connect(ep, ipv4::handler<>([&, this] {
        socket.write(message.data(), message.size(), ipv4::handler<int>([&, this] (int r) {
            on_write(r);
        }));
    }), [this] {
        this->serv->con.erase(this);
    });
}

void PyServer::connection::on_write(int r) {
    if (r == message.size()) {
        this->serv->con.erase(this);
    } else {
        socket.write(message.data() + r, message.size() - r, ipv4::handler<int>([this] (int r) { on_write(r); }));
    }
}
