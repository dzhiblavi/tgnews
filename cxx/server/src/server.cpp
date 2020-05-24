#include "server.h"

namespace {
std::mutex errm;

void errlog() {}

template <typename A, typename... Args>
void errlog(A&& a, Args&&... args) {
    std::cout << a << std::endl;
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


deleter::deleter(const std::filesystem::path &base)
    : base(base) {}

void deleter::remove(std::pair<uint8_t, std::filesystem::path> p) {
    static std::set<std::string> categories = {
            "entertainment",
            "society",
            "technology",
            "sports",
            "science",
            "economy",
            "other",
    };
    static std::set<std::string> languages = {
            "ru",
            "en",
    };
    static std::filesystem::path pbase = base / "../../../out/";

    std::filesystem::path const &path = p.second;
    errlog(9, "Removing file: ", path);

    for (auto &lang : languages) {
        for (auto &cat : categories) {
            std::string full_path = pbase.string() + "/" + lang + "/" + cat + "/" + path.string();
            if (std::filesystem::is_regular_file(full_path)) {
                errlog(9, "found file: removing file: ", full_path);
                std::filesystem::remove(full_path);
                return;
            }
        }
    }

    std::lock_guard<std::mutex> lg(m);
    if (--p.first > 0) {
        errlog(9, "retrying to remove the file next time!");
        q.push_back(p);
    }
}

void deleter::submit(const std::filesystem::path &path) {
    std::lock_guard<std::mutex> lg(m);
    q.push_back({MAX_DELETE_DELAY, path});
}

void deleter::wakeup() {
    std::lock_guard<std::mutex> lg(m);

    while (!q.empty()) {
        thp.submit([p{q.back()}, this] {
            remove(p);
        });
        q.pop_back();
    }
}


server::server(std::filesystem::path const& base, io_api::io_context &ctx,
        ipv4::endpoint const &server_ep, ipv4::endpoint const& pyserver_ep)
    : del(base)
    , socket(ctx)
    , pyserver(base, ctx, pyserver_ep) {
    socket.bind(server_ep);
    socket.accept(ipv4::handler<ipv4::socket>([&, this] (ipv4::socket sock) {
        errlog(8, "on_connect");
        client_connection* cc = new client_connection(this, std::move(sock));
        clients.emplace(cc, std::unique_ptr<client_connection>(cc));
    }));

    std::cout << "Server booted" << std::endl;
}

server::client_connection::client_connection(server* srv, ipv4::socket&& sock)
    : srv(srv)
    , socket(std::move(sock))
    , stor(&socket, [this] { on_disconnect(); }) {
    socket.set_on_disconnect([this] { on_disconnect(); });
    socket.read(buff, CLIENT_BUFF_SIZE, ipv4::handler<int>([this] (int r) { on_read(r); }));
}

void server::client_connection::on_disconnect() {
    errlog(10, __func__);
    srv->clients.erase(this);
}

void server::client_connection::on_read(int r) {
    errlog(10, __func__);
    errlog(15, std::string(__func__) + ": '" + std::string(buff, buff + r) + "'");

    int offset = 0;
    int s = parser.feed(buff, offset, r);
    r -= s;
    offset += s;
    while (parser.ready()) {
        srv->request_thp.submit([this, request{parser.get()}] () mutable {
            stor.push(srv->process(std::move(request)).to_string());
        });
        parser.clear();

        s = parser.feed(buff, offset, r);
        r -= s;
        offset += s;
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
            return {http::version::HTTP11, 400, "Bad Request"};
    }
}

http::response server::process_put(http::request&& request) {
    errlog(8, __func__);

    uint64_t end_time = atoi(request.fields["Cache-Control"].substr(8).c_str())
                        + html::parser::extract_time_from_html(request.body);

    http::response result{http::version::HTTP11};
    if (daemon.add(request.uri.substr(1), end_time)) {
        result.code = 201;
        result.reason = "Created";
    } else {
        result.code = 204;
        result.reason = "Updated";
    }

    if (name_daemon::compare_time(end_time)) {
        errlog(10, "article will last");

        worker_thp.submit([this, request{std::move(request)}]() mutable {
            std::unordered_map<std::string, std::string> meta;
            html::parser::extract(request.body, meta);

            request.fields["Content-Length"] = std::to_string(request.body.size());
            errlog(15, "Extraction result: " + request.body);

            std::string lang = detector.detect(request.body).name().substr(0, 2);

            request.fields["Language"] = lang;
            request.fields["header"] = meta["og:title"];
            request.fields["published_time"] = std::to_string(html::parser::extract_time(meta["article:published_time"]));
            request.fields["og_url"] = meta["og:url"];

            errlog(10, "Language detection result: " + lang);

            if (lang == "ru" || lang == "en") {
                errlog(10, "Submiting request");
                pyserver.submit_request(request.to_string());
            } else {
                errlog(10, "Ignoring request");
            }
        });
    } else {
        errlog(10, "article is already rotten");
    }

    return result;
}

http::response server::process_delete(http::request&& request) {
    errlog(8, __func__);

    http::response result = {http::version::HTTP11};
    std::string filename = request.uri.substr(1);
    if (daemon.remove(filename)) {
        result.code = 204;
        result.reason = "No Content";
        del.submit(filename);
    } else {
        result.code = 404;
        result.reason = "Not Found";
    }

    del.wakeup();
    return result;
}

http::response server::process_get(http::request&& request) {
    errlog(8, __func__);

    std::string uri = request.uri;
    request.uri = "/threads";

    std::string period, lang_code, category;
    size_t i = 0;
    while (uri[i] != '=') ++i;
    size_t j = ++i;
    while (uri[j] != '&') ++j;
    period = uri.substr(i, j - i);

    i = j + 1;
    while (uri[i] != '=') ++i;
    j = ++i;
    while (uri[j] != '&') ++j;
    lang_code = uri.substr(i, j - i);

    i = j + 1;
    while (uri[i] != '=') ++i;
    category = uri.substr(++i);

    request.fields.clear();
    request.fields["period"] = period;
    request.fields["lang_code"] = lang_code;
    request.fields["category"] = category;
    request.fields["max_indexed_time"] = "0";

    std::string response = pyserver.submit_and_await(request.to_string());
    http::parser<http::response> parser;
    parser.feed(response.data(), 0, response.size());
    http::response rsp = parser.get();

    rsp.fields.clear();
    rsp.fields["Content-type"] = "application/json";
    rsp.fields["Content-Length"] = rsp.body.size();
    rsp.ver = http::HTTP11;

    return rsp;
}

langdetect::Detected concurrent_detector::detect(std::string const& s) {
    std::lock_guard<std::mutex> lg(m);
    return detector.detect(s.c_str(), s.size());
}
