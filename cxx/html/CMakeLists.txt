set(GTEST_SRC
        ../../gtest/gtest-all.cc
        ../../gtest/gtest.h
        ../../gtest/gtest_main.cc)

set(HTML_SRC parser.h)

include_directories(../gtest)

add_executable(html_test
        test_html.cpp
        ${HTML_SRC}
        ${GTEST_SRC})

if (UNIX AND NOT APPLE)
    target_link_libraries(html_test pthread)
endif (UNIX AND NOT APPLE)