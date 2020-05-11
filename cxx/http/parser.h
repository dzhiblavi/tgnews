#ifndef TGSERVER_PARSER_H
#define TGSERVER_PARSER_H


namespace http {
template <bool isRequest>
class parser {
public:
    static request<isRequest> parse(std::istream& is) {
        return {};
    }
};
} // namespace http


#endif //TGSERVER_PARSER_H
