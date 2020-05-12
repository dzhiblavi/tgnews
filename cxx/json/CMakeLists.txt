set(GTEST_SRC
        ../../gtest/gtest-all.cc
        ../../gtest/gtest.h
        ../../gtest/gtest_main.cc)

set(JSON_SRC
        json.cpp
        json.h)

include_directories(../gtest)

add_executable(json_test
        test_json.cpp
        ${JSON_SRC}
        ${GTEST_SRC})
