#include <mU/Numeric.h>
#include <mpfr.h>
#ifdef _MSC_VER
#pragma comment(lib,"../Lib/mpfr.lib")
#endif

namespace {
inline mpfr_ptr mpfr_new(long prec) {
    mpfr_ptr r = new __mpfr_struct;
	mpfr_init2(r, prec);
    return r;
}
inline mpfr_ptr mpfr_new(mpf_srcptr x) {
    mpfr_ptr r = mpfr_new(mpf_get_prec(x));
    mpfr_set_f(r, x, GMP_RNDD);
    return r;
}
inline void mpfr_delete(mpfr_ptr x) {
    mpfr_clear(x);
    delete x;
}
inline void mpfr2mpf(mpf_ptr r, mpfr_srcptr x) {
	mpfr_get_f(r, x, GMP_RNDD);
}
}
namespace mU {
namespace Numeric {
#define M(name,f)\
void name(Real& r)\
{\
	mpfr_ptr rr = mpfr_new(mpf_get_prec(r.mpf));\
	f(rr,GMP_RNDD);\
	mpfr2mpf(r.mpf, rr);\
	mpfr_delete(rr);\
}

M(ConstPi, mpfr_const_pi)
M(ConstEuler, mpfr_const_euler)
M(ConstCatalan, mpfr_const_catalan)
#undef M
#define M(name,f)\
void name(Real& r, const Real& x)\
{\
	mpfr_ptr xr = mpfr_new(x.mpf);\
	mpfr_ptr rr = mpfr_new(mpf_get_prec(r.mpf));\
	f(rr,xr,GMP_RNDD);\
	mpfr2mpf(r.mpf, rr);\
	mpfr_delete(rr);\
	mpfr_delete(xr);\
}\
}\
}\
using namespace mU;\
CAPI void VALUE(name)(Kernel& k, var& r, Tuple& x) {\
	if (x.size > 1 && x[1].isObject($.Real)) {\
		r = new Real(cast<Real>(x[1]).prec());\
		Numeric::name(cast<Real>(r), cast<Real>(x[1]));\
	}\
}\
namespace mU {\
namespace Numeric {

M(Log, mpfr_log)
M(Log2, mpfr_log2)
M(Log10, mpfr_log10)
M(Exp, mpfr_exp)
M(Exp2, mpfr_exp2)
M(Exp10, mpfr_exp10)
M(Cos, mpfr_cos)
M(Sin, mpfr_sin)
M(Tan, mpfr_tan)
M(Sec, mpfr_sec)
M(Csc, mpfr_csc)
M(Cot, mpfr_cot)
M(ArcCos, mpfr_acos)
M(ArcSin, mpfr_asin)
M(ArcTan, mpfr_atan)
M(Cosh, mpfr_cosh)
M(Sinh, mpfr_sinh)
M(Tanh, mpfr_tanh)
M(Sech, mpfr_sech)
M(Csch, mpfr_csch)
M(Coth, mpfr_coth)
M(ArcCosh, mpfr_acosh)
M(ArcSinh, mpfr_asinh)
M(ArcTanh, mpfr_atanh)
M(Gamma, mpfr_gamma)
M(LogGamma, mpfr_lngamma)
M(Zeta, mpfr_zeta)
M(Erf, mpfr_erf)
M(Erfc, mpfr_erfc)
M(Sqrt, mpfr_sqrt)
#undef M
#define M(name,f)\
void name(Real& r, const Real& x, const Real& y)\
{\
	mpfr_ptr xr = mpfr_new(x.mpf);\
	mpfr_ptr yr = mpfr_new(y.mpf);\
	mpfr_ptr rr = mpfr_new(mpf_get_prec(r.mpf));\
	f(rr,xr,yr,GMP_RNDD);\
	mpfr2mpf(r.mpf, rr);\
	mpfr_delete(rr);\
	mpfr_delete(yr);\
	mpfr_delete(xr);\
}
M(Powf, mpfr_pow)
#undef M
}
}

using namespace mU;

/*
CAPI void VALUE(Pi)(Kernel& k, var& r, Tuple& x) {
	r = Pi();
}
CAPI void VALUE(EulerGamma)(Kernel& k, var& r, Tuple& x) {
	r = Euler();
}
CAPI void VALUE(Catalan)(Kernel& k, var& r, Tuple& x) {
	r = Catalan();
}
CAPI void VALUE(E)(Kernel& k, var& r, Tuple& x) {
	r = new Real(1L);
	r = Exp(cast<Real>(r));
}
*/