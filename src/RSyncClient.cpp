#include "StdAfx.h"

#include <boost/asio/ip/tcp.hpp>

#include "RUtil.h"
#include "RSyncClient.h"

struct RSyncClient::Impl
{
	Impl(boost::asio::io_service & io_src)
		: m_socket(io_src)
	{

	}

	bool connect( const std::string & ip, const unsigned int & port )
	{
		boost::system::error_code ec;
		boost::asio::ip::address addr = boost::asio::ip::address::from_string(ip);

		boost::asio::ip::tcp::endpoint ep(addr, port);
		m_socket.connect(ep, ec);

		return !ec;
	}

	RVariant command(
		const std::string & cmd)
	{

		RVariant ret;

		std::string message;
		std::vector<RVariant> params;
		RUtil::create_message(cmd, params, message);

		command_p(message, ret);

		return ret;
	}

	RVariant command(
		const std::string & cmd, 
		const RVariant & param_1)
	{
		RVariant ret;

		std::string message;
		std::vector<RVariant> params;
		params.push_back(param_1);
		RUtil::create_message(cmd, params, message);

		command_p(message, ret);

		return ret;
	}

	RVariant command(
		const std::string & cmd, 
		const RVariant & param_1, 
		const RVariant & param_2)
	{
		RVariant ret;

		std::string message;
		std::vector<RVariant> params;
		params.push_back(param_1);
		params.push_back(param_2);
		RUtil::create_message(cmd, params, message);

		command_p(message, ret);

		return ret;
	};

	void command_p(const std::string & param, RVariant & result)
	{
		m_socket.send(boost::asio::buffer(param));

		std::string rece_data;
		for (;;)
		{
			boost::system::error_code ec;
			std::vector<char> rece_buf(m_socket.available() + 1, 0);
			int len = m_socket.receive(boost::asio::buffer(rece_buf), 0, ec);

			rece_data.append(rece_buf.begin(), rece_buf.end());

			boost::string_ref left_str;
			if (RVariant::parseFromString(rece_data, result, left_str))
			{
				return;
			}
			else
			{
			}
		}
	};
	
	boost::asio::ip::tcp::socket m_socket;
};

RSyncClient::RSyncClient(boost::asio::io_service & io_src)
	: m_pImpl(new Impl(io_src))
{
}


RSyncClient::~RSyncClient(void)
{
}

bool RSyncClient::connect( 
	const std::string & ip, 
	const unsigned int & port )
{
	return m_pImpl->connect(ip, port);
}

RVariant RSyncClient::command( const std::string & cmd )
{
	return m_pImpl->command(cmd);
}

RVariant RSyncClient::command( 
	const std::string & cmd, 
	const RVariant & param_1 )
{
	return m_pImpl->command(cmd, param_1);
}

RVariant RSyncClient::command( 
	const std::string & cmd, 
	const RVariant & param_1, 
	const RVariant & param_2 )
{
	return m_pImpl->command(cmd, param_1, param_2);
}
