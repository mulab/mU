#pragma once
#include "Interface.h"

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
API extern sym poly;
API var Add(Kernel&, const Tuple&, const Tuple&);
API var Mul(Kernel&, const Tuple&, const Tuple&);
}
API var Minus(Kernel&, const var&);
API var Divide(Kernel&, const var&);
API var Divide(Kernel&, const var&, const var&);
API var Plus(Kernel&, const Tuple&);
API var Plus(Kernel&, const var&, const var&);
API var Times(Kernel&, const Tuple&);
API var Times(Kernel&, const var&, const var&);
API var Expand(Kernel&, const Tuple&);
API var Expand(Kernel&, const var&, const var&);
}