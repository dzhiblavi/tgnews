add_executable(list_files
        list_files.cpp
        ${WALKER_SRC}
        ${SYSAPI_SRC})

if (UNIX AND NOT APPLE)
    target_link_libraries(list_files stdc++fs pthread)
endif (UNIX AND NOT APPLE)