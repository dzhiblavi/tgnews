#ifndef TGSERVER_REQUEST_H
#define TGSERVER_REQUEST_H

#include <string>
#include <map>
#include <sstream>


#define HTTP_ENDL "\r\n"


namespace http {
template<bool isRequest>
struct request;

template<>
struct request<true> {
    std::string method;
    std::string path;
    std::string version = "HTTP/1.1";
    std::string content_type = "text/html";
    size_t cache_keepalive = 0;
    size_t content_length = 0;
    std::string content;

    [[nodiscard]] std::string to_string() const {
        std::stringstream ss;

        ss << method << ' ' << path << ' ' << version << HTTP_ENDL
           << "Content-Type: " << content_type << HTTP_ENDL
           << "Cache-Control: max-age=" << cache_keepalive << HTTP_ENDL
           << "Content-Length: " << content_length << HTTP_ENDL
           << HTTP_ENDL
           << content << HTTP_ENDL;

        return ss.str();
    }
};

template<>
struct request<false> {
    std::string version;
    int code;
    std::string comment;
    size_t content_length;
    std::string content;

    [[nodiscard]] std::string to_string() const {
        std::stringstream ss;

        ss << version << " " << code << " " << comment << HTTP_ENDL
           << "Content-type: application/json" << HTTP_ENDL
           << "Content-length: " << content_length << HTTP_ENDL
           << HTTP_ENDL
           << content << HTTP_ENDL;

        return ss.str();
    }
};
} // namespace http


#endif //TGSERVER_REQUEST_H
