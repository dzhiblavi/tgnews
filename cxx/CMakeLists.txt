cmake_minimum_required(VERSION 3.14)
project(tgserver)

set(CMAKE_CXX_STANDARD 17)

set(GTEST_SRC
        ${PROJECT_SOURCE_DIR}/gtest/gtest-all.cc
        ${PROJECT_SOURCE_DIR}/gtest/gtest.h
        ${PROJECT_SOURCE_DIR}/gtest/gtest_main.cc)

set(HTML_SRC
        ${PROJECT_SOURCE_DIR}/html/parser.h)

set(HTTP_SRC
        ${PROJECT_SOURCE_DIR}/http/parser.h
        ${PROJECT_SOURCE_DIR}/http/parser.cpp
        ${PROJECT_SOURCE_DIR}/http/request.h
        ${PROJECT_SOURCE_DIR}/http/request.cpp)

set(JSON_SRC
        ${PROJECT_SOURCE_DIR}/json/json.h)

set(LANGDETECT_SRC
        ${PROJECT_SOURCE_DIR}/language/src/lang_detect/code_sequence.cpp
        ${PROJECT_SOURCE_DIR}/language/src/lang_detect/code_sequence.h
        ${PROJECT_SOURCE_DIR}/language/src/lang_detect/const.cpp
        ${PROJECT_SOURCE_DIR}/language/src/lang_detect/langdetect.cpp
        ${PROJECT_SOURCE_DIR}/language/src/lang_detect/langdetect.h
        ${PROJECT_SOURCE_DIR}/language/src/lang_detect/langdetect_c.h
        ${PROJECT_SOURCE_DIR}/language/src/lang_detect/ngram_storage.cpp
        ${PROJECT_SOURCE_DIR}/language/src/lang_detect/ngram_storage.h
        ${PROJECT_SOURCE_DIR}/language/src/lang_detect/normalizer.cpp
        ${PROJECT_SOURCE_DIR}/language/src/lang_detect/normalizer.h
        ${PROJECT_SOURCE_DIR}/language/src/lang_detect/unicode_data.cpp
        ${PROJECT_SOURCE_DIR}/language/src/lang_detect/unicode_data.h
        ${PROJECT_SOURCE_DIR}/language/src/detect.cpp
        ${PROJECT_SOURCE_DIR}/language/src/detect.h)

set(DAEMON_SRC
        ${PROJECT_SOURCE_DIR}/name_daemon/name_daemon.h
        ${PROJECT_SOURCE_DIR}/name_daemon/name_daemon.cpp)

set(NET_SRC
        ${PROJECT_SOURCE_DIR}/net/address.cpp
        ${PROJECT_SOURCE_DIR}/net/address.h
        ${PROJECT_SOURCE_DIR}/net/io_api.cpp
        ${PROJECT_SOURCE_DIR}/net/io_api.h
        ${PROJECT_SOURCE_DIR}/net/ipv4_error.h
        ${PROJECT_SOURCE_DIR}/net/poll.cpp
        ${PROJECT_SOURCE_DIR}/net/poll.h
        ${PROJECT_SOURCE_DIR}/net/socket.cpp
        ${PROJECT_SOURCE_DIR}/net/socket.h
        ${PROJECT_SOURCE_DIR}/net/timer.cpp
        ${PROJECT_SOURCE_DIR}/net/timer.h
        ${PROJECT_SOURCE_DIR}/net/unique_fd.h)

set(SYSAPI_SRC
        ${PROJECT_SOURCE_DIR}/sysapi/sysapi.cpp
        ${PROJECT_SOURCE_DIR}/sysapi/sysapi.h
        ${PROJECT_SOURCE_DIR}/sysapi/thread_pool.h)

set(WALKER_SRC
        ${PROJECT_SOURCE_DIR}/walker/src/walker.h
        ${PROJECT_SOURCE_DIR}/walker/src/walker.cpp)

include_directories(.)

add_subdirectory(language)
add_subdirectory(server)
add_subdirectory(walker)
add_subdirectory(http)
add_subdirectory(html)
add_subdirectory(tgnews)
