#ifndef RVariant_h__
#define RVariant_h__

#include <string>
#include <vector>

#include <boost/any.hpp>
#include <boost/utility/string_ref.hpp>

#include "RedisClient_global.h"

class REDISCLIENT_GLOBAL_DECL RVariant
{
public:
	RVariant(void);

	RVariant(const int & i);

	RVariant(const std::string & str);

	~RVariant(void);

	REDISCLIENT_GLOBAL_DECL friend std::ostream & operator << (
		std::ostream & os, 
		const RVariant & var);

	int toInt() const;

	std::string toError() const;

	std::string toString() const;

	std::vector<RVariant> toArray() const;

	bool isInt() const;

	bool isError() const;

	// both simple string and bulk string
	bool isString() const;

	bool isArray() const;

	static bool parseFromString(
		const std::string & param, 
		RVariant & var, 
		boost::string_ref left_str);

private:

	static bool parseFromString_p(
		const boost::string_ref & param, 
		RVariant & var, 
		boost::string_ref & left_param);

	template<class T>
	bool can_cast() const
	{
		return typeid(T) == m_data.type();
	};

private:
	boost::any m_data;
	bool m_isError;
};

#endif // RVariant_h__
