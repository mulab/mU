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

}
API var Minus(Kernel&, const var&);
API var Divide(Kernel&, const var&);
API var Divide(Kernel&, const var&, const var&);
API var Plus(Kernel&, const Tuple&);
API var Plus(Kernel&, const var&, const var&);
API var Times(Kernel&, const Tuple&);
API var Times(Kernel&, const var&, const var&);
}