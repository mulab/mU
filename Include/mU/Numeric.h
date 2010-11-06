#pragma once
#include "Interface.h"
#include "Kernel.h"

#undef API
#ifdef _MSC_VER
#ifdef NUMERIC_EXPORTS
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#else
#define API
#endif

#undef ASSIGN
#define ASSIGN(f) CASSIGN(System_Numeric,f)
#undef VALUE
#define VALUE(f) CVALUE(System_Numeric,f)
#undef METHOD
#define METHOD(f,sig) CMETHOD(System_Numeric,f,sig)

namespace mU {
API var N(Kernel&, const var&, uint = 0);
API uint Precision(Kernel&, const var&);
API int Compare(Kernel&, const var&, const var&);
namespace Numeric {
API void ConstPi(Real&);
API void ConstEuler(Real&);
API void ConstCatalan(Real&);

API void Log(Real&, const Real&);
API void Log2(Real&, const Real&);
API void Log10(Real&, const Real&);
API void Exp(Real&, const Real&);
API void Exp2(Real&, const Real&);
API void Exp10(Real&, const Real&);
API void Cos(Real&, const Real&);
API void Sin(Real&, const Real&);
API void Tan(Real&, const Real&);
API void Sec(Real&, const Real&);
API void Csc(Real&, const Real&);
API void Cot(Real&, const Real&);
API void ArcCos(Real&, const Real&);
API void ArcSin(Real&, const Real&);
API void ArcTan(Real&, const Real&);
API void Cosh(Real&, const Real&);
API void Sinh(Real&, const Real&);
API void Tanh(Real&, const Real&);
API void Sech(Real&, const Real&);
API void Csch(Real&, const Real&);
API void Coth(Real&, const Real&);
API void ArcCosh(Real&, const Real&);
API void ArcSinh(Real&, const Real&);
API void ArcTanh(Real&, const Real&);
API void Gamma(Real&, const Real&);
API void LogGamma(Real&, const Real&);
API void Zeta(Real&, const Real&);
API void Erf(Real&, const Real&);
API void Erfc(Real&, const Real&);
API void Sqrt(Real&, const Real&);
API void Power(Real&, const Real&, const Real&);
}
}
