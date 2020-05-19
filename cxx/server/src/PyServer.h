#ifndef TGSERVER_PYSERVER_H
#define TGSERVER_PYSERVER_H

#include <functional>

#include "socket.h"
#include "io_api.h"
#include "storage.h"
#include "sysapi.h"


class PyServer {
private:
    ipv4::socket socket;
    storage<std::string> st;

private:
    void on_write();

public:
    PyServer(io_api::io_context& ctx, ipv4::endpoint const& ep);

    ~PyServer();

    void submit_request(std::string const& s);
};


#endif //TGSERVER_PYSERVER_H
