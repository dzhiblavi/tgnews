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

class map : object {
    std::map<std::string, obj_ptr> mp;

public:
    map(std::map<std::string, obj_ptr>&& mp);

    std::string to_string(int tabs) const override;

    static obj_ptr create(std::map<std::string, obj_ptr>&& mp);
};

class array : object {
    std::vector<obj_ptr> v;

public:
    array(std::vector<obj_ptr>&& v);

    std::string to_string(int tabs) const override;

    static obj_ptr create(std::vector<obj_ptr>&& v);
};

class number : object {
    int value;

public:
    number(int value);

    std::string to_string(int tabs) const override;

    static obj_ptr create(int value);
};

class string : object {
    std::string value;

public:
    string(std::string&& value);

    string(std::string const& value);

    std::string to_string(int tabs) const override;

    static obj_ptr create(std::string&& value);

    static obj_ptr create(std::string const& value);
};

class literal : object {
public:
    enum VALUE {
        NUL,
        TRUE,
        FALSE,
    };

private:
    VALUE value;

public:
    literal(VALUE value);

    std::string to_string(int tabs) const override;

    static obj_ptr create(VALUE value);
};
} // namespace json


#endif //TGSERVER_JSON_H
