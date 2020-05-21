add_executable(http_test
        test_http.cpp
        ${HTTP_SRC}
        ${GTEST_SRC})

if (UNIX AND NOT APPLE)
    target_link_libraries(http_test pthread)
endif (UNIX AND NOT APPLE)
