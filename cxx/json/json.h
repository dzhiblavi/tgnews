#ifndef TGSERVER_JSON_H
#define TGSERVER_JSON_H

#include <string>
#include <map>
#include <vector>
#include <sstream>


namespace json {
class object {
public:
    virtual ~object() {}
    virtual std::string to_string(int tabs = 0) const = 0;
};

class map : object {
    std::map<std::string, object*> mp;

public:
    map(std::map<std::string, object*> const& mp);

    std::string to_string(int tabs) const override;
};

class array : object {
    std::vector<object*> v;

public:
    array(std::vector<object*> const& v);

    std::string to_string(int tabs) const override;
};

class number : object {
    int value;

public:
    number(int value);

    std::string to_string(int tabs) const override;
};

class string : object {
    std::string value;

public:
    string(std::string const& value);

    std::string to_string(int tabs) const override;
};

class literal : object {
    enum VALUE {
        NUL,
        TRUE,
        FALSE,
    } value;

public:
    literal(VALUE value);

    std::string to_string(int tabs) const override;
};
} // namespace json


#endif //TGSERVER_JSON_H
