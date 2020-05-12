#include "json.h"

namespace json {
std::string object::gen_tabs(int tabs) {
    std::string r;
    while (tabs--) {
        r += JSON_TAB;
    }
    return r;
}


map::map(std::map<std::string, obj_ptr>&& mp)
    : mp(std::move(mp)) {}

std::string map::to_string(int tabs) const {
    std::stringstream ss;
    std::string tab = gen_tabs(tabs);

    ss << "{" << JSON_ENDL;
    size_t i = 0;
    for (auto const& p : mp) {
        ss << tab << JSON_TAB << "\"" << p.first << "\": " << p.second->to_string(tabs + 1);

        if (i < mp.size() - 1)
            ss << ",";
        ss << JSON_ENDL;
        ++i;
    }
    ss << tab << "}";

    return ss.str();
}

obj_ptr map::create(std::map<std::string, obj_ptr> &&mp) {
    return obj_ptr(new map(std::move(mp)));
}


array::array(std::vector<obj_ptr>&& v)
    : v(std::move(v)) {}

std::string array::to_string(int tabs) const {
    if (v.empty()) {
        return "[]";
    }

    std::stringstream ss;
    std::string tab = gen_tabs(tabs);

    ss << "[" << JSON_ENDL;
    for (size_t i = 0; i < v.size(); ++i) {
        ss << tab + JSON_TAB << v[i]->to_string(tabs + 1);

        if (i < v.size() - 1)
            ss << ",";
        ss << JSON_ENDL;
    }
    ss << tab << "]";

    return ss.str();
}

obj_ptr array::create(std::vector<obj_ptr> &&v) {
    return obj_ptr(new array(std::move(v)));
}


number::number(int value)
    : value(value) {}

std::string number::to_string(int tabs) const {
    return std::to_string(value);
}

obj_ptr number::create(int value) {
    return obj_ptr(new number(value));
}


string::string(std::string&& value)
    : value(std::move(value)) {}

string::string(std::string const& value)
    : value(value) {}

std::string string::to_string(int tabs) const {
    return "\"" + value + "\"";
}

obj_ptr string::create(std::string&& value) {
    return obj_ptr(new string(std::move(value)));
}

obj_ptr string::create(std::string const& value) {
    return obj_ptr(new string(value));
}


literal::literal(VALUE value)
    : value(value) {}

std::string literal::to_string(int tabs) const {
    std::string tab = gen_tabs(tabs);
    std::string val;
    switch (value) {
        case NUL:
            val = "null";
            break;
        case TRUE:
            val = "true";
            break;
        case FALSE:
            val = "false";
            break;
    }
    return tab + val;
}

obj_ptr literal::create(literal::VALUE value) {
    return obj_ptr(new literal(value));
}
}