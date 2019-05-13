#ifndef RedisClient_global_h__
#define RedisClient_global_h__

#ifdef WIN32
#	ifdef REDISCLIENT_GLOBAL_EXPORT
#		define REDISCLIENT_GLOBAL_DECL __declspec(dllexport)
#	else
#		define REDISCLIENT_GLOBAL_DECL __declspec(dllimport)
#	endif
#else
#define REDISCLIENT_GLOBAL_DECL
#endif

#endif // RedisClient_global_h__
