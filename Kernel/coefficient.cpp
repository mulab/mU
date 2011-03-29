#include <mU/Number.h>
#include <mU/Kernel.h>

namespace mU {
//////////////////////////////////////
typedef std::set<var,Less2> set_t;
void Variables(set_t &s, Var x)
{
	if(SymQ(x))
		s.insert(x);
	else if(VecQ(x))
	{
		size_t n = Size(x);
		for(size_t i = 0; i < n; ++i)
			Variables(s,At(x,i));
	}
	else if(ExQ(x))
	{
		var h = Head(x);
		var b = Body(x);
		size_t n = Size(b);
		if(h == TAG(Plus) || h == TAG(Times))
			Variables(s,b);
		else if(h == TAG(Power) && NumQ(At(b,1)))
			Variables(s,At(b,0));
	}
}
var Variables(Var x)
{
	set_t s;
	Variables(s,x);
	var r = Vec();
	CVec(r).reserve(s.size());
	std::copy(s.begin(),s.end(),std::back_inserter(CVec(r)));
	return r;
}
void Coefficient(Var r, Var x, Var y)
{
	if(Same(x,y))
		Push(r,Int(1L));
	else if(ExQ(x))
	{
		var h = Head(x);
		var b = Body(x);
		size_t n = Size(b);
		if(h == TAG(Plus))
		{
			for(size_t i = 0; i < n; ++i)
				Push(r,Coefficient(At(b,i),y));
		}
		else if(h == TAG(Times))
		{
			size_t m = 0;
			for(; m < n && !Same(At(b,m),y); ++m);
			if(m != n)
			{
				var t = b;
				b = Vec(n - 1);
				for(size_t i = 0; i < m; ++i)
					At(b,i) = At(t,i);
				for(size_t i = m + 1; i < n; ++i)
					At(b,i - 1) = At(t,i);
				Push(r,n == 2 ? At(b,0) : Ex(h,b));
			}
		}
	}
}
var Coefficient(Var x, Var y)
{
	var r = Vec();
	Coefficient(r,x,y);
	Sort(r);
	return Plus(r);
}
var Exponent(Var x, Var y)
{
	if(x == y) return Int(1L);
	if(ExQ(x,TAG(Power)) && At(Body(x),0) == y)
		return At(Body(x),1);
	return Int(0L);
}
var Deg(Var x, Var y)
{
	if(ExQ(x))
	{
		var h = Head(x);
		if(h == TAG(Plus) || h == TAG(Times))
		{
			var b = Body(x);
			size_t n = Size(b);
			var r = Deg(At(b,0),y);
			for(size_t i = 1; i < n; ++i)
				r = std::max<var>(r,Deg(At(b,i),y),Less);
			return r;
		}
	}
	return Exponent(x,y);
}
typedef std::multimap<size_t,var> coe_t;
void Coefficients(coe_t &c, Var x, Var y)
{
	var e = Exponent(x,y);
	if(IntQ(e) && Z::Sgn(e) > 0)
	{
		c.insert(std::make_pair(Z::UI(e),Int(1L)));
		return;
	}
	if(ExQ(x))
	{
		var h = Head(x);
		var b = Body(x);
		size_t n = Size(b);
		if(h == TAG(Plus))
		{
			for(size_t i = 0; i < n; ++i)
				Coefficients(c,At(b,i),y);
			return;
		}
		else if(h == TAG(Times))
		{
			size_t m = 0;
			for(; m < n; ++m)
			{
				e = Exponent(At(b,m),y);
				if(IntQ(e) && Z::Sgn(e) > 0)
					break;
			}
			if(m != n)
			{
				var t = b;
				b = Vec(n - 1);
				for(size_t i = 0; i < m; ++i)
					At(b,i) = At(t,i);
				for(size_t i = m + 1; i < n; ++i)
					At(b,i - 1) = At(t,i);
				c.insert(std::make_pair(Z::UI(e),
					n == 2 ? At(b,0) : Ex(h,b)));
				return;
			}
		}
	}
	c.insert(std::make_pair(0,x));
}
var Coefficients(Var x, Var y)
{
	coe_t c;
	Coefficients(c,x,y);
	size_t n = c.rbegin()->first + 1;
	var r = Vec(n);
	coe_t::iterator iter = c.begin();
	while(iter != c.end())
	{
		size_t e = iter->first;
		coe_t::iterator end = c.upper_bound(e);
		var t = Vec();
		for(; iter != end; ++iter)
			Push(t,iter->second);
		Sort(t);
		At(r,e) = Plus(t);
	}
	for(size_t i = 0; i < n; ++i)
		if(!At(r,i))
			At(r,i) = Int(0L);
	return r;
}
var CoefficientList(Var x, vec_t::rep_t::const_iterator a, vec_t::rep_t::const_iterator b)
{
	var r = Coefficients(x,*a);
	size_t n = Size(r);
	if(++a != b)
		for(size_t i = 0; i < n; ++i)
			At(r,i) = CoefficientList(At(r,i),a,b);
	return r;
}
var CoefficientList(Var x, Var y)
{
	return CoefficientList(x,CVec(y).begin(),CVec(y).end());
}
var FromCoefficients(Var x, Var y)
{
	size_t n = Size(x);
	var r = Vec();
	if(n > 0)
	{
		var c = At(x,0);
		if(!ZeroQ(c))
			Push(r,c);
		if(n > 1)
		{
			c = At(x,1);
			if(!ZeroQ(c))
				Push(r,OneQ(c) ? var(y) : Ex(TAG(Times),Vec(c,y)));
			for(size_t i = 2; i < n; ++i)
			{
				var c = At(x,i);
				if(ZeroQ(c))
					continue;
				else if(OneQ(c))
					Push(r,Ex(TAG(Power),Vec(y,Int((uint)i))));
				else
					Push(r,Ex(TAG(Times),Vec(c,Ex(TAG(Power),Vec(y,Int((uint)i))))));
			}
		}
	}
	n = Size(r);
	if(n == 0) return Int(0L);
	if(n == 1) return At(r,0);
	return Ex(TAG(Plus),r);
}
var FromCoefficientList(Var x, vec_t::rep_t::const_iterator a, vec_t::rep_t::const_iterator b)
{
	if(++a != b)
	{
		size_t n = Size(x);
		var r = Vec(n);
		for(size_t i = 0; i < n; ++i)
			At(r,i) = FromCoefficientList(At(x,i),a,b);
		return FromCoefficients(r,*--a);
	}
	return FromCoefficients(x,*--a);
}
var FromCoefficientList(Var x, Var y)
{
	return FromCoefficientList(x,CVec(y).begin(),CVec(y).end());
}
//////////////////////////////////////
}
