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
	RSyncClient client(io_src);

	// connect with local REDIS server
	client.connect("127.0.0.1", 6379);

	// check the connection whit server
	RVariant ret = client.command("ping");
	std::cout << "the result of ping is " << ret.toString() << std::endl;
	
	// set name with lingchenye
	ret = client.command("set", RVariant("name"), RVariant("lingchenye"));
	std::cout << "the result of set name is " << ret.toString() << std::endl;
	
	// get the value which key is name
	ret = client.command("get", RVariant("name"));
	std::cout << "the result of get name is " << ret.toString() << std::endl;
	
	io_src.run();

	return 0;
}

