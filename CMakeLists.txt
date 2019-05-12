cmake_minimum_required(VERSION 2.8)
project (RedisClient)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-std=c++11)

find_package(
    Boost
    COMPONENTS
        chrono
        coroutine
        filesystem
        system
        thread
    REQUIRED
)

include_directories(${RedisClient_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

aux_source_directory(./src SRC_LIST)
add_library(RedisClient SHARED ${SRC_LIST})

#add_executable(sync_set_get ./example/sync_set_get.cpp)
#target_link_libraries(sync_set_get RedisClient)

add_executable(async_set_get ./example/async_set_get.cpp)
target_link_libraries(async_set_get RedisClient boost_thread boost_system pthread)

#add_executable(publish_subscribe ./example/publish_subscribe.cpp)
#target_link_libraries(publish_subscribe RedisClient)



