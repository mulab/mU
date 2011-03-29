#include <mU/Number.h>
#include <mU/Kernel.h>

namespace mU {
//////////////////////////////////////
var Times(Var x)
{
	size_t n = Size(x);
	if(n == 0) return Int(1L);
	if(n == 1) return At(x,0);
	size_t pos = 0;
	var r = Vec();
	if(NumQ(At(x,0)))
	{
		var c = At(x,0);
		size_t i = 1;
		for(; i < n && NumQ(At(x,i)); ++i);
		if(i >= 2)
		{
			c = N::Mul(c,At(x,1));
			for(size_t j = 2; j < i; ++j)
				c = N::MulTo(c,At(x,j));
		}
		c = N::Canonicalize(c);
		if(i == n) return c;
		if(ZeroQ(c))
			return Int(0L);
		if(!OneQ(c))
			Push(r,c);
		pos = i;
	}
	typedef std::multimap<var,var,Less2> coe_t;
	coe_t coe;
	for(; pos < n; ++pos)
	{
		var b = At(x,pos), e;
		if(Tag(b) == TAG(Power))
		{
			e = At(Body(b),1);
			b = At(Body(b),0);
		}
		else
			e = Int(1L);
		coe.insert(std::make_pair(b,e));
	}
	typedef coe_t::iterator iter_t;
	iter_t iter = coe.begin();
	while(iter != coe.end())
	{
		var b = iter->first, e = iter->second;
		iter_t end = coe.upper_bound(b);
		++iter;
		if(iter != end)
		{
			e = Vec(e);
			for(; iter != end; ++iter)
				Push(e,iter->second);
			Sort(e);
			e = Plus(e);
		}
		if(ZeroQ(e))
			continue;
		if(!OneQ(e))
			b = Ex(TAG(Power),Vec(b,e));
		Push(r,b);
	}
	n = Size(r);
	if(n == 0) return Int(1L);
	if(n == 1) return At(r,0);
	if(NOneQ(At(r,0)) && ExQ(At(r,1),TAG(Plus)))
	{
		var c = Int(-1L);
		r = Body(At(r,1));
		n = Size(r);
		var t = Vec(n);
		for(size_t i = 0; i < n; ++i)
			At(t,i) = Times(c,At(r,i));
		return Ex(TAG(Plus),t);
	}
	return Ex(TAG(Times),r);	
}
var Times(Var x, Var y)
{
	var r = Vec();
	if(Tag(x) == TAG(Times))
		Flatten(r,TAG(Times),Body(x));
	else
		Push(r,x);
	if(Tag(y) == TAG(Times))
		Flatten(r,TAG(Times),Body(y));
	else
		Push(r,y);
	Sort(r);
	return Times(r);
}
//////////////////////////////////////
}
