#include <mU/Number.h>
#include <mU/Kernel.h>

namespace mU {
//////////////////////////////////////
var Dot(Var x)
{
	size_t n = Size(x);
	if(n == 0) return Int(1L);
	if(n == 1) return At(x,0);
	var r = Dot(At(x,0),At(x,1));
	for(size_t i = 2; i < n; ++i)
		r = Dot(r,At(x,i));
	return r;
}
var Dot(Var x, Var y)
{
	if(VecQ(x) && VecQ(y))
	{
		size_t m = Size(x), n = Size(y);
		if(VecQ(At(x,0)) && Size(At(x,0)) == n)
		{
			if(VecQ(At(y,0)))
			{
				size_t p = Size(At(y,0));
				var r = Vec(m);
				for(size_t i = 0; i < m; ++i)
				{
					At(r,i) = Vec(p);
					for(size_t j = 0; j < p; ++j)
					{
						var t = Times(At(At(x,i),0),At(At(y,0),j));
						for(size_t k = 1; k < n; ++k)
							t = Plus(t,Times(At(At(x,i),k),At(At(y,k),j)));
						At(At(r,i),j) = t;
					}
				}
				return r;
			}
			else
			{
				var r = Vec(m);
				for(size_t i = 0; i < m; ++i)
					At(r,i) = Dot(At(x,i),y);
				return r;
			}
		}
		else if(m == n)
		{
			if(VecQ(At(y,0)))
			{
				size_t p = Size(At(y,0));
				var r = Vec(p);
				for(size_t i = 0; i < p; ++i)
				{
					var t = Times(At(x,0),At(At(y,0),i));
					for(size_t j = 1; j < m; ++j)
						t = Plus(t,Times(At(x,j),At(At(y,j),i)));
					At(r,i) = t;
				}
				return r;
			}
			else
			{
				var r = Times(At(x,0),At(y,0));
				for(size_t i = 1; i < m; ++i)
					r = Plus(r,Times(At(x,i),At(y,i)));
				return r;
			}
		}
	}
	return Ex(TAG(Dot),Vec(x,y));
}
//////////////////////////////////////
}
