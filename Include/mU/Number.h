#pragma once
#include "Interface.h"
#include "Kernel.h"

#undef API
#ifdef _MSC_VER
#ifdef NUMBER_EXPORTS
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#else
#define API
#endif

#undef ASSIGN
#define ASSIGN(f) CASSIGN(System_Number,f)
#undef VALUE
#define VALUE(f) CVALUE(System_Number,f)
#undef METHOD
#define METHOD(f,sig) CMETHOD(System_Number,f,sig)

namespace mU {
namespace Number {
API var Cast(Kernel&, const var&, sym);
API var Minus(Kernel&, const Object&);
API var Divide(Kernel&, const Object&);
API var Divide(Kernel&, const Object&, const Object&);
API var Plus(Kernel&, const Object&, const Object&);
API var Times(Kernel&, const Object&, const Object&);
}
}