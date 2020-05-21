set(SERV_SRC
        PyServer.h
        PyServer.cpp
        server.cpp
        server.h)

add_executable(tgserver
        main.cpp
        ${NET_SRC}
        ${SERV_SRC}
        ${HTTP_SRC}
        ${JSON_SRC}
        ${DAEMON_SRC}
        ${WALKER_SRC}
        ${SYSAPI_SRC}
        ${HTML_SRC}
        ${LANGDETECT_SRC})


if (UNIX AND NOT APPLE)
    target_link_libraries(tgserver stdc++fs pthread)
endif (UNIX AND NOT APPLE)