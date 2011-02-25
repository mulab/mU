/*
 * Exceptions.h
 *
 *  Created on: 2010-1-8
 *      Author: baritono
 */

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

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

	class InvalidUtf8StreamException : public RuntimeException
	{

	};

	class UnsupportedCharacterException : public RuntimeException
	{

	};
}

#endif /* EXCEPTIONS_H_ */
