add_executable(html_test
        test_html.cpp
        ${HTML_SRC}
        ${GTEST_SRC})

if (UNIX AND NOT APPLE)
    target_link_libraries(html_test pthread)
endif (UNIX AND NOT APPLE)