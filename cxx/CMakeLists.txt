cmake_minimum_required(VERSION 3.14)
project(tgserver)

set(CMAKE_CXX_STANDARD 17)

# TESTS
add_subdirectory(json)
add_subdirectory(http)

# EXECS
add_subdirectory(language)
add_subdirectory(server)
add_subdirectory(walker)

add_subdirectory(json)
add_subdirectory(http)
add_subdirectory(html)

add_subdirectory(tgnews)
