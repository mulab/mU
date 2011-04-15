#include <mU/Number.h>
#include <mU/Kernel.h>

namespace mU {
//////////////////////////////////////
var Evalf(Var x)
{
	switch(Type(x))
	{
	case TYPE(int):
		{
			var r = Flt();
			F::SetZ(r,x);
			return r;
		}
		break;
	case TYPE(rat):
		{
			var r = Flt();
			F::SetQ(r,x);
			return r;
		}
		break;
	case TYPE(flt):
		{
			var r = Flt();
			F::Set(r,x);
			mpf_set_prec(CFlt(r),mpf_get_default_prec());
			return r;
		}
		break;
	case TYPE(sym):
		{
			std::map<Var,attr_t>::const_iterator
				iter = Attributes.find(x);
			if(iter != Attributes.end() &&
				iter->second.count(Constant))
				return CProcs[x](0);
		}
		break;
	case TYPE(vec):
		{
			size_t n = Size(x);
			var r = Vec(n);
			for(size_t i = 0; i < n; ++i)
				At(r,i) = Evalf(At(x,i));
			return r;
		}
		break;
	case TYPE(ex):
		{
			var b = Evalf(Body(x));
			return Eval(Ex(Head(x),b));
		}
		break;
	}
	return x;
}
var Evalf(Var x, size_t y)
{
	size_t z = mpf_get_default_prec();
	mpf_set_default_prec((uint)(LOG_2_10 * y));
	var r = Evalf(x);
	mpf_set_default_prec(z);
	if(FltQ(r)) mpf_set_prec(CFlt(r),(uint)(LOG_2_10 * y));
	return r;
}
var IntegerPart(Var x)
{
	switch(Type(x))
	{
	case TYPE(int): return x;
	case TYPE(rat):
		{
			var r = Int();
			Z::SetQ(r,x);
			return r;
		}
		break;
	case TYPE(flt):
		{
			var r = Int();
			Z::SetF(r,x);
			return r;
		}
		break;
	}
	return x;
}
var Floor(Var x)
{
	return IntegerPart(x);
}
var Ceiling(Var x)
{
	return IntegerPart(x);
}
var Round(Var x)
{
	return IntegerPart(x);
}
//////////////////////////////////////
}
