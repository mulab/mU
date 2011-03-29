#include <mU/Number.h>
#include <mU/Kernel.h>

namespace mU {
//////////////////////////////////////
void Expand(Var r, Var x, Var y)
{
	if(Tag(x) == TAG(Plus))
	{
		x = Body(x);
		size_t m = Size(x);
		if(Tag(y) == TAG(Plus))
		{
			y = Body(y);
			size_t n = Size(y);
			for(size_t i = 0; i < m; ++i)
				for(size_t j = 0; j < n; ++j)
					Push(r,Times(At(x,i),At(y,j)));
		}
		else
			for(size_t i = 0; i < m; ++i)
				Push(r,Times(At(x,i),y)); 
	}
	else if(Tag(y) == TAG(Plus))
	{
		y = Body(y);
		size_t n = Size(y);
		for(size_t j = 0; j < n; ++j)
			Push(r,Times(x,At(y,j))); 
	}
	else
		Push(r,Times(x,y));
}
var Expand(Var x, Var y)
{
	var r = Vec();
	Expand(r,x,y);
	Sort(r);
	return Plus(r);
}
var Expand(Var x)
{
	if(VecQ(x))
	{
		size_t n = Size(x);
		var r = Vec(n);
		for(size_t i = 0; i < n; ++i)
			At(r,i) = Expand(At(x,i)); 
		return r;
	}
	else if(ExQ(x))
	{
		var h = Head(x);
		var b = Body(x);
		size_t n = Size(b);
		if(h == TAG(Times))
		{
			typedef std::priority_queue<var,std::vector<var>,After2> heap_t;
			heap_t heap;
			for(size_t i = 0; i < n; ++i)
				heap.push(Expand(At(b,i)));
			while(heap.size() != 1)
			{
				var t[2];
				t[0] = heap.top(); heap.pop();
				t[1] = heap.top(); heap.pop();
				heap.push(Expand(t[0],t[1]));
			}
			return heap.top();
		}
		else if(h == TAG(Power))
		{
			var c = At(b,1);
			b = At(b,0);
			if(IntQ(c))
			{
				sint se = Z::SI(c);
				uint e = se > 0 ? se : -se;
				var r = Expand(b), t = r; --e;
				while(e > 0)
				{
					if(e & 1) r = Expand(r,t);
					if(e > 1) t = Expand(t,t);
					e >>= 1;
				}
				return se > 0 ? r : Power(r,Int(-1L));
			}
			else if(RatQ(c))
			{
				var d = Int();
				Z::SetQ(d,c);
				var e = Rat();
				Q::SetZ(e,d);
				Q::Sub(e,c,e);
				return Expand(Expand(Power(b,d)),Power(b,e));
			}
		}
		else
		{
			var r = Vec(n);
			for(size_t i = 0; i < n; ++i)
				At(r,i) = Expand(At(b,i)); 
			if(h == TAG(Plus))
			{
				var t = r;
				r = Vec();
				Flatten(r,h,t);
				Sort(r);
				return Plus(r);
			}
			return Ex(h,r);
		}
	}
	return x;
}
//////////////////////////////////////
}
