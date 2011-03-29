#include <mU/Number.h>

namespace mU {
//////////////////////////////////////
namespace Z {
#define T(x) (((int_t*)(Var)(x))->rep)
void Set(Var x, uint y)
{
	mpz_set_ui(T(x),y);
}
void Set(Var x, sint y)
{
	mpz_set_si(T(x),y);
}
void Set(Var x, double y)
{
	mpz_set_d(T(x),y);
}
void Set(Var x, const char *y, int z)
{
	mpz_set_str(T(x),y,z);
}
void Set(Var x, Var y)
{
	mpz_set(T(x),T(y));
}
void SetQ(Var x, Var y)
{
	mpz_set_q(T(x),(mpq_ptr)T(y));
}
void SetF(Var x, Var y)
{
	mpz_set_f(T(x),(mpf_ptr)T(y));
}
void Abs(Var x, Var y)
{
	mpz_abs(T(x),T(y));
}
void Neg(Var x, Var y)
{
	mpz_neg(T(x),T(y));
}
void Com(Var x, Var y)
{
	mpz_com(T(x),T(y));
}
void And(Var x, Var y, Var z)
{
	mpz_and(T(x),T(y),T(z));
}
void Ior(Var x, Var y, Var z)
{
	mpz_ior(T(x),T(y),T(z));
}
void Xor(Var x, Var y, Var z)
{
	mpz_xor(T(x),T(y),T(z));
}
void Add(Var x, Var y, Var z)
{
	mpz_add(T(x),T(y),T(z));
}
void Add(Var x, Var y, uint z)
{
	mpz_add_ui(T(x),T(y),z);
}
void Sub(Var x, Var y, Var z)
{
	mpz_sub(T(x),T(y),T(z));
}
void Sub(Var x, Var y, uint z)
{
	mpz_sub_ui(T(x),T(y),z);
}
void Sub(Var x, uint y, Var z)
{
	mpz_ui_sub(T(x),y,T(z));
}
void Mul(Var x, Var y, Var z)
{
	mpz_mul(T(x),T(y),T(z));
}
void Mul(Var x, Var y, uint z)
{
	mpz_mul_ui(T(x),T(y),z);
}
void Mul(Var x, Var y, int z)
{
	mpz_mul_si(T(x),T(y),z);
}
void Div(Var x, Var y, Var z)
{
	mpz_div(T(x),T(y),T(z));
}
void Div(Var x, Var y, uint z)
{
	mpz_div_ui(T(x),T(y),z);
}
void Mod(Var x, Var y, Var z)
{
	mpz_mod(T(x),T(y),T(z));
}
uint Mod(Var x, uint y)
{
	return mpz_fdiv_ui(T(x),y);
}
void Pow(Var x, Var y, uint z)
{
	mpz_pow_ui(T(x),T(y),z);
}
void Pow(Var x, uint y, uint z)
{
	mpz_ui_pow_ui(T(x),y,z);
}
void Inv(Var x, Var y, Var z)
{
	mpz_invert(T(x),T(y),T(z));
}
int Sgn(Var x)
{
	return mpz_sgn(T(x));
}
int Cmp(Var x, Var y)
{
	return mpz_cmp(T(x),T(y));
}
int Cmp(Var x, uint y)
{
	return mpz_cmp_ui(T(x),y);
}
int Cmp(Var x, int y)
{
	return mpz_cmp_si(T(x),y);
}
int Cmp(Var x, double y)
{
	return mpz_cmp_d(T(x),y);
}
uint UI(Var x)
{
	return mpz_get_ui(T(x));
}
sint SI(Var x)
{
	return mpz_get_si(T(x));
}
double D(Var x)
{
	return mpz_get_d(T(x));
}
#undef T
}
namespace Q {
#define T(x) (((rat_t*)(x))->rep)
void Set(Var x, uint y, uint z)
{
	mpq_set_ui(T(x),y,z);
}
void Set(Var x, sint y, uint z)
{
	mpq_set_si(T(x),y,z);
}
void Set(Var x, double y)
{
	mpq_set_d(T(x),y);
}
void Set(Var x, const char *y, int z)
{
	mpq_set_str(T(x),y,z);
}
void Set(Var x, Var y)
{
	mpq_set(T(x),T(y));
}
void SetZ(Var x, Var y)
{
	mpq_set_z(T(x),(mpz_ptr)T(y));
}
void SetZ(Var x, Var y, Var z)
{
	mpq_set_num(T(x),(mpz_ptr)T(y));
	mpq_set_den(T(x),(mpz_ptr)T(z));
}
void SetNum(Var x, Var y)
{
	mpq_set_num(T(x),(mpz_ptr)T(y));
}
void SetDen(Var x, Var y)
{
	mpq_set_den(T(x),(mpz_ptr)T(y));
}
void SetF(Var x, Var y)
{
	mpq_set_f(T(x),(mpf_ptr)T(y));
}
void Num(Var x, Var y)
{
	mpq_get_num((mpz_ptr)T(x),T(y));
}
void Den(Var x, Var y)
{
	mpq_get_den((mpz_ptr)T(x),T(y));
}
void Canonicalize(Var x)
{
	mpq_canonicalize(T(x));
}
void Abs(Var x, Var y)
{
	mpq_abs(T(x),T(y));
}
void Neg(Var x, Var y)
{
	mpq_neg(T(x),T(y));
}
void Add(Var x, Var y, Var z)
{
	mpq_add(T(x),T(y),T(z));
}
void Sub(Var x, Var y, Var z)
{
	mpq_sub(T(x),T(y),T(z));
}
void Mul(Var x, Var y, Var z)
{
	mpq_mul(T(x),T(y),T(z));
}
void Div(Var x, Var y, Var z)
{
	mpq_div(T(x),T(y),T(z));
}
int Sgn(Var x)
{
	return mpq_sgn(T(x));
}
int Cmp(Var x, Var y)
{
	return mpq_cmp(T(x),T(y));
}
int Cmp(Var x, uint y, uint z)
{
	return mpq_cmp_ui(T(x),y,z);
}
int Cmp(Var x, int y, uint z)
{
	return mpq_cmp_si(T(x),y,z);
}
double D(Var x)
{
	return mpq_get_d(T(x));
}
#undef T
}
namespace F {
#define T(x) (((flt_t*)(x))->rep)
void Set(Var x, uint y)
{
	mpf_set_ui(T(x),y);
}
void Set(Var x, int y)
{
	mpf_set_si(T(x),y);
}
void Set(Var x, double y)
{
	mpf_set_d(T(x),y);
}
void Set(Var x, const char *y, int z)
{
	mpf_set_str(T(x),y,z);
}
void Set(Var x, Var y)
{
	mpf_set(T(x),T(y));
}
void SetZ(Var x, Var y)
{
	mpf_set_z(T(x),(mpz_ptr)T(y));
}
void SetQ(Var x, Var y)
{
	mpf_set_q(T(x),(mpq_ptr)T(y));
}
void Abs(Var x, Var y)
{
	mpf_abs(T(x),T(y));
}
void Neg(Var x, Var y)
{
	mpf_neg(T(x),T(y));
}
void Add(Var x, Var y, Var z)
{
	mpf_add(T(x),T(y),T(z));
}
void Sub(Var x, Var y, Var z)
{
	mpf_sub(T(x),T(y),T(z));
}
void Mul(Var x, Var y, Var z)
{
	mpf_mul(T(x),T(y),T(z));
}
void Div(Var x, Var y, Var z)
{
	mpf_div(T(x),T(y),T(z));
}
void Pow(Var x, Var y, uint z)
{
	mpf_pow_ui(T(x),T(y),z);
}
int Sgn(Var x)
{
	return mpf_sgn(T(x));
}
int Cmp(Var x, Var y)
{
	return mpf_cmp(T(x),T(y));
}
int Cmp(Var x, uint y)
{
	return mpf_cmp_ui(T(x),y);
}
int Cmp(Var x, int y)
{
	return mpf_cmp_si(T(x),y);
}
int Cmp(Var x, double y)
{
	return mpf_cmp_d(T(x),y);
}
uint UI(Var x)
{
	return mpf_get_ui(T(x));
}
sint SI(Var x)
{
	return mpf_get_si(T(x));
}
double D(Var x)
{
	return mpf_get_d(T(x));
}
#undef T
}
namespace N
{
#define DEF(f)\
var f(Var x)\
{\
	var r;\
	switch(Type(x))\
	{\
	case TYPE(int):\
		r = Int();\
		Z::f(r,x);\
		break;\
	case TYPE(rat):\
		r = Rat();\
		Q::f(r,x);\
		break;\
	case TYPE(flt):\
		r = Flt();\
		F::f(r,x);\
		break;\
	}\
	return r;\
}

DEF(Abs)
DEF(Neg)
#undef DEF
#define DEF(f)\
var f##To(Var x)\
{\
	switch(Type(x))\
	{\
	case TYPE(int): Z::f(x,x); break;\
	case TYPE(rat): Q::f(x,x); break;\
	case TYPE(flt): F::f(x,x); break;\
	}\
	return x;\
}

DEF(Abs)
DEF(Neg)
#undef DEF
#define DEF(f)\
var f(Var x, Var y)\
{\
	var r;\
	switch(Type(x))\
	{\
	case TYPE(int):\
		switch(Type(y))\
		{\
		case TYPE(int):\
			r = Int();\
			Z::f(r,x,y);\
			break;\
		case TYPE(rat):\
			r = Rat();\
			Q::SetZ(r,x);\
			Q::f(r,r,y);\
			break;\
		case TYPE(flt):\
			r = Flt();\
			F::SetZ(r,x);\
			F::f(r,r,y);\
			break;\
		}\
		break;\
	case TYPE(rat):\
		switch(Type(y))\
		{\
		case TYPE(int):\
			r = Rat();\
			Q::SetZ(r,y);\
			Q::f(r,x,r);\
			break;\
		case TYPE(rat):\
			r = Rat();\
			Q::f(r,x,y);\
			break;\
		case TYPE(flt):\
			r = Flt();\
			F::SetQ(r,x);\
			F::f(r,r,y);\
			break;\
		}\
		break;\
	case TYPE(flt):\
		switch(Type(y))\
		{\
		case TYPE(int):\
			r = Flt();\
			F::SetZ(r,y);\
			F::f(r,x,r);\
			break;\
		case TYPE(rat):\
			r = Flt();\
			F::SetQ(r,y);\
			F::f(r,x,r);\
			break;\
		case TYPE(flt):\
			r = Flt();\
			F::f(r,x,y);\
			break;\
		}\
		break;\
	}\
	return r;\
}

DEF(Add)
DEF(Sub)
DEF(Mul)
DEF(Div)
#undef DEF
#define DEF(f)\
var f##To(Var x, Var y)\
{\
	var r;\
	switch(Type(x))\
	{\
	case TYPE(int):\
		switch(Type(y))\
		{\
		case TYPE(int):\
			r = x;\
			Z::f(r,x,y);\
			break;\
		case TYPE(rat):\
			r = Rat();\
			Q::SetZ(r,x);\
			Q::f(r,r,y);\
			break;\
		case TYPE(flt):\
			r = Flt();\
			F::SetZ(r,x);\
			F::f(r,r,y);\
			break;\
		}\
		break;\
	case TYPE(rat):\
		switch(Type(y))\
		{\
		case TYPE(int):\
			r = Rat();\
			Q::SetZ(r,y);\
			Q::f(r,x,r);\
			break;\
		case TYPE(rat):\
			r = x;\
			Q::f(r,x,y);\
			break;\
		case TYPE(flt):\
			r = Flt();\
			F::SetQ(r,x);\
			F::f(r,r,y);\
			break;\
		}\
		break;\
	case TYPE(flt):\
		switch(Type(y))\
		{\
		case TYPE(int):\
			r = Flt();\
			F::SetZ(r,y);\
			F::f(r,x,r);\
			break;\
		case TYPE(rat):\
			r = Flt();\
			F::SetQ(r,y);\
			F::f(r,x,r);\
			break;\
		case TYPE(flt):\
			r = x;\
			F::f(r,x,y);\
			break;\
		}\
		break;\
	}\
	return r;\
}

DEF(Add)
DEF(Sub)
DEF(Mul)
DEF(Div)
#undef DEF
int Cmp(Var x, Var y)
{
	switch(Type(x))
	{
	case TYPE(int):
		switch(Type(y))
		{
		case TYPE(int): return Z::Cmp(x,y);
		case TYPE(rat):
		{
			var r = Rat();
			Q::SetZ(r,x);
			return Q::Cmp(r,y);
		}
		case TYPE(flt):
		{
			var r = Flt();
			F::SetZ(r,x);
			return F::Cmp(r,y);
		}
		}
		break;
	case TYPE(rat):
		switch(Type(y))
		{
		case TYPE(int):
		{
			var r = Rat();
			Q::SetZ(r,y);
			return Q::Cmp(x,r);
		}
		case TYPE(rat): return Q::Cmp(x,y);
		case TYPE(flt):
		{
			var r = Flt();
			F::SetQ(r,x);
			return F::Cmp(r,y);
		}
		}
		break;
	case TYPE(flt):
		switch(Type(y))
		{
		case TYPE(int):
		{
			var r = Flt();
			F::SetZ(r,y);
			return F::Cmp(x,r);
		}
		case TYPE(rat):
		{
			var r = Flt();
			F::SetQ(r,y);
			return F::Cmp(x,r);
		}
		case TYPE(flt): return F::Cmp(x,y);
		}
		break;
	}
	return 0;
}
int Sgn(Var x)
{
	switch(Type(x))
	{
	case TYPE(int): return Z::Sgn(x);
	case TYPE(rat): return Q::Sgn(x);
	case TYPE(flt): return F::Sgn(x);
	}
	return 0;
}
double D(Var x)
{
	switch(Type(x))
	{
	case TYPE(int): return Z::D(x);
	case TYPE(rat): return Q::D(x);
	case TYPE(flt): return F::D(x);
	}
	return 0.0;
}
var Canonicalize(Var x)
{
	/*
	switch(Type(x))
	{
	case TYPE(rat):
		{
			Q::Canonicalize(x);
			if(mpz_cmp_ui(mpq_denref(CRat(x)),1) == 0)
			{
				var r = Int();
				Q::Num(r,x);
				return r;
			}
		}
		return x;
	case TYPE(flt):
		if(mpf_integer_p(CFlt(x)))
		{
			var r = Int();
			Z::SetF(r,x);
			return r;
		}
	}
	*/
	if(Type(x) == TYPE(rat))
	{
		Q::Canonicalize(x);
		if(mpz_cmp_ui(mpq_denref(CRat(x)),1) == 0)
		{
			var r = Int();
			Q::Num(r,x);
			return r;
		}
	}
	return x;
}
}
//////////////////////////////////////
}
