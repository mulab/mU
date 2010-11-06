#pragma once
#include "Interface.h"
#include "Kernel.h"

#undef API
#ifdef _MSC_VER
#ifdef COMBINATORICS_EXPORTS
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#else
#define API
#endif

#undef ASSIGN
#define ASSIGN(f) CASSIGN(System_Combinatorics,f)
#undef VALUE
#define VALUE(f) CVALUE(System_Combinatorics,f)
#undef METHOD
#define METHOD(f,sig) CMETHOD(System_Combinatorics,f,sig)

namespace mU {

namespace Combinatorics {
API var Factorial(uint);
API var Binomial(uint, uint);
API var Binomial(const Integer&, uint);
API var Fibonacci(uint);
API var Lucas(uint);
}
}
