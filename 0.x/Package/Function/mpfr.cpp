#include <mU/Function.h>
#include "mpfr.h"
#ifdef _MSC_VER
#pragma comment(lib,"../../Lib/mpfr.lib")
#endif

void pi_chudnovsky(mpf_ptr,long int);

namespace {
inline mpf_ptr mpf_new()
{
	mpf_ptr r = new __mpf_struct;
	mpf_init(r);
	return r;
}
inline mpf_ptr mpf_new(double x)
{
	mpf_ptr r = mpf_new();
	mpf_set_d(r,x);
	return r;
}
inline mpf_ptr mpf_new(mpf_ptr x)
{
	mpf_ptr r = mpf_new();
	mpf_set(r,x);
	return r;
}
inline void mpf_delete(mpf_ptr x)
{
	mpf_clear(x);
	delete x;
}
inline mpf_ptr mpf_clone(mpf_ptr x)
{
	mpf_ptr r = new __mpf_struct;
	*r = *x;
	return r;
}
inline mpf_ptr mpf_new(mpfr_ptr x)
{
	mpf_ptr r = mpf_new();
	mpfr_get_f(r,x,GMP_RNDD);
	return r;
}
inline mpfr_ptr mpfr_new()
{
	mpfr_ptr r = new __mpfr_struct;
	mpfr_init2(r,mpf_get_default_prec());
	return r;
}
inline mpfr_ptr mpfr_new(mpf_ptr x)
{
	mpfr_ptr r = mpfr_new();
	mpfr_set_f(r,x,GMP_RNDD);
	return r;
}
inline void mpfr_delete(mpfr_ptr x)
{
	mpfr_clear(x);
	delete x;
}
}
namespace mU {
namespace Function {
#define DEF(name,f)\
var name()\
{\
	mpfr_ptr rr = mpfr_new();\
	f(rr,GMP_RNDD);\
	mpf_ptr r = mpf_new(rr);\
	mpfr_delete(rr);\
	var t = Flt();\
	*CFlt(t) = *r;\
	return t;\
}

DEF(Euler,mpfr_const_euler)
DEF(Catalan,mpfr_const_catalan)
#undef DEF
#define DEF(name,f)\
var name(Var x)\
{\
	mpfr_ptr rr = mpfr_new();\
	mpfr_ptr xr = mpfr_new(CFlt(x));\
	f(rr,xr,GMP_RNDD);\
	mpf_ptr r = mpf_new(rr);\
	mpfr_delete(xr);\
	mpfr_delete(rr);\
	var t = Flt();\
	*CFlt(t) = *r;\
	return t;\
}

DEF(Log,mpfr_log)
DEF(Log2,mpfr_log2)
DEF(Log10,mpfr_log10)
DEF(Exp,mpfr_exp)
DEF(Exp2,mpfr_exp2)
DEF(Exp10,mpfr_exp10)
DEF(Cos,mpfr_cos)
DEF(Sin,mpfr_sin)
DEF(Tan,mpfr_tan)
DEF(Sec,mpfr_sec)
DEF(Csc,mpfr_csc)
DEF(Cot,mpfr_cot)
DEF(Acos,mpfr_acos)
DEF(Asin,mpfr_asin)
DEF(Atan,mpfr_atan)
DEF(Cosh,mpfr_cosh)
DEF(Sinh,mpfr_sinh)
DEF(Tanh,mpfr_tanh)
DEF(Sech,mpfr_sech)
DEF(Csch,mpfr_csch)
DEF(Coth,mpfr_coth)
DEF(Acosh,mpfr_acosh)
DEF(Asinh,mpfr_asinh)
DEF(Atanh,mpfr_atanh)
DEF(Gamma,mpfr_gamma)
DEF(Lngamma,mpfr_lngamma)
DEF(Zeta,mpfr_zeta)
DEF(Erf,mpfr_erf)
DEF(Erfc,mpfr_erfc)
DEF(Sqrtf,mpfr_sqrt)
#undef DEF
#define DEF(name,f)\
var name(Var x, Var y)\
{\
	mpfr_ptr rr = mpfr_new();\
	mpfr_ptr xr = mpfr_new(CFlt(x));\
	mpfr_ptr yr = mpfr_new(CFlt(y));\
	f(rr,xr,yr,GMP_RNDD);\
	mpf_ptr r = mpf_new(rr);\
	mpfr_delete(xr);\
	mpfr_delete(yr);\
	mpfr_delete(rr);\
	var t = Flt();\
	*CFlt(t) = *r;\
	return t;\
}
DEF(Powf,mpfr_pow)
#undef DEF
var Pif()
{
	var r = Flt();
	pi_chudnovsky(CFlt(r),mpf_get_default_prec());
	return r;
}
}
}