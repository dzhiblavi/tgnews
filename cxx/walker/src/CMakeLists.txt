include_directories(../../sysapi/)

add_executable(list_files list_files.cpp walker.h walker.cpp)

if (UNIX AND NOT APPLE)
    target_link_libraries(list_files stdc++fs pthread)
endif (UNIX AND NOT APPLE)