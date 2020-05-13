set(DETECT_SRC
        lang_detect/code_sequence.cpp
        lang_detect/code_sequence.h
        lang_detect/const.cpp
        lang_detect/langdetect.cpp
        lang_detect/langdetect.h
        lang_detect/langdetect_c.h
        lang_detect/ngram_storage.cpp
        lang_detect/ngram_storage.h
        lang_detect/normalizer.cpp
        lang_detect/normalizer.h
        lang_detect/unicode_data.cpp
        lang_detect/unicode_data.h)

set(JSON_SRC
        ../../json/json.cpp
        ../../json/json.h)

set(WALKER_SRC
        ../../walker/src/walker.cpp
        ../../walker/src/walker.h)

set(SYSAPI_SRC
        ../../sysapi/sysapi.cpp
        ../../sysapi/sysapi.h
        ../../sysapi/thread_pool.h)

include_directories(../../json)
include_directories(../../walker/src)
include_directories(../../sysapi)

add_executable(language
        language.cpp
        ${DETECT_SRC}
        ${WALKER_SRC}
        ${JSON_SRC}
        ${SYSAPI_SRC})

if (UNIX AND NOT APPLE)
    target_link_libraries(language stdc++fs pthread)
endif (UNIX AND NOT APPLE)