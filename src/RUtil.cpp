#include "stdafx.h"

#include <string>
#include <sstream>

#include "RUtil.h"

#define CRLF "\r\n"

bool RUtil::create_message( 
	_in const std::string & cmd, 
	_in std::vector<RVariant> params, 
	_out std::string & message )
{
	std::stringstream ss;
	ss << "*" << params.size() + 1 << CRLF;
	ss << "$" << cmd.size() << CRLF;
	ss << cmd << CRLF;

	for (size_t i = 0; i < params.size(); ++i)
	{
		if (params[i].isString())
		{
			ss << "$" << params[i].toString().size() << CRLF;
			ss << params[i].toString() << CRLF;
		}
		else if (params[i].isInt())
		{
			ss << ":" << params[i].toInt() << CRLF;
		}
		else if (params[i].isError())
		{
			ss << "-" << params[i].toError() << CRLF;
		}
		else if (params[i].isArray())
		{
			std::vector<RVariant> sub_params = params[i].toArray();
		}
		else
		{
			return false;
		}
	}

	message = ss.str();

	return true;
}
