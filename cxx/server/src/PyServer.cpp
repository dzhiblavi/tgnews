#include "PyServer.h"
#include <utility>

PyServer::PyServer(std::filesystem::path const& base, io_api::io_context &ctx, const ipv4::endpoint &ep)
    : ctx(ctx)
    , serv_addr(ep) {
    std::string s = "/usr/bin/python3";
    pid = sysapi::execute(s, s, std::string(base / "../../../bin/python/watch.pyc"),
            std::to_string(ep.host_port()));
    sleep(1);

    std::cout << "PyServer booted" << std::endl;
}

PyServer::~PyServer() {
    sysapi::kill(pid);
}

void PyServer::submit_request(const std::string &s) {
    auto c = new connection(ctx, serv_addr, s, this);
    con.emplace(c, std::unique_ptr<connection>(c));
}

PyServer::connection::connection(io_api::io_context& ctx, ipv4::endpoint const &ep, std::string message, PyServer *serv)
    : socket(ctx)
    , message(std::move(message))
    , serv(serv) {

    socket.connect(ep, ipv4::handler<>([message, this] () mutable {
        socket.write(message.data(), message.size(), ipv4::handler<int>([&, this] (int r) {
            on_write(r);
        }));
        }, [] (std::runtime_error re) {
            std::cerr << "Failed to connect to PyServer: " << re.what() << std::endl;
        }), [this] {
        this->serv->con.erase(this);
    });
}

std::string PyServer::submit_and_await(std::string const& ss) {
    ipv4::basic_socket sock(serv_addr);
    std::string s = ss;
    while (!s.empty()) {
        int r = sock.send(s.data(), s.size());
        s = s.substr(r);
    }
    std::string resp;
    char buff[1024];
    for (;;) {
        int r = sock.recv(buff, 1024);
        if (r == 0) {
            break;
        }
        resp += std::string(buff, buff + r);
    }
    return resp;
}

void PyServer::connection::on_write(int r) {
    if (r == message.size()) {
        this->serv->con.erase(this);
    } else {
        message = message.substr(r);
        socket.write(message.data(), message.size(), ipv4::handler<int>([this] (int r) { on_write(r); }));
    }
}
