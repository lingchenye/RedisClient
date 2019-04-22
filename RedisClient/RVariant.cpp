#include "StdAfx.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/utility/string_ref.hpp>

#include "RVariant.h"

#define CRLF "\r\n"

RVariant::RVariant(void)
{

}

RVariant::RVariant( const int & i )
{
	m_data = i;
}

RVariant::RVariant( const std::string & str )
{
	m_data = str;
}

RVariant::~RVariant(void)
{
}

std::ostream & operator << (std::ostream & os, const RVariant & var)
{
	if (var.isInt())
	{
		os << var.toInt();
	}
	else if (var.isError())
	{
		os << var.toError();
	}
	else if (var.isString())
	{
		os << var.toString();
	}
	if (var.isArray())
	{
		os << "{";
		std::vector<RVariant> varList = var.toArray();
		for (size_t i = 0; i < varList.size(); ++i)
		{
			os << varList[i] << (i != varList.size() - 1 ? "," : "");
		}
		os << "}";
	}

	return os;
}

int RVariant::toInt() const
{
	if (!isInt())
	{
		throw boost::bad_any_cast();
	}

	return boost::any_cast<int>(m_data);
}

std::string RVariant::toError() const
{
	if (!isError())
	{
		throw boost::bad_any_cast();
	}

	return boost::any_cast<std::string>(m_data);
}

std::string RVariant::toString() const
{
	if (!isString())
	{
		throw boost::bad_any_cast();
	}

	return boost::any_cast<std::string>(m_data);
}

std::vector<RVariant> RVariant::toArray() const
{
	if (!isArray())
	{
		throw boost::bad_any_cast();
	}

	return boost::any_cast<std::vector<RVariant> >(m_data);
}

bool RVariant::isInt() const
{
	return can_cast<int>();
}

bool RVariant::isError() const
{
	return m_isError;
}

bool RVariant::isString() const
{
	return can_cast<std::string>();
}

bool RVariant::isArray() const
{
	return can_cast<std::vector<RVariant> >();
}

bool RVariant::parseFromString( 
	const std::string & str, 
	RVariant & var, 
	boost::string_ref left_str)
{
	return parseFromString_p(str, var, left_str);
}

bool RVariant::parseFromString_p( 
	const boost::string_ref & param, 
	RVariant & var, 
	boost::string_ref & left_param )
{
	var.m_isError = false;

	boost::string_ref::const_iterator data_begin = param.begin() + 1;
	boost::string_ref::const_iterator first_crlf_pre = boost::find_first(param, CRLF).begin();
	boost::string_ref::const_iterator first_crlf_post = boost::find_first(param, CRLF).end();
	
	// non complete param without end with CRLF
	if (first_crlf_pre == param.end())
	{
		return false;
	}

	if (boost::starts_with(param, "+"))
	{
		var.m_data = std::string(data_begin, first_crlf_pre);

		size_t start_pos = std::distance(param.begin(), first_crlf_post);
		size_t len = std::distance(first_crlf_post, param.end());
		left_param = param.substr(start_pos, len);
	}
	else if (boost::starts_with(param, "-"))
	{
		var.m_data = std::string(data_begin, first_crlf_pre);
		var.m_isError = true;

		size_t start_pos = std::distance(param.begin(), first_crlf_post);
		size_t len = std::distance(first_crlf_post, param.end());
		left_param = param.substr(start_pos, len);
	}
	else if (boost::starts_with(param, ":"))
	{
		std::string i_str(data_begin, first_crlf_pre);
		var.m_data = boost::lexical_cast<int>(i_str);

		size_t start_pos = std::distance(param.begin(), first_crlf_post);
		size_t len = std::distance(first_crlf_post, param.end());
		left_param = param.substr(start_pos, len);
	}
	else if (boost::starts_with(param, "$"))
	{
		std::string i_str(data_begin, first_crlf_pre);
		int len = boost::lexical_cast<int>(i_str);

		auto range = boost::find_first(param, CRLF);
		var.m_data = std::string(first_crlf_post, first_crlf_post + len);

		size_t start_pos = std::distance(param.begin(), first_crlf_post + len);
		size_t len_sub = std::distance(first_crlf_post + len, param.end());
		left_param = param.substr(start_pos + 2, len_sub);
	}
	else if (boost::starts_with(param, "*"))
	{
		// get array size 
		std::string i_str(data_begin, first_crlf_pre);
		int size = boost::lexical_cast<int>(i_str);

		// get left param
		size_t start_pos = std::distance(param.begin(), first_crlf_post);
		size_t len_sub = std::distance(first_crlf_post, param.end());
		boost::string_ref handle_param = param.substr(start_pos, len_sub);
		
		std::vector<RVariant> arr(size, 0);
		for (size_t i = 0; i < arr.size(); ++i)
		{
			if (!parseFromString_p(handle_param, arr[i], handle_param))
			{
				return false;
			}
		}

		var.m_data = arr;
	}
	else
	{
		return false;
	}

	return true;
}
