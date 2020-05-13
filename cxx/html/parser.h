#ifndef TGSERVER_PARSER_H
#define TGSERVER_PARSER_H

#include <string>
#include <unordered_map>

namespace html {
    static const std::string META_PREFIX = "<meta property=";

    class parser {
    private:
        using siter = std::string::iterator;

    private:
        static bool starts_with(siter it, siter end, std::string const &prefix) {
            size_t p = 0;
            while (p < prefix.size() && it < end && *it == prefix[p]) {
                p++;
                it++;
            }
            return p == prefix.size();
        }

        static siter skip_until(siter start, siter end, char c) {
            while (start != end && *start != c) {
                start++;
            }
            if (start != end) start++;
            return start;
        }

        static bool is_spec(char c) {
            return c == '\n' || c == '\r' || c == '\0' || c == '\t' || c == ' ';
        }

        static bool append(siter begin, siter left, siter right) {
            bool prev_spec = (left == begin || is_spec(*(left - 1)));
            bool cur_spec = is_spec(*right);

            return (!cur_spec || !prev_spec);
        }

    public:
        static void extract(std::string &s, std::unordered_map<std::string, std::string> &meta) {
            const siter beg = s.begin();
            siter left = beg;
            siter right = beg;
            const siter end = s.end();

            while (right != s.end()) {
                if (*right == '<') {
                    if (starts_with(right, end, META_PREFIX)) {
                        std::string field, content;
                        right = skip_until(right, end, '\"');
                        while (right != end && *right != '\"') {
                            field += *right;
                            right++;
                        }
                        if (right != end) right++;
                        right = skip_until(right, end, '\"');
                        while (right != s.end() && *right != '\"') {
                            content += *right;
                            right++;
                        }
                        meta[field] = content;
                        right = skip_until(right, end, '>');
                    } else {
                        right = skip_until(right, end, '>');
                    }
                } else {
                    if (append(beg, left, right)) {
                        *left = *right;
                        left++;
                    }
                    right++;
                }
            }
            s.resize(left - beg);
        }

        static void extract(std::string &s) {
            siter beg = s.begin();
            siter left = beg;
            siter right = beg;
            siter end = s.end();

            while (right != end) {
                if (*right == '<') {
                    right = skip_until(right, end, '>');
                } else {
                    if (append(beg, left, right)) {
                        *left = *right;
                        left++;
                    }
                    right++;
                }
            }
            s.resize(left - beg);
        }
    };

}

#endif //TGSERVER_PARSER_H
