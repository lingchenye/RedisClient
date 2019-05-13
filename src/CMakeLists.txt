add_definitions(-DREDISCLIENT_GLOBAL_EXPORT)
aux_source_directory(./ SRC_LIST)
add_library(RedisClient SHARED ${SRC_LIST})