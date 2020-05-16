#include "parser.h"


namespace http {
void base_parser::parse_header_line() {
    std::stringstream ss(line);
    std::string key;
    ss >> key;
    key.pop_back();

    if (key == "Content-Length") {
        content_present = true;
        ss >> content_size;
        m[key] = std::to_string(content_size);
    } else {
        m[key] = line.substr(key.size() + 2, line.size() - key.size() - 4);
    }
}

int base_parser::feed_content(char const* data, size_t offset, size_t length) {
    size_t sz = std::min(length, content_size);
    content_size -= sz;
    body += std::string(data + offset, data + offset + sz);
    return sz;
}

bool base_parser::end_of_line(char const* data, size_t i) const {
    if (i == 0)
        return data[0] == '\n' && (!line.empty() && line.back() == '\r');
    return data[i - 1] == '\r' && data[i] == '\n';
}

void base_parser::flush(const char *data, size_t sz) {
    line.append(std::string(data, data + sz));
    if (line == HTTPCRLF) {
        header_ended = true;
    } else if (!first_parsed){
        parse_first_line();
        first_parsed = true;
    } else {
        parse_header_line();
    }
    line.clear();
}


int base_parser::feed(char const* data, size_t offset, size_t length) {
    if (header_ended)
        return feed_content(data, offset, length);

    data += offset;
    size_t i = 0;
    size_t sz = 0;

    while (sz < length) {
        if (end_of_line(data, i)) {
            ++i;
            flush(data, i);
            data += i;
            i = 0;

            if (header_ended) {
                return sz + 1 + feed_content(data, 0, length - sz - 1);
            }
        } else {
            ++i;
        }
        ++sz;
    }

    line.append(std::string(data, data + i));
    return sz;
}


void base_parser::clear() {
    m.clear();
    body.clear();
    line.clear();
    header_ended = false;
    content_present = false;
    first_parsed = false;
    content_size = 0;
}

bool base_parser::ready() const {
    return header_ended && (!content_present || content_size == 0);
}


method base_parser::parse_method(std::string const& s) {
    if (s == "OPTIONS") {
        return OPTIONS;
    } else if (s == "GET") {
        return GET;
    } else if (s == "HEAD") {
        return HEAD;
    } else if (s == "POST") {
        return POST;
    } else if (s == "PUT") {
        return PUT;
    } else if (s == "DELETE") {
        return DELETE;
    } else if (s == "TRACE") {
        return TRACE;
    } else if (s == "CONNECT") {
        return CONNECT;
    } else {
        return EXTENSION;
    }
}

version base_parser::parse_version(std::string const& s) {
    if (s == "HTTP/1.1") {
        return HTTP11;
    } else {
        return UNKNOWN;
    }
}
}