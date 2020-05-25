#ifndef TGSERVER_SERVER_H
#define TGSERVER_SERVER_H


#include <atomic>
#include <map>
#include <memory>

#include "net/io_api.h"
#include "net/address.h"
#include "net/timer.h"
#include "net/socket.h"
#include "storage.h"

#include "http/request.h"
#include "http/parser.h"

#include "sysapi/sysapi.h"
#include "sysapi/thread_pool.h"

#include "name_daemon/name_daemon.h"
#include "PyServer.h"
#include "html/parser.h"
#include "language/src/lang_detect/langdetect.h"


#define ERRLOG_LVL 5
#define WORKER_THP_SIZE 8
#define DELETER_THP_SIZE 4
#define MAX_DELETE_DELAY 3
#define CLIENT_BUFF_SIZE 1 << 14


struct concurrent_detector {
    std::mutex m;
    langdetect::Detector detector;

public:
    langdetect::Detected detect(std::string const& data);
};


class deleter {
    std::filesystem::path base;
    std::mutex m;
    std::vector<std::pair<uint8_t, std::filesystem::path>> q;
    thread_pool<DELETER_THP_SIZE> thp;

private:
    void remove(std::pair<uint8_t, std::filesystem::path> p);

public:
    deleter(std::filesystem::path const& base);

    void submit(std::filesystem::path const& path);

    void wakeup();
};


class server {
private:
    struct client_connection;

private:
    thread_pool<WORKER_THP_SIZE> request_thp;
    thread_pool<WORKER_THP_SIZE> worker_thp;
    thread_pool<WORKER_THP_SIZE> get_thp;
    deleter del;
    ipv4::server_socket socket;
    std::map<client_connection*, std::unique_ptr<client_connection>> clients;
    name_daemon daemon;
    PyServer pyserver;
    concurrent_detector detector;
    io_api::io_context* ctx;

private:
    http::response process_put(http::request&& request);

    http::response process_delete(http::request&& request);

    http::response process_get(http::request&& request);

public:
    server(std::filesystem::path const& base, io_api::io_context& ctx,
            ipv4::endpoint const& server_ep, ipv4::endpoint const& pyserver_ep);

    http::response process(http::request&& request);
};

struct server::client_connection {
private:
    server* srv;
    ipv4::socket socket;
    storage stor;

    char buff[CLIENT_BUFF_SIZE];
    http::parser<http::request> parser;

private:
    void on_disconnect();

    void on_read(int r);

public:
    client_connection(server* srv, ipv4::socket&& sock);
};


#endif //TGSERVER_SERVER_H
