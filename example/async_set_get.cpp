// RedisClient_Unittest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>

#include <boost/regex.hpp>
#include <boost/asio/io_service.hpp>

#include "RSyncClient.h"
#include "RAsyncClient.h"

int main()
{
	// create IO service
	boost::asio::io_service io_src;
	boost::asio::io_service::work worker(io_src);

	// create client
	RAsyncClient client(io_src);
	
	// connect with local REDIS server
	client.connect("127.0.0.1", 6379, [&](bool state, const std::string & err_msg)
	{
		if (state)
		{
			client.command("set", RVariant("name"), RVariant("lingchenye"), [&](RVariant var)
			{
				std::cout << "set name with lingchenye successful!" << std::endl;
				client.command("get", RVariant("name"), [](RVariant var)
				{
					std::cout << "get the value of name is : " << var.toString() << std::endl;
				});
			});
		}
		else
		{
			std::cout << "connection failed, error code is " << err_msg << std::endl;
		}
	});

	io_src.run();

	return 0;
}

