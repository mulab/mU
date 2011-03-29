#include <mU/Number.h>
#include <mU/Kernel.h>

namespace mU {
//////////////////////////////////////
var Plus(Var x)
{
	size_t n = Size(x);
	if(n == 0) return Int(0L);
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
			c = N::Add(c,At(x,1));
			for(size_t j = 2; j < i; ++j)
				c = N::AddTo(c,At(x,j));
		}
		c = N::Canonicalize(c);
		if(i == n) return c;
		if(!ZeroQ(c))
			Push(r,c);
		pos = i;
	}
	typedef std::multimap<var,var,Less2> coe_t;
	coe_t coe;
	{
		for(; pos < n; ++pos)
		{
			var b = At(x,pos), e;
			if(Tag(b) == TAG(Times))
			{
				b = Body(b);
				if(NumQ(At(b,0)))
				{
					e = At(b,0);
					size_t n = Size(b);
					if(n == 2)
						b = At(b,1);
					else
					{
						var t = b;
						b = Vec(n - 1);
						for(size_t i = 1; i < n; ++i)
							At(b,i - 1) = At(t,i);
					}

				}
				else
					e = Int(1L);
			}
			else
				e = Int(1L);
			coe.insert(std::make_pair(b,e));
		}
	}
	typedef coe_t::iterator iter_t;
	{
		iter_t iter = coe.begin();
		while(iter != coe.end())
		{
			var b = iter->first, e = iter->second;
			iter_t end = coe.upper_bound(b);
			++iter;
			if(iter != end)
			{
				e = N::Add(e,iter->second);
				for(++iter; iter != end; ++iter)
					e = N::AddTo(e,iter->second);
			}
			if(ZeroQ(e))
				continue;
			if(OneQ(e))
			{ 
				if(VecQ(b)) 
					b = Ex(TAG(Times),b); 
			}
			else
			{
				if(VecQ(b))
				{
					size_t n = Size(b);
					var t = b;
					b = Vec(n + 1);
					At(b,0) = e;
					for(size_t i = 0; i < n; ++i)
						At(b,i + 1) = At(t,i);
				}
				else
					b = Vec(e,b);
				b = Ex(TAG(Times),b);
			}
			Push(r,b);		
		}
	}
	n = Size(r);
	if(n == 0) return Int(0L);
	if(n == 1) return At(r,0);
	return Ex(TAG(Plus),r);	
}
var Plus(Var x, Var y)
{
	var r = Vec();
	if(Tag(x) == TAG(Plus))
		Flatten(r,TAG(Plus),Body(x));
	else
		Push(r,x);
	if(Tag(y) == TAG(Plus))
		Flatten(r,TAG(Plus),Body(y));
	else
		Push(r,y);
	Sort(r);
	return Plus(r);
}
//////////////////////////////////////
}
