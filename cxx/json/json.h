#ifndef TGSERVER_JSON_H
#define TGSERVER_JSON_H


namespace json {
class object {
public:
    virtual ~object() {}
    virtual std::string to_string() const;
};

class map : object {
    std::map<std::string, object*> mp;

public:
};

class array : object {
    std::vector<object*> v;

public:
};

class number : object {

};

class string : object {

};

class literal : object {

};

} // namespace json


#endif //TGSERVER_JSON_H
