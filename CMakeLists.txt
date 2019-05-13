cmake_minimum_required(VERSION 2.8)
project (RedisClient)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_DEBUG_POSTFIX "d")

add_compile_options(-std=c++11)

find_package(
    Boost
    COMPONENTS
        system
        thread
    REQUIRED
)

message(STATUS "CMAKE_BINARY_DIR" ${CMAKE_BINARY_DIR})

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

include_directories(${RedisClient_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS} ${LIBRARY_OUTPUT_PATH})

include_directories(${RedisClient_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS} ${LIBRARY_OUTPUT_PATH})

add_subdirectory(./src)
add_subdirectory(./example)



