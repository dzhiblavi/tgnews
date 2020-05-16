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

http::response process_request(http::request const& req) {
    // TODO
    errlog(0, std::string("STUB CALLED: ") + __func__);
    errlog(0, "'" + req.to_string() + "'");
    return {};
}
}


void server::on_connect(io_api::io_context& ctx) {
    errlog(2, __func__);
    auto* cc = new client_connection(ctx, this);
    clients.emplace(cc, std::unique_ptr<client_connection>(cc));
}

server::server(io_api::io_context &ctx, ipv4::endpoint const &ep)
    : socket(ctx, ep, [this, &ctx] { on_connect(ctx); }) {}


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

    errlog(15, std::string(__func__) + ": '" + std::string(buff, buff + r) + "'");

    int offset = 0;
    int s = parser.feed(buff, offset, r);
    r -= s;
    offset += s;
    while (parser.ready()) {
        thp.submit([this, request{parser.get()}] {
            process_request(request);
            stor.push_front("STUB: " + request.to_string());
        });
        parser.clear();

        s = parser.feed(buff, offset, r);
        r -= s;
        offset += s;
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

