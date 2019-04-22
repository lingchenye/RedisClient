#ifndef RSyncClient_h__
#define RSyncClient_h__

#include <string>

#include "RVariant.h"

#include "RedisClient_global.h"

class REDISCLIENT_GLOBAL_DECL RSyncClient
{
public:
	RSyncClient(boost::asio::io_service & io_src);
	~RSyncClient(void);

	bool connect(
		const std::string & ip, 
		const unsigned int & port);

	RVariant command(
		const std::string & cmd);

	RVariant command(
		const std::string & cmd, 
		const RVariant & param_1);

	RVariant command(
		const std::string & cmd, 
		const RVariant & param_1, 
		const RVariant & param_2);

private:
	struct Impl;
	Impl * m_pImpl;
};

#endif // RSyncClient_h__
