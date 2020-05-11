#ifndef TGSERVER_PARSER_H
#define TGSERVER_PARSER_H

#include "request.h"


namespace http {
template <bool isRequest>
class parser;

template <>
class parser<true> {
    request<true> req;
    std::string line;
    int content_size = -1;
    int content_started = 0;

private:
    static bool equals(char const* buff, size_t start, size_t end, std::string const& p) {
        if (p.size() > end - start)
            return false;
        size_t i = 0;
        while (i < p.size()) {
            if (buff[start + i] != p[i])
                return false;
            ++i;
        }
        return true;
    }

    static bool equals(std::string const& s, size_t start, std::string const& p) {
        return equals(s.data(), start, s.size(), p);
    }

    static bool in(char c, std::string const& s) {
        for (auto ch : s)
            if (c == ch)
                return true;
        return false;
    }

    static int get_int_after(char c, std::string const& s, size_t i) {
        while (i < s.size() && s[i] != c)
            ++i;
        int st = ++i;
        assert(i < s.size());
        while (i < s.size() && std::isdigit(s[i]))
            ++i;
        return atoi(std::string(s.data() + st, s.data() + i).c_str());
    }

    static std::string get_str_after(char c, std::string const& s, size_t i) {
        while (i < s.size() && s[i] != c)
            ++i;
        int st = ++i;
        assert(i < s.size());
        while (i < s.size() && !in(s[i], " \t\r\n"))
            ++i;
        return std::string(s.data() + st, s.data() + i).c_str();
    }

    void parse_line() {
        size_t i = 0;
        while (i < line.size() && in(line[i], " \t\r\n"))
            ++i;

        if (equals(line, i, "PUT")) {
            req.method = "PUT";
            req.path = get_str_after(' ', line, i);
        } else if (equals(line, i, "DELETE")) {
            req.method = "DELETE";
            req.path = get_str_after(' ', line, i);
            content_size = 0;
        } else if (equals(line, i, "GET")) {
            req.method = "GET";
            req.path = get_str_after(' ', line, i);
            content_size = 0;
        } else if (equals(line, i, "Content-T")) {
        } else if (equals(line, i, "Content-L")) {
            req.content_length = get_int_after(' ', line, i);
            content_size = req.content_length;
            content_started = 1;
        } else if (equals(line, i, "Cache")) {
            req.cache_keepalive = get_int_after('=', line, i);
        } else {
            assert(line == "\r\n");
            if (content_started == 1) {
                content_started = 2;
            } else {
                req.content += "\r\n";
            }
        }

        line.clear();
    }

    int append_content(char const* buff, int offset, int size) {
        if (content_size == 0)
            return 0;
        int get = std::min(content_size, size);
        req.content += std::string(buff + offset, buff + offset + get);
        content_size -= get;
        return get;
    }

    int check_conn(char const* buff, int idx, int end, std::string const& p) {
        int ans = 1;
        while (ans < p.size() && line.size() >= ans) {
            if (line.substr(line.size() - ans, ans) + std::string(buff + idx, buff + idx + p.size() - ans) == p)
                return ans;
            ++ans;
        }
        return -1;
    }

public:
    [[nodiscard]] bool ready() const noexcept {
        return content_size == 0;
    }

    [[nodiscard]] request<true> get_request() {
        content_size = -1;
        content_started = 0;
        return std::move(req);
    }

    int append(char const* buff, int offset, int size) {
        if (content_started == 2)
            return append_content(buff, offset, size);

        int end = offset + size;
        int idx = offset;
        int begin = offset;

        while (idx < end && content_started != 2) {
            int m = 2;
            int k = check_conn(buff, idx, end, "\r\n");
            if (k == -1) {
                m = 8;
                k = check_conn(buff, idx, end, "HTTP/1.1");
            }
            if (k != -1) {
                assert(k >= 1);
                line += std::string(buff, buff + k);
                idx += m - k;
                offset += m - k;
                parse_line();
                continue;
            }

            if (idx + 1 == end) {
                line += buff[idx];
                return idx - begin;
            }

            while (idx + 1 < end && content_started != 2) {
                if (equals(buff, idx, end, "\r\n")) {
                    idx += 2;
                    line += std::string(buff + offset, buff + idx);
                    offset = idx;
                    parse_line();
                } else if (idx + 7 < end && equals(buff, idx, end, "HTTP/1.1")) {
                    idx += 8;
                    line += std::string(buff + offset, buff + idx);
                    offset = idx;
                    parse_line();
                } else {
                    ++idx;
                }
            }

            if (content_started != 2) {
                if (idx < end) ++idx;
                line += std::string(buff + offset, buff + idx);
                return idx - begin;
            }
        }

        if (idx < end && content_started == 2)
            return idx - begin + append_content(buff, idx, end - idx);

        return idx - begin;
    }
};
} // namespace http


#endif //TGSERVER_PARSER_H
