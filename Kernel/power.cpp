#include <mU/Number.h>
#include <mU/Kernel.h>

namespace mU {
//////////////////////////////////////
var Power(Var x, Var y)
{
	if(ZeroQ(x)) return Int(0L);
	if(OneQ(x) || ZeroQ(y)) return Int(1L);
	if(OneQ(y)) return x;
	if(ExQ(x))
	{
		var h = Head(x);
		var b = Body(x);
		if(h == TAG(Power) && IntQ(y))
			return Power(At(b,0),Times(At(b,1),y));
		if(h == TAG(Times))
		{
			size_t n = Size(b);
			var r = Vec(n);
			for(size_t i = 0; i < n; ++i)
				At(r,i) = Power(At(b,i),y);
			Sort(r);
			return Times(r);
		}
	}
	switch(Type(y))
	{
	case TYPE(int):
		{
			sint se = Z::SI(y);
			uint e = se > 0 ? se : -se;
			var r;
			switch(Type(x))
			{
			case TYPE(int):
				if(se > 0)
				{
					r = Int();
					mpz_pow_ui(CInt(r),CInt(x),e);
				}
				else
				{
					r = Rat();
					mpz_set_ui(mpq_numref(CRat(r)),1L);
					mpz_pow_ui(mpq_denref(CRat(r)),CInt(x),e);
				}
				return r;
			case TYPE(rat):
				r = Rat();
				if(se > 0)
				{
					mpz_pow_ui(mpq_numref(CRat(r)),mpq_numref(CRat(x)),e);
					mpz_pow_ui(mpq_denref(CRat(r)),mpq_denref(CRat(x)),e);
				}
				else
				{
					mpz_pow_ui(mpq_numref(CRat(r)),mpq_numref(CRat(x)),e);
					mpz_pow_ui(mpq_denref(CRat(r)),mpq_denref(CRat(x)),e);
				}
				return r;
			case TYPE(flt):
				r = Flt();
				mpf_pow_ui(CFlt(r),CFlt(x),e);
				if(se < 0) mpf_ui_div(CFlt(r),1L,CFlt(r));
				return r;
			}
		}
		break;
	case TYPE(rat):
		{
			switch(Type(x))
			{
			case TYPE(int):
				{
					sint se = mpz_get_si(mpq_numref(CRat(y)));
					uint e = se > 0 ? se : -se;
					var r = Int();
					mpz_pow_ui(CInt(r),CInt(x),e);
					return Eval(Ex(TAG(Radical),Vec(r,
						Int((se > 0 ? 1 : -1) * mpz_get_si(mpq_denref(CRat(y)))))));
				}
				break;
			case TYPE(rat):
				{
					var a = Int(), b = Int();
					Q::Num(a,x); Q::Den(b,x);
					return Times(Power(a,y),Power(Power(b,y),Int(-1L)));
				}
				break;
			case TYPE(flt):
				{
					var r = Flt();
					F::SetQ(r,y);
					return Eval(Ex(TAG(Pow),Vec(x,r)));
				}
				break;
			}
		}
		break;
	case TYPE(flt):
		{
			switch(Type(x))
			{
			case TYPE(int):
				{
					var r = Flt();
					F::SetZ(r,x);
					return Eval(Ex(TAG(Pow),Vec(r,y)));
				}
				break;
			case TYPE(rat):
				{
					var r = Flt();
					F::SetQ(r,x);
					return Eval(Ex(TAG(Pow),Vec(r,y)));
				}
				break;
			case TYPE(flt): return Eval(Ex(TAG(Pow),Vec(x,y)));
			}
		}
		break;
	}
	return Ex(TAG(Power),Vec(x,y));
}
var Mod(Var x, Var y)
{
	if(ZeroQ(x) || ZeroQ(y)) return Int(0L);
	if(IntQ(x) && IntQ(y))
	{
		var r = Int();
		Z::Mod(r,x,y);
		return r;
	}
	return Ex(TAG(Mod),Vec(x,y));
}
//////////////////////////////////////
}
