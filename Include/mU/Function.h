#pragma once
#include "Interface.h"
#include "Kernel.h"

#undef API
#ifdef _MSC_VER
#ifdef FUNCTION_EXPORTS
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#else
#define API
#endif

#undef ASSIGN
#define ASSIGN(f) CASSIGN(System_Function,f)
#undef VALUE
#define VALUE(f) CVALUE(System_Function,f)
#undef METHOD
#define METHOD(f,sig) CMETHOD(System_Function,f,sig)

namespace mU {
API var Factorial(uint);
API var Binomial(const Integer&, uint);
API var Binomial(const Integer&, uint);
API var Fibonacci(uint);
API var Lucas(uint);
API var Pi();
API var Euler();
API var Catalan();
API var Log(const Real&);
API var Log2(const Real&);
API var Log10(const Real&);
API var Exp(const Real&);
API var Exp2(const Real&);
API var Exp10(const Real&);
API var Cos(const Real&);
API var Sin(const Real&);
API var Tan(const Real&);
API var Sec(const Real&);
API var Csc(const Real&);
API var Cot(const Real&);
API var Acos(const Real&);
API var Asin(const Real&);
API var Atan(const Real&);
API var Cosh(const Real&);
API var Sinh(const Real&);
API var Tanh(const Real&);
API var Sech(const Real&);
API var Csch(const Real&);
API var Coth(const Real&);
API var Acosh(const Real&);
API var Asinh(const Real&);
API var Atanh(const Real&);
API var Gamma(const Real&);
API var Lngamma(const Real&);
API var Zeta(const Real&);
API var Erf(const Real&);
API var Erfc(const Real&);
API var Sqrtf(const Real&);
API var Powf(const Real&, const Real&);
}
