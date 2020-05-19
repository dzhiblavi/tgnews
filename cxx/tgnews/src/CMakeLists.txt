SET(SYSAPI_SRC
        ../../sysapi/sysapi.cpp
        ../../sysapi/sysapi.h
        ../../sysapi/thread_pool.h)

include_directories(../../sysapi)

add_executable(tgnews
        tgnews.cpp
        ${SYSAPI_SRC})


if (UNIX AND NOT APPLE)
    target_link_libraries(tgnews stdc++fs)
endif (UNIX AND NOT APPLE)