#ifndef TGSERVER_REQUEST_H
#define TGSERVER_REQUEST_H

#include <string>
#include <map>
#include <sstream>


#define HTTPCRLF "\r\n"


namespace http {
enum method {
    OPTIONS,
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    TRACE,
    CONNECT,
    EXTENSION,
};

enum version {
    HTTP11,
    UNKNOWN,
};

[[nodiscard]] std::string strmethod(method m);

[[nodiscard]] std::string strversion(version v);

struct response {
    version ver{};
    int code = 0;
    std::string reason;

    std::string body;
    std::map<std::string, std::string> fields;

public:
    [[nodiscard]] std::string to_string() const;
};

struct request {
    method meth{};
    std::string uri;
    version ver{};

    std::string body;
    std::map<std::string, std::string> fields;

public:
    [[nodiscard]] std::string to_string() const;
};
} // namespace http


#endif //TGSERVER_REQUEST_H
