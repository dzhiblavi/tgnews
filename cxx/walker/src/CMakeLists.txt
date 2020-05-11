include_directories(../../sysapi/)

add_executable(walker main.cpp walker.h walker.cpp)

if (UNIX)
    target_link_libraries(walker stdc++fs pthread)
endif (UNIX)