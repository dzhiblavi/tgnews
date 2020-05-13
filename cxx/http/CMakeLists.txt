set(GTEST_SRC
        ../../gtest/gtest-all.cc
        ../../gtest/gtest.h
        ../../gtest/gtest_main.cc)

set(HTTP_SRC
        parser.h
        request.h)

include_directories(../gtest)

add_executable(http_test
        test_http.cpp
        ${HTTP_SRC}
        ${GTEST_SRC})

if (UNIX AND NOT APPLE)
    target_link_libraries(http_test pthread)
endif (UNIX AND NOT APPLE)
