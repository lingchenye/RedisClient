if(WIN32)
	set(LINK_DEPENDENCIES RedisClient)
else()
	set(LINK_DEPENDENCIES RedisClient boost_thread boost_system pthread)
endif()

include_directories(${RedisClient_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS} ${LIBRARY_OUTPUT_PATH})

add_executable(sync_set_get ./sync_set_get.cpp)
target_link_libraries(sync_set_get ${LINK_DEPENDENCIES})

add_executable(async_set_get ./async_set_get.cpp)
target_link_libraries(async_set_get ${LINK_DEPENDENCIES})

add_executable(publish_subscribe ./publish_subscribe.cpp)
target_link_libraries(publish_subscribe ${LINK_DEPENDENCIES})