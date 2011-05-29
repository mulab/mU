/*
 * Exceptions.h
 *
 *  Created on: 2010-1-8
 *      Author: baritono
 */

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <boost/system/error_code.hpp>
#include <boost/version.hpp>
#ifdef _WIN32
#include <Windows.h>
#else
#include <errno.h>
#endif

namespace mU
{
	class Exception
	{

	};

	class ParserException : public Exception
	{

	};

	class UnexpectedTokenException : public ParserException
	{

	};

	class MoreInputNeededException : public ParserException
	{

	};

	class RuntimeException : public Exception
	{

	};

	class IOException : public RuntimeException
	{

	};

	class InvalidCodePointException : public RuntimeException
	{

	};

	class InvalidUtf8StreamException : public InvalidCodePointException
	{

	};

	class UnsupportedCharacterException : public RuntimeException
	{

	};

	class SystemError : public RuntimeException
	{
	public:
		SystemError()
#if (BOOST_VERSION >= 104400)
#ifdef _WIN32
			: error(GetLastError(), boost::system::system_category())
#else
			: error(errno, boost::system::system_category())
#endif
#else
#ifdef _WIN32
			: error(GetLastError(), boost::system::system_category)
#else
			: error(errno, boost::system::system_category)
#endif
#endif
		{}
	private:
		boost::system::error_condition error;
	};

#ifdef _WIN32
#define API_CALL(api_func, ...)			\
	if (!api_func(__VA_ARGS__))			\
		throw SystemError();
#define API_CALL_R(r, api_func, ...)	\
	if (!(r = api_func(__VA_ARGS__)))	\
		throw SystemError();
#else
#define API_CALL(api_func, ...) 		\
	if (api_func(__VA_ARGS__) == -1)	\
		throw SystemError();
#define API_CALL_R(r, api_func, ...)		\
	if ((r = api_func(__VA_ARGS__)) == -1)	\
		throw SystemError();
#endif
}

#endif /* EXCEPTIONS_H_ */
