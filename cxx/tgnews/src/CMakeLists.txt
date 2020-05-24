add_executable(tgnews
        tgnews.cpp
        ${LANGDETECT_SRC}
        ${WALKER_SRC}
        ${JSON_SRC}
        ${SYSAPI_SRC}
        ${HTML_SRC})


if (UNIX AND NOT APPLE)
    target_link_libraries(tgnews stdc++fs pthread)
endif (UNIX AND NOT APPLE)