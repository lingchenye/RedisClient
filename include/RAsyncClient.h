#ifndef RAsyncClient_h__
#define RAsyncClient_h__

#include <string>

#include <boost/function.hpp>
#include <boost/asio/io_service.hpp>

#include "RVariant.h"

#include "RedisClient_global.h"

class REDISCLIENT_GLOBAL_DECL RAsyncClient
{
public:
	RAsyncClient(boost::asio::io_service & io_src);
	~RAsyncClient(void);

	typedef boost::function<void (bool, const std::string &)> RConnectCallbck;
	typedef boost::function<void (const RVariant &)> RCommandCallbck;
	typedef boost::function<void (const RVariant &)> RPublishCallback;
	typedef boost::function<void (const RVariant &)> RSubscribeCallback;

	void connect(
		const std::string & ip, 
		const unsigned int & port, 
		RConnectCallbck cb = [](bool, const std::string &){});

	void command(
		const std::string & cmd, 
		RCommandCallbck cb);

	void command(
		const std::string & cmd, 
		const RVariant & param_1, 
		RCommandCallbck cb);

	void command(
		const std::string & cmd, 
		const RVariant & param_1, 
		const RVariant & param_2, 
		RCommandCallbck cb);

	void subscribe(
		const std::string & channel, 
		RSubscribeCallback cb);

	void publish(
		const std::string & channel, 
		const RVariant & var, 
		RPublishCallback cb);

private:
	struct Impl;
	Impl * m_pImpl;
};

#endif // RAsyncClient_h__
