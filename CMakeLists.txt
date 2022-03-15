cmake_minimum_required(VERSION 3.20)
project(IDATT2104___Project___Onion_router)

set(CMAKE_CXX_STANDARD 14)

add_executable(IDATT2104___Project___Onion_router main.cpp)

if(MINGW)
    target_link_libraries(IDATT2104___Project___Onion_router ws2_32)
endif()