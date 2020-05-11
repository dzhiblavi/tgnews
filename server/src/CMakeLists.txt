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
        net/unique_fd.h)

add_executable(tgserver
        main.cpp
        ${NET_SRC})
