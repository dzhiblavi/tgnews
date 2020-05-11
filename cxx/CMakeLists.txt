cmake_minimum_required(VERSION 3.14)
project(tgserver)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(language)
add_subdirectory(server)
add_subdirectory(walker)

add_subdirectory(json)
add_subdirectory(http)
