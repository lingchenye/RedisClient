// RedisClient_Unittest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>

#include <boost/regex.hpp>
#include <boost/asio/io_service.hpp>

#include "../Redisclient/RSyncClient.h"
#include "../RedisClient/RAsyncClient.h"

int main()
{
	// create IO service
	boost::asio::io_service io_src;
	boost::asio::io_service::work worker(io_src);

	// create client: subscriber and publisher have to use different client
	RAsyncClient sclient(io_src);
	RAsyncClient pclient(io_src);

	// subscriber connect server
	sclient.connect("127.0.0.1", 6379, [&](bool state, const std::string & err_msg)
	{
		if (state)
		{
			sclient.subscribe("tchannel", [](RVariant var)
			{
				std::cout << "subscriber: received value from tchannel: " << var << std::endl;
			});
		}
		else
		{
			std::cout << "publisher :connection failed: " << err_msg << std::endl;
		}
	});

	
	pclient.connect("127.0.0.1", 6379, [&](bool state, const std::string & err_msg)
	{
		if (state)
		{
			pclient.publish("tchannel", RVariant("hello redis"), [](RVariant var)
			{
				std::cout << "publisher :publish " << (var.toInt() ? "successed" : "failed") << std::endl;
			});
		}
		else
		{
			std::cout << "publisher: connection failed: " << err_msg << std::endl;
		}
	});
	
	io_src.run();

	return 0;
}

