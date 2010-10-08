#pragma once
#include "Interface.h"
#include "Kernel.h"

#undef API
#ifdef _MSC_VER
#ifdef POLYNOMIAL_EXPORTS
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#else
#define API
#endif

#undef ASSIGN
#define ASSIGN(f) CASSIGN(System_Polynomial,f)
#undef VALUE
#define VALUE(f) CVALUE(System_Polynomial,f)
#undef METHOD
#define METHOD(f,sig) CMETHOD(System_Polynomial,f,sig)

namespace mU {
namespace Polynomial {
API var Cast(Kernel&, const var&, sym);
}
inline int cmpD(const Object& x, double a = 0.0, double b = 1.0) {
	if (x.type == $.Integer)
		return mpz_cmp_si(static_cast<const Integer&>(x).mpz, 
		static_cast<long>(a / b));
	if (x.type == $.Rational)
		return mpq_cmp_si(static_cast<const Rational&>(x).mpq, static_cast<long>(a), 
		static_cast<long>(b));
	if (x.type == $.Real)
		return mpf_cmp_d(static_cast<const Real&>(x).mpf, a / b);
	return 0;
}
API var Plus(Kernel&, const Object&, const Object&);
API var Plus(Kernel&, const Tuple&);
API var Times(Kernel&, const Object&, const Object&);
API var Times(Kernel&, const Tuple&);
}