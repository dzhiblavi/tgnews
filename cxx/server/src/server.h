#ifndef TGSERVER_SERVER_H
#define TGSERVER_SERVER_H

#include <map>
#include <memory>

#include "io_api.h"
#include "address.h"
#include "timer.h"
#include "socket.h"
#include "storage.h"

#include "request.h"
#include "parser.h"

#include "sysapi.h"
#include "thread_pool.h"

#include "name_daemon.h"
#include "PyServer.h"


#define ERRLOG_LVL 3
#define CLIENT_THP_SIZE 2
#define CLIENT_BUFF_SIZE 1000


class server {
private:
    struct client_connection;

private:
    ipv4::server_socket socket;
    std::map<client_connection*, std::unique_ptr<client_connection>> clients;
    name_daemon daemon;
    PyServer pyserver;

private:
    void on_connect(io_api::io_context& ctx);

public:
    server(io_api::io_context& ctx, ipv4::endpoint const& server_ep, ipv4::endpoint const& pyserver_ep);

    void process(const http::request &request);
};

struct server::client_connection {
private:
    server* srv;
    ipv4::socket socket;
    thread_pool<CLIENT_THP_SIZE> thp;
    storage<std::string> stor;

    char buff[CLIENT_BUFF_SIZE]{0};
    http::parser<http::request> parser;

private:
    void on_disconnect();

    void on_read();

    void on_write();

public:
    client_connection(io_api::io_context& ctx, server* srv);
};


#endif //TGSERVER_SERVER_H
