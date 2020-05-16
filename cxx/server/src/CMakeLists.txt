set(NET_SRC
        ../../net/address.cpp
        ../../net/address.h
        ../../net/io_api.cpp
        ../../net/io_api.h
        ../../net/ipv4_error.h
        ../../net/poll.cpp
        ../../net/poll.h
        ../../net/socket.cpp
        ../../net/socket.h
        ../../net/timer.cpp
        ../../net/timer.h
        ../../net/unique_fd.h
        ../../net/storage.h)

set(SERV_SRC
        server.cpp
        server.h)

set(HTTP_SRC
        ../../http/parser.h
        ../../http/request.h
        ../../http/parser.cpp
        ../../http/request.cpp)

set(JSON_SRC
        ../../json/json.cpp
        ../../json/json.h)

set(GTEST
        ../../gtest/gtest-all.cc
        ../../gtest/gtest.h
        ../../gtest/gtest_main.cc)

include_directories(../../sysapi)
include_directories(../../http)
include_directories(../../json)
include_directories(../../net)

add_executable(tgserver
        main.cpp
        ${NET_SRC}
        ${SERV_SRC}
        ${HTTP_SRC}
        ${JSON_SRC})

if (UNIX)
    target_link_libraries(tgserver pthread)
endif (UNIX)
