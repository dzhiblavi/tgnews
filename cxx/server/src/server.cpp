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

server::server(io_api::io_context &ctx, ipv4::endpoint const &server_ep, ipv4::endpoint const& pyserver_ep)
    : socket(ctx, server_ep, [this, &ctx] { on_connect(ctx); })
    , pyserver(ctx, pyserver_ep) {
    std::cerr << "Server booted" << std::endl;
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

    errlog(15, std::string(__func__) + ": '" + std::string(buff, buff + r) + "'");

    int offset = 0;
    int s = parser.feed(buff, offset, r);
    r -= s;
    offset += s;
    while (parser.ready()) {
        srv->thp.submit([this, request{parser.get()}] () mutable {
            stor.push_front(srv->process(std::move(request)).to_string());
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

    if (r < res.size()) {
        errlog(8, "write cached");
        stor.push_back(res.substr(r, res.size() - r));
    } else {
        // :NOTE: we assume that only one request is made per connection
        on_disconnect();
    }
}

http::response server::process(http::request&& request) {
    switch (request.meth) {
        case http::PUT:
            return process_put(std::move(request));
        case http::GET:
            return process_get(std::move(request));
        case http::DELETE:
            return process_delete(std::move(request));
        default:
            errlog(0, "BAD REQUEST");
    }
    return {http::version::HTTP11, 400, "Bad Request"};
}

http::response server::process_put(http::request&& request) {
    errlog(8, __func__);

//    uint64_t time = html::parser::extract_time(meta["article:published_time"]);
    // TODO: extract time
    http::response result {http::version::HTTP11};
    if (daemon.add(request.uri, -1)) {
        result.code = 201;
        result.reason = "Created";
    } else {
        result.code = 204;
        result.reason = "Updated";
    }

    errlog(0, "threadpool1");
    thp.submit([this, request{std::move(request)}] () mutable {
        std::unordered_map<std::string, std::string> meta;
        html::parser::extract(request.body, meta);

        request.fields["Content-Length"] = std::to_string(request.body.size());
        errlog(15, "Extraction result: " + request.body);

        std::string lang = detector.detect(request.body).name().substr(0, 2);
        request.fields["Language"] = lang;
        errlog(10, "Language detection result: " + lang);

        if (lang == "ru" || lang == "en") {
            errlog(10, "Submiting request");
            pyserver.submit_request(request.to_string());
        } else {
            errlog(10, "Ignoring request");
        }
    });
    errlog(0, "threadpool2");

    return result;
}

http::response server::process_delete(http::request&& request) {
    errlog(8, __func__);

    http::response result = {http::version::HTTP11};
    if (daemon.remove(request.uri)) {
        result.code = 204;
        result.reason = "No Content";
    } else {
        result.code = 404;
        result.reason = "Not Found";
    }

    // TODO: add removing task to thread pool

    return result;
}

http::response server::process_get(http::request&& request) {
    errlog(8, __func__);

    // TODO: process this type of request, lol

    return {http::version::HTTP11, 200, "OK"};
}

langdetect::Detected concurrent_detector::detect(std::string const& s) {
    std::lock_guard<std::mutex> lg(m);
    return detector.detect(s.c_str(), s.size());
}
