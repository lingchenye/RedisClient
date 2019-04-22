#include "StdAfx.h"

#include <queue>

#include <boost/bind.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "RUtil.h"
#include "RAsyncClient.h"

struct RAsyncClient::Impl
{
	Impl(boost::asio::io_service & io_src)
		: m_buf(4096, 0)
		, m_socket(io_src)
		, m_state(kDisconnect)
	{

	}

	enum State
	{
		kDisconnect	=	1 << 1, 
		kConnect	=	1 << 2, 
		kSubscribe	=	1 << 3, 
	};

	void connect( 
		const std::string & ip, 
		const unsigned int & port, 
		RAsyncClient::RConnectCallbck cb)
	{
		boost::system::error_code ec;
		boost::asio::ip::address addr = boost::asio::ip::address::from_string(ip);

		boost::asio::ip::tcp::endpoint ep(addr, port);

		m_socket.async_connect(ep, 
			boost::bind(&Impl::connect_handler, this, _1, cb));
	}

	void command( 
		const std::string & cmd, 
		RAsyncClient::RCommandCallbck cb)
	{
		std::string message;
		std::vector<RVariant> params;
		RUtil::create_message(cmd, params, message);

		command_p(message, cb);
	}

	void command(
		const std::string & cmd, 
		const RVariant & param_1, 
		RAsyncClient::RCommandCallbck cb)
	{
		std::string message;
		std::vector<RVariant> params;
		params.push_back(param_1);
		RUtil::create_message(cmd, params, message);

		command_p(message, cb);
	}

	void command(
		const std::string & cmd, 
		const RVariant & param_1, 
		const RVariant & param_2, 
		RAsyncClient::RCommandCallbck cb)
	{
		std::string message;
		std::vector<RVariant> params;
		params.push_back(param_1);
		params.push_back(param_2);
		RUtil::create_message(cmd, params, message);

		command_p(message, cb);
	};

	void subscribe( 
		const std::string & channel, 
		RAsyncClient::RSubscribeCallback cb )
	{
		m_state |= kSubscribe;

		m_sub_cb[channel] = cb;

		command("subscribe", channel, cb);
	}

	void publish( 
		const std::string & channel, 
		const RVariant & var, 
		RAsyncClient::RPublishCallback cb )
	{
		command("publish", RVariant(channel), var, cb);
	}

	void command_p( 
		const std::string & param, 
		RAsyncClient::RCommandCallbck cb)
	{
		boost::asio::async_write(
			m_socket, 
			boost::asio::buffer(param.c_str(), param.size()), 
			boost::asio::transfer_all(), 
			boost::bind(&Impl::async_write, this, _1, _2, cb));
	};

	void connect_handler(
		const boost::system::error_code& ec, 
		RAsyncClient::RConnectCallbck cb)
	{
		if (!ec)
		{
			boost::asio::async_read(
				m_socket, 
				boost::asio::buffer(m_buf), 
				boost::asio::transfer_at_least(1), 
				boost::bind(&Impl::async_read, this, _1, _2));
		}
		cb(!ec, !ec ? "" : ec.message());
	}

	void async_read(
		const boost::system::error_code & ec, 
		std::size_t len)
	{
		RVariant var;
		std::string message;
		message.append(m_left_str);
		message.append(m_buf.begin(), m_buf.begin() + len);

		boost::string_ref left_str;
		if (RVariant::parseFromString(message, var, left_str))
		{
			m_left_str = std::string(left_str.begin(), left_str.end());

			// subscribe channel
			if (m_state & kSubscribe)
			{
				if (var.isArray() && var.toArray().size() >= 3)
				{
					std::vector<RVariant> vars = var.toArray();
					auto it = m_sub_cb.find(vars[1].toString());
					if (vars[1].isString() && 
						vars[0].toString() == "message" && 
						it != m_sub_cb.end())
					{
						(it->second)(vars[2]);
					}
				}
			}
			else if (!m_cmd_cb.empty())
			{
				if (RCommandCallbck cb = m_cmd_cb.front())
				{
					m_cmd_cb.pop();
					cb(var);
				}
			}

		}
		else
		{
			m_left_str = message;
		}

		boost::asio::async_read(
			m_socket, 
			boost::asio::buffer(m_buf), 
			boost::asio::transfer_at_least(1), 
			boost::bind(&Impl::async_read, this, _1, _2));
	}

	void async_write(
		const boost::system::error_code & ec, 
		std::size_t bytes, 
		RAsyncClient::RCommandCallbck cb)
	{
		if (!ec)
		{
			m_cmd_cb.push(cb);
		}
	}

	std::string m_left_str;
	std::vector<char> m_buf;
	boost::asio::ip::tcp::socket m_socket;

	uint8_t m_state;
	std::queue<RAsyncClient::RCommandCallbck> m_cmd_cb;
	std::map<std::string, RAsyncClient::RSubscribeCallback> m_sub_cb;
};

RAsyncClient::RAsyncClient(boost::asio::io_service & io_src)
	: m_pImpl(new Impl(io_src))
{
}


RAsyncClient::~RAsyncClient(void)
{
}

void RAsyncClient::connect( 
	const std::string & ip, 
	const unsigned int & port, 
	RConnectCallbck cb )
{
	m_pImpl->connect(ip, port, cb);
}

void RAsyncClient::command( 
	const std::string & cmd, 
	RCommandCallbck cb )
{
	m_pImpl->command(cmd, cb);
}

void RAsyncClient::command( 
	const std::string & cmd, 
	const RVariant & param_1, 
	RCommandCallbck cb )
{
	m_pImpl->command(cmd, param_1, cb);
}

void RAsyncClient::command( 
	const std::string & cmd, 
	const RVariant & param_1, 
	const RVariant & param_2, 
	RCommandCallbck cb )
{
	m_pImpl->command(cmd, param_1, param_2, cb);
}

void RAsyncClient::subscribe( 
	const std::string & channel, 
	RSubscribeCallback cb )
{
	m_pImpl->subscribe(channel, cb);
}

void RAsyncClient::publish( 
	const std::string & channel, 
	const RVariant & var, 
	RPublishCallback cb )
{
	m_pImpl->publish(channel, var, cb);
}



//1.发送指令
//	链接？非订购？
//2.订购通道
//	链接？
//3.链接
// 
//	链接？
//		发送指令
//			订购，订购类指令？