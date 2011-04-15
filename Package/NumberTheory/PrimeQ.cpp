/** \file 
\brief 素性检测.
*/
#include "common.h"
#include "objects/integer.h"
#include <gmp.h>

#pragma comment(lib, "gmp")

namespace Integer = maTHmU::Objects::Integer;

namespace maTHmU {
namespace
{ 
	inline mpz_ptr PTR(VOID *x) { return (mpz_ptr)x; }
	inline mpz_ptr PTR(VAR x) { return PTR(x.ptr); }
	inline Z& CAST(VAR x) { return *(Z*)(&x); }
	
	inline var operator * (VAR x, VAR y)
	{
		return Integer::Mul(x,y);
	}
	inline var operator +(VAR x, VAR y)
	{
		return Integer::Add(x,y);
	}
	inline var operator -(VAR x, VAR y)
	{
		return Integer::Sub(x,y);
	}
	inline bool operator ==(VAR x, VAR y)
	{
		return Integer::Cmp(x,y)==0;
	}
	inline bool operator <(VAR x, VAR y)
	{
		return Integer::Cmp(x,y)<0;
	}
	inline bool operator >(VAR x, VAR y)
	{
		return Integer::Cmp(x,y)>0;
	}
	inline var operator /(VAR x, VAR y)
	{
		return Modules::NumberTheory::Quotient(x,y);
	}

}

namespace Modules {
namespace NumberTheory {

/** \brief 判断\f$n\f$是否为素数.
	\param n 整数.
	\retval >0 是.
	\retval 0 否.
	\note 使用Rabin-Miller强伪素数检测.
*/
INT PrimeQ(VAR n) {
	return mpz_probab_prime_p(PTR(n),10);
}

}
}
}