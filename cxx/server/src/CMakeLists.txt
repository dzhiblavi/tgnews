set(NET_SRC
        net/address.cpp
        net/address.h
        net/io_api.cpp
        net/io_api.h
        net/ipv4_error.h
        net/poll.cpp
        net/poll.h
        net/socket.cpp
        net/socket.h
        net/timer.cpp
        net/timer.h
        net/unique_fd.h
        net/storage.h
        thread_pool.h)

set(SERV_SRC
        server.cpp
        server.h)

set(HTTP_SRC
        http/request.h
        http/parser.h)

include_directories(../../sysapi)
include_directories(../../json)

add_executable(tgserver
        main.cpp
        ${NET_SRC}
        ${SERV_SRC}
        ${HTTP_SRC})

if (UNIX)
    target_link_libraries(tgserver pthread)

