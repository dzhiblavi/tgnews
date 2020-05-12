SET(SYSAPI_SRC
        ../../sysapi/sysapi.cpp
        ../../sysapi/sysapi.h
        ../../sysapi/thread_pool.h)

include_directories(../../sysapi)

add_executable(tgnews
        tgnews.cpp
        ${SYSAPI_SRC})