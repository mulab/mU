#pragma once
#include "Number.h"
#include "Kernel.h"

#ifdef _WIN32
#ifdef FUNCTION_EXPORTS
#define FUNCTION_API __declspec(dllexport) extern
#else
#define FUNCTION_API __declspec(dllimport) extern
#pragma comment(lib,"Function")
#endif
#else
#define FUNCTION_API extern
#endif

namespace mU {
namespace Function {
FUNCTION_API var Factorial(uint);
FUNCTION_API var Binomial(Var,uint);
FUNCTION_API var Binomial(Var,uint);
FUNCTION_API var Fibonacci(uint);
FUNCTION_API var Lucas(uint);
FUNCTION_API var Euler();
FUNCTION_API var Catalan();
FUNCTION_API var Log(Var);
FUNCTION_API var Log2(Var);
FUNCTION_API var Log10(Var);
FUNCTION_API var Exp(Var);
FUNCTION_API var Exp2(Var);
FUNCTION_API var Exp10(Var);
FUNCTION_API var Cos(Var);
FUNCTION_API var Sin(Var);
FUNCTION_API var Tan(Var);
FUNCTION_API var Sec(Var);
FUNCTION_API var Csc(Var);
FUNCTION_API var Cot(Var);
FUNCTION_API var Acos(Var);
FUNCTION_API var Asin(Var);
FUNCTION_API var Atan(Var);
FUNCTION_API var Cosh(Var);
FUNCTION_API var Sinh(Var);
FUNCTION_API var Tanh(Var);
FUNCTION_API var Sech(Var);
FUNCTION_API var Csch(Var);
FUNCTION_API var Coth(Var);
FUNCTION_API var Acosh(Var);
FUNCTION_API var Asinh(Var);
FUNCTION_API var Atanh(Var);
FUNCTION_API var Gamma(Var);
FUNCTION_API var Lngamma(Var);
FUNCTION_API var Zeta(Var);
FUNCTION_API var Erf(Var);
FUNCTION_API var Erfc(Var);
FUNCTION_API var Sqrtf(Var);
FUNCTION_API var Powf(Var,Var);
FUNCTION_API var Pif();
}
}
