#ifndef TGSERVER_PARSER_H
#define TGSERVER_PARSER_H

#include "request.h"


namespace http {
class base_parser {
protected:
    std::map<std::string, std::string> m;
    std::string body;
    std::string line;
    bool header_ended = false;
    bool content_present = false;
    bool first_parsed = false;
    size_t content_size = 0;

private:
    void parse_header_line();

    int feed_content(char const* data, size_t offset, size_t length);

    bool end_of_line(char const* data, size_t i) const;

    void flush(char const* data, size_t sz);

protected:
    virtual void parse_first_line() = 0;

public:
    base_parser() = default;

    void clear();

    int feed(char const* data, size_t offset, size_t length);

    [[nodiscard]] bool ready() const;

    static method parse_method(std::string const& s);

    static version parse_version(std::string const& s);
};

template <typename Obj>
class parser;

template <>
class parser<request> : public base_parser {
private:
    method meth{};
    std::string uri;
    version ver{};

private:
    void parse_first_line() override {
        std::stringstream ss(line);
        std::string s;

        ss >> s;
        meth = parse_method(s);
        ss >> uri;
        ss >> s;
        ver = parse_version(s);
    }

public:
    parser() = default;

    [[nodiscard]] request get() const {
        return {meth, uri, ver, body, m};
    }
};

template <>
class parser<response> : public base_parser {
private:
    version ver{};
    int code = 0;
    std::string reason;

private:
    void parse_first_line() override {
        std::stringstream ss(line);
        std::string s;

        ss >> s;
        ver = parse_version(s);
        ss >> code;
        ss >> reason;
    }

public:
    [[nodiscard]] response get() const {
        return {ver, code, reason, body, m};
    }
};
} // namespace http


#endif //TGSERVER_PARSER_H
