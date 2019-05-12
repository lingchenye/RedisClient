// RedisClient_Unittest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>

#include <boost/regex.hpp>
#include <boost/asio/io_service.hpp>

#include "../Redisclient/RSyncClient.h"
#include "../RedisClient/RAsyncClient.h"

int main()
{	boost::asio::io_service io_src;
	boost::asio::io_service::work worker(io_src);

	{
// 		RAsyncClient client(io_src);
// 
// 		client.connect("127.0.0.1", 6379, [&](bool state, const std::string & err_msg)
// 		{
// 			if (state)
// 			{
// 				std::cout << "connection succeeded" << std::endl;
// 
// 				client.command("set", RVariant("name"), RVariant("zfs"), [](RVariant var)
// 				{
// 					std::cout << var.toString() << std::endl;
// 				});
// 			}
// 			else
// 			{
// 				std::cout << "connection failed because " << err_msg << std::endl;
// 			}
// 		});
	}

	{
		RAsyncClient sclient(io_src);
		RAsyncClient pclient(io_src);

		sclient.connect("127.0.0.1", 6379, [&](bool state, const std::string & err_msg)
		{
			if (state)
			{
				std::cout << "connection succeeded" << std::endl;

				sclient.subscribe("tchannel", [](RVariant var)
				{
					std::cout << var << std::endl;
				});

				pclient.connect("127.0.0.1", 6379, [&](bool state, const std::string & err_msg)
				{
					if (state)
					{
						std::cout << "connection succeeded" << std::endl;

						//pclient.command("publish", RVariant("tchannel"), RVariant("fsfds"), [](RVariant var)
						pclient.publish("tchannel", RVariant("fsfds"), [](RVariant var)
						{
							std::cout << "channel published success : " << var << std::endl;
						});
					}
				});
				io_src.run();
			}
			else
			{
				std::cout << "connection failed because " << err_msg << std::endl;
			}
		});
	}


	{
// 		RSyncClient client(io_src);
// 
// 		RVariant var;
// 		RVariant::parseFromString("*2\r\n$4\r\nLLEN\r\n$6\r\nmylist\r\n", var);
// 
// 		client.connect("127.0.0.1", 6379);
// 
// 		RVariant ret = client.command("ping");
// 		std::cout << "the result of ping is " << ret.toString() << std::endl;
// 		ret = client.command("set", RVariant("name"), RVariant("lcy"));
// 		std::cout << "the result of set name is " << ret.toString() << std::endl;
// 		ret = client.command("get", RVariant("name"));
// 		std::cout << "the result of get name is " << ret.toString() << std::endl;
	}

	io_src.run();

	return 0;
}

