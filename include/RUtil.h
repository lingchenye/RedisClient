#ifndef RUtil_h__
#define RUtil_h__

#include "RVariant.h"
#include "RedisClient_global.h"

#define _in
#define _out
#define _in_out

class REDISCLIENT_GLOBAL_DECL RUtil
{
public:
	static bool create_message(
		_in const std::string & cmd, 
		_in std::vector<RVariant> params, 
		_out std::string & message);
};

#endif // RUtil_h__
