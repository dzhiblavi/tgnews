#ifndef TGSERVER_JSON_H
#define TGSERVER_JSON_H

#include <string>
#include <map>
#include <memory>
#include <vector>
#include <sstream>


#define JSON_TAB "  "
#define JSON_ENDL "\r\n"


namespace json {
class object {
public:
    virtual ~object() {}
    virtual std::string to_string(int tabs = 0) const = 0;
    static std::string gen_tabs(int tabs);
};

class map;
class array;
class number;
class string;
class literal;

typedef std::unique_ptr<object> obj_ptr;

struct map : object {
    std::map<std::string, obj_ptr> mp;

public:
    explicit map(std::map<std::string, obj_ptr>&& mp);

    [[nodiscard]] std::string to_string(int tabs) const override;

    static obj_ptr create(std::map<std::string, obj_ptr>&& mp);
};

struct array : object {
    std::vector<obj_ptr> v;

public:
    explicit array(std::vector<obj_ptr>&& v);

    [[nodiscard]] std::string to_string(int tabs) const override;

    static obj_ptr create(std::vector<obj_ptr>&& v);
};

struct number : object {
    int value;

public:
    explicit number(int value);

    [[nodiscard]] std::string to_string(int tabs) const override;

    static obj_ptr create(int value);
};

struct string : object {
    std::string value;

public:
    explicit string(std::string&& value);

    explicit string(std::string const& value);

    [[nodiscard]] std::string to_string(int tabs) const override;

    static obj_ptr create(std::string&& value);

    static obj_ptr create(std::string const& value);
};

struct literal : object {
    enum VALUE {
        NUL,
        TRUE,
        FALSE,
    } value;

public:
    explicit literal(VALUE value);

    [[nodiscard]] std::string to_string(int tabs) const override;

    static obj_ptr create(VALUE value);
};
} // namespace json


#endif //TGSERVER_JSON_H
