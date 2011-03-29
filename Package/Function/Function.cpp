#include <mU/Function.h>

namespace mU {
namespace Function {
namespace {
Wrap(Factorial)
{
	if(IntQ(At(x,0))) return Factorial(Z::UI(At(x,0)));
	return 0;
}
Wrap(Binomial)
{
	if(IntQ(At(x,0))) return Binomial(At(x,0),Z::UI(At(x,1)));
	return 0;
}
Wrap(Fibonacci)
{
	if(IntQ(At(x,0))) return Fibonacci(Z::UI(At(x,0)));
	return 0;
}
Wrap(Lucas)
{
	if(IntQ(At(x,0))) return Lucas(Z::UI(At(x,0)));
	return 0;
}
Wrap(EulerGamma)
{
	return Euler();
}
Wrap(Catalan)
{
	return Catalan();
}
Wrap(Log)
{
	if(FltQ(At(x,0))) return Log(At(x,0));
	return 0;
}
Wrap(Log2)
{
	if(FltQ(At(x,0))) return Log2(At(x,0));
	return 0;
}
Wrap(Log10)
{
	if(FltQ(At(x,0))) return Log10(At(x,0));
	return 0;
}
Wrap(Exp)
{
	if(FltQ(At(x,0))) return Exp(At(x,0));
	return 0;
}
Wrap(Cos)
{
	if(FltQ(At(x,0))) return Cos(At(x,0));
	return 0;
}
Wrap(Sin)
{
	if(FltQ(At(x,0))) return Sin(At(x,0));
	return 0;
}
Wrap(Tan)
{
	if(FltQ(At(x,0))) return Tan(At(x,0));
	return 0;
}
Wrap(Sec)
{
	if(FltQ(At(x,0))) return Sec(At(x,0));
	return 0;
}
Wrap(Csc)
{
	if(FltQ(At(x,0))) return Csc(At(x,0));
	return 0;
}
Wrap(Cot)
{
	if(FltQ(At(x,0))) return Cot(At(x,0));
	return 0;
}
Wrap(ArcCos)
{
	if(FltQ(At(x,0))) return Acos(At(x,0));
	return 0;
}
Wrap(ArcSin)
{
	if(FltQ(At(x,0))) return Asin(At(x,0));
	return 0;
}
Wrap(ArcTan)
{
	if(FltQ(At(x,0))) return Atan(At(x,0));
	return 0;
}
Wrap(Cosh)
{
	if(FltQ(At(x,0))) return Cosh(At(x,0));
	return 0;
}
Wrap(Sinh)
{
	if(FltQ(At(x,0))) return Sinh(At(x,0));
	return 0;
}
Wrap(Tanh)
{
	if(FltQ(At(x,0))) return Tanh(At(x,0));
	return 0;
}
Wrap(Sech)
{
	if(FltQ(At(x,0))) return Sech(At(x,0));
	return 0;
}
Wrap(Csch)
{
	if(FltQ(At(x,0))) return Csch(At(x,0));
	return 0;
}
Wrap(Coth)
{
	if(FltQ(At(x,0))) return Coth(At(x,0));
	return 0;
}
Wrap(ArcCosh)
{
	if(FltQ(At(x,0))) return Acosh(At(x,0));
	return 0;
}
Wrap(ArcSinh)
{
	if(FltQ(At(x,0))) return Asinh(At(x,0));
	return 0;
}
Wrap(ArcTanh)
{
	if(FltQ(At(x,0))) return Atanh(At(x,0));
	return 0;
}
Wrap(Gamma)
{
	if(FltQ(At(x,0))) return Gamma(At(x,0));
	return 0;
}
Wrap(LogGamma)
{
	if(FltQ(At(x,0))) return Lngamma(At(x,0));
	return 0;
}
Wrap(Zeta)
{
	if(FltQ(At(x,0))) return Zeta(At(x,0));
	return 0;
}
Wrap(Erf)
{
	if(FltQ(At(x,0))) return Erf(At(x,0));
	return 0;
}
Wrap(Erfc)
{
	if(FltQ(At(x,0))) return Erfc(At(x,0));
	return 0;
}
Wrap(Sqrt)
{
	if(FltQ(At(x,0))) return Sqrtf(At(x,0));
	return Power(At(x,0),Rat(1L,2L));
}
Wrap(Pow)
{
	if(FltQ(At(x,0)) && FltQ(At(x,1))) return Powf(At(x,0),At(x,1));
	return 0;
}
Wrap(Pi)
{
	return Pif();
}
Wrap(E)
{
	return Exp(Flt(1L));
}
}
void Initialize()
{
	static bool Initialized = false;
	if(Initialized) return;
#define T(x) var TAG(x) = Sym(L###x,System);Externals[TAG(x)] = WRAP(x);
	T(Factorial)T(Binomial)T(Fibonacci)T(Lucas)T(EulerGamma)T(Catalan)T(Log)T(Log2)
	T(Log10)T(Exp)T(Cos)T(Sin)T(Tan)T(Sec)T(Csc)T(Cot)T(ArcCos)T(ArcSin)T(ArcTan)
	T(Cosh)T(Sinh)T(Tanh)T(Sech)T(Csch)T(Coth)T(ArcCosh)T(ArcSinh)T(ArcTanh)T(Gamma)
	T(LogGamma)T(Zeta)T(Erf)T(Erfc)
#undef T
#define T(x) var TAG(x) = Sym(L###x,System);Functionals[TAG(x)] = WRAP(x);

#undef T
#define T(x,y) Attributes[TAG(x)].insert(y);
	
#undef T
#define T(x) Externals[TAG(x)] = WRAP(x);
	T(Sqrt)T(Pow)T(Pi)T(E)
#undef T
	Initialized = true;
}
}
}
DLLMAIN(mU::Function::Initialize)
