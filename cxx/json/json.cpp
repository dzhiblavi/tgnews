#include "json.h"

namespace json {
map::map(std::map<std::string, object *> const& mp)
    : mp(mp) {}

std::string map::to_string(int tabs) const {

}


array::array(std::vector<object *> const& v)
    : v(v) {}

std::string array::to_string(int tabs) const {

}


number::number(int value)
    : value(value) {}

std::string number::to_string(int tabs) const {

}


string::string(std::string const& value)
    : value(value) {}

std::string string::to_string(int tabs) const {

}


literal::literal(VALUE value)
    : value(value) {}

std::string literal::to_string(int tabs) const {

}
}