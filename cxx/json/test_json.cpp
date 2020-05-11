#include "gtest.h"

#include "json.h"

using namespace json;

obj_ptr gen_array(int, int);

obj_ptr gen_map(int depth, int size) {
    std::map<std::string, obj_ptr> v;

    for (size_t i = 0; i < size; ++i) {
        std::string key = "key" + std::to_string(i);
        if (i % 2) {
            if (!depth)
                v.emplace(key, number::create(i));
            else
                v.emplace(key, gen_array(depth - 1, size));
        } else {
            if (!depth)
                v.emplace(key, string::create("hello, " + std::to_string(i)));
            else
                v.emplace(key, gen_map(depth - 1, size));
        }
    }

    return map::create(std::move(v));
}

obj_ptr gen_array(int depth, int size) {
    std::vector<obj_ptr> v;

    for (size_t i = 0; i < size; ++i) {
        if (i % 2) {
            if (!depth)
                v.push_back(number::create(i));
            else
                v.push_back(gen_array(depth - 1, size));
        } else {
            if (!depth)
                v.push_back(string::create("hello, " + std::to_string(i)));
            else
                v.push_back(gen_map(depth - 1, size));
        }
    }

    return array::create(std::move(v));
}

TEST(json, number) {
    obj_ptr num = number::create(1000234);
    ASSERT_EQ(num->to_string(), "1000234");
}

TEST(json, string) {
    obj_ptr str = string::create("Hello, json world!");
    ASSERT_EQ(str->to_string(), "\"Hello, json world!\"");
    str = string::create("");
    ASSERT_EQ(str->to_string(), "\"\"");
}

TEST(json, literal) {
    obj_ptr lit = literal::create(literal::NUL);
    ASSERT_EQ(lit->to_string(), "null");
    lit = literal::create(literal::TRUE);
    ASSERT_EQ(lit->to_string(), "true");
    lit = literal::create(literal::FALSE);
    ASSERT_EQ(lit->to_string(), "false");
}

TEST(json, array0) {
    std::vector<obj_ptr> v;

    obj_ptr ar = array::create(std::move(v));
    ASSERT_EQ(ar->to_string(), "[]");

    v.clear();
    v.push_back(number::create(124));
    ar = array::create(std::move(v));
    ASSERT_EQ(ar->to_string(), "[\r\n  124\r\n]");

    v.clear();
    v.push_back(number::create(124));
    v.push_back(string::create("helelelel"));
    ar = array::create(std::move(v));
    ASSERT_EQ(ar->to_string(), "[\r\n  124,\r\n  \"helelelel\"\r\n]");
}

TEST(json, array1) {
    obj_ptr ar = gen_array(0, 5);
    std::cout << ar->to_string() << std::endl;
}

TEST(json, array2) {
    obj_ptr ar = gen_array(1, 2);
    std::cout << ar->to_string() << std::endl;
}

TEST(json, map1) {
    obj_ptr mp = gen_map(0, 5);
    std::cout << mp->to_string() << std::endl;
}

TEST(json, map2) {
    obj_ptr mp = gen_map(1, 2);
    std::cout << mp->to_string() << std::endl;
}

