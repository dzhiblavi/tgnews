#include "request.h"

namespace {
[[nodiscard]] std::string form_header(std::map<std::string, std::string> const& m) {
    std::stringstream ss;
    for (auto const& p : m) {
        ss << p.first << ": " << p.second << HTTPCRLF;
    }
    return ss.str();
}
}


namespace http {
std::string strmethod(method m) {
    switch (m) {
        case OPTIONS:
            return "OPTIONS";
        case GET:
            return "GET";
        case HEAD:
            return "HEAD";
        case POST:
            return "POST";
        case PUT:
            return "PUT";
        case DELETE:
            return "DELETE";
        case TRACE:
            return "TRACE";
        case CONNECT:
            return "CONNECT";
        case EXTENSION:
            return "EXT";
    }
}

std::string strversion(version v) {
    switch (v) {
        case HTTP11:
            return "HTTP/1.1";
        case UNKNOWN:
            return "UNKNOWN";
    }
}


std::string response::to_string() const {
    std::stringstream ss;
    ss << strversion(ver) << ' ' << code << ' ' << reason << HTTPCRLF
       << form_header(fields) << HTTPCRLF
       << HTTPCRLF
       << body;
    return ss.str();
}


std::string request::to_string() const {
    std::stringstream ss;
    ss << strmethod(meth) << ' ' << uri << ' ' << strversion(ver) << HTTPCRLF
       << form_header(fields) << HTTPCRLF
       << HTTPCRLF
       << body;
    return ss.str();
}
}