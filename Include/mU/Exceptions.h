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

	class RuntimeException : public Exception
	{

	};

	class IOException : public RuntimeException
	{

	};

	class InvalidUtf8StreamException : public RuntimeException
	{

	};
}

#endif /* EXCEPTIONS_H_ */
