#ifndef TGSERVER_PYSERVER_H
#define TGSERVER_PYSERVER_H

#include <functional>
#include <set>

#include "socket.h"
#include "io_api.h"
#include "storage.h"
#include "sysapi.h"


class PyServer {
private:
    struct connection;

private:
    std::map<connection*, std::unique_ptr<connection>> con;
    ipv4::endpoint serv_addr;
    io_api::io_context& ctx;

public:
    PyServer(io_api::io_context& ctx, ipv4::endpoint const& ep);

    ~PyServer() = default;

    void submit_request(std::string const& s);
};

struct PyServer::connection {
    ipv4::socket socket;
    std::string message;
    PyServer* serv;

private:
    void on_write();

public:
    connection(io_api::io_context& ctx, ipv4::endpoint const& ep, std::string  message, PyServer* serv);
};


#endif //TGSERVER_PYSERVER_H
