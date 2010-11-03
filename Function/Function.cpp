#include <mU/Function.h>
#ifdef _MSC_VER
#pragma comment(lib,"Kernel.lib")
#endif

using namespace mU;

CAPI void Install(Kernel& k) {
	//wcout << "#Function# Install Successfully!" << endl;
}
CAPI void Uninstall(Kernel& k) {
	wcout << "#Function# Uninstall Successfully!" << endl;
}
CAPI void CVALUE(System, Factorial)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject())
		r = Factorial(static_cast<uint>(toD(x[1].object())));
}
CAPI void CVALUE(System, Binomial)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 2 && x[1].isObject() && x[1].object().type == $.Integer && x[2].isObject())
		r = Binomial(cast<Integer>(x[1]), static_cast<uint>(toD(x[2].object())));
}
CAPI void CVALUE(System, Fibonacci)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject())
		r = Fibonacci(static_cast<uint>(toD(x[1].object())));
}
CAPI void CVALUE(System, Lucas)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject())
		r = Lucas(static_cast<uint>(toD(x[1].object())));
}
CAPI void CVALUE(System, Pi)(Kernel& k, var& r, Tuple& x) {
	r = Pi();
}
CAPI void CVALUE(System, EulerGamma)(Kernel& k, var& r, Tuple& x) {
	r = Euler();
}
CAPI void CVALUE(System, Catalan)(Kernel& k, var& r, Tuple& x) {
	r = Catalan();
}
CAPI void CVALUE(System, E)(Kernel& k, var& r, Tuple& x) {
	r = new Real(1L);
	r = Exp(cast<Real>(r));
}
CAPI void CVALUE(System, Log)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject() && x[1].object().type == $.Real)
		r = Log(cast<Real>(x[1]));
}
CAPI void CVALUE(System, Log2)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject() && x[1].object().type == $.Real)
		r = Log2(cast<Real>(x[1]));
}
CAPI void CVALUE(System, Log10)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject() && x[1].object().type == $.Real)
		r = Log10(cast<Real>(x[1]));
}
CAPI void CVALUE(System, Exp)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject() && x[1].object().type == $.Real)
		r = Exp(cast<Real>(x[1]));
}
CAPI void CVALUE(System, Cos)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject() && x[1].object().type == $.Real)
		r = Cos(cast<Real>(x[1]));
}
CAPI void CVALUE(System, Sin)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject() && x[1].object().type == $.Real)
		r = Sin(cast<Real>(x[1]));
}
CAPI void CVALUE(System, Tan)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject() && x[1].object().type == $.Real)
		r = Tan(cast<Real>(x[1]));
}
CAPI void CVALUE(System, Sec)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject() && x[1].object().type == $.Real)
		r = Sec(cast<Real>(x[1]));
}
CAPI void CVALUE(System, Csc)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject() && x[1].object().type == $.Real)
		r = Csc(cast<Real>(x[1]));
}
CAPI void CVALUE(System, Cot)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject() && x[1].object().type == $.Real)
		r = Cot(cast<Real>(x[1]));
}
CAPI void CVALUE(System, ArcCos)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject() && x[1].object().type == $.Real)
		r = Acos(cast<Real>(x[1]));
}
CAPI void CVALUE(System, ArcSin)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject() && x[1].object().type == $.Real)
		r = Asin(cast<Real>(x[1]));
}
CAPI void CVALUE(System, ArcTan)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject() && x[1].object().type == $.Real)
		r = Atan(cast<Real>(x[1]));
}
CAPI void CVALUE(System, Cosh)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject() && x[1].object().type == $.Real)
		r = Cosh(cast<Real>(x[1]));
}
CAPI void CVALUE(System, Sinh)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject() && x[1].object().type == $.Real)
		r = Sinh(cast<Real>(x[1]));
}
CAPI void CVALUE(System, Tanh)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject() && x[1].object().type == $.Real)
		r = Tanh(cast<Real>(x[1]));
}
CAPI void CVALUE(System, Coth)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject() && x[1].object().type == $.Real)
		r = Coth(cast<Real>(x[1]));
}
CAPI void CVALUE(System, Sech)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject() && x[1].object().type == $.Real)
		r = Sech(cast<Real>(x[1]));
}
CAPI void CVALUE(System, Csch)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject() && x[1].object().type == $.Real)
		r = Csch(cast<Real>(x[1]));
}
CAPI void CVALUE(System, ArcCosh)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject() && x[1].object().type == $.Real)
		r = Acosh(cast<Real>(x[1]));
}
CAPI void CVALUE(System, ArcSinh)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject() && x[1].object().type == $.Real)
		r = Asinh(cast<Real>(x[1]));
}
CAPI void CVALUE(System, ArcTanh)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject() && x[1].object().type == $.Real)
		r = Atanh(cast<Real>(x[1]));
}
CAPI void CVALUE(System, Gamma)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject() && x[1].object().type == $.Real)
		r = Gamma(cast<Real>(x[1]));
}
CAPI void CVALUE(System, LogGamma)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject() && x[1].object().type == $.Real)
		r = Lngamma(cast<Real>(x[1]));
}
CAPI void CVALUE(System, Zeta)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject() && x[1].object().type == $.Real)
		r = Zeta(cast<Real>(x[1]));
}
CAPI void CVALUE(System, Erfc)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject() && x[1].object().type == $.Real)
		r = Erfc(cast<Real>(x[1]));
}
CAPI void CVALUE(System, Sqrt)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[1].isObject() && x[1].object().type == $.Real)
		r = Sqrtf(cast<Real>(x[1]));
}
