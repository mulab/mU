#pragma once
#include "Interface.h"

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
API void neg(var&, const Object&);
API void div(var&, const Object&);
API void div(var&, const Object&, const Object&);
API void add(var&, const Object&, const Object&);
API void mul(var&, const Object&, const Object&);
}
}