#include <mU/Number.h>
#include <mU/Kernel.h>

namespace mU {
namespace {
void Do(dict_t &d, Var x, size_t n, const var *y)
{
	if(n > 0 && VecQ(y[0]))
	{
		var a = y[0], c = At(a,0);
		size_t m = Size(a);
		for(size_t i = 1; i < m; ++i)
			At(a,i) = Eval(Subs(d,At(a,i)));
		switch(m)
		{
		case 1:
			c = Subs(d,c);
			if(IntQ(c) && Z::Sgn(c) > 0)
			{
				size_t m = Z::UI(c);
				for(size_t i = 0; i < m; ++i)
					Do(d,x,n - 1,y + 1);
			}
			break;
		case 2:
			if(IntQ(At(a,1)))
			{
				if(Z::Sgn(At(a,1)) > 0)
				{
					size_t m = Z::UI(At(a,1));
					for(size_t i = 0; i < m; ++i)
					{
						d[c] = Int((uint)i + 1);
						Do(d,x,n - 1,y + 1);
					}
				}
			}
			else if(VecQ(At(a,1)))
			{
				var u = At(a,1);
				size_t m = Size(u);
				for(size_t i = 0; i < m; ++i)
				{
					d[c] = At(u,i);
					Do(d,x,n - 1,y + 1);
				}
			}
			break;
		case 3:
			if(IntQ(At(a,1)) && IntQ(At(a,2)))
			{
				sint u = Z::SI(At(a,1)), v = Z::SI(At(a,2));
				if(u <= v)
				{
					size_t m = v + 1 - u;
					for(size_t i = 0; i < m; ++i)
					{
						d[c] = Int(u + i);
						Do(d,x,n - 1,y + 1);
					}
				}
			}
			break;
		case 4:
			var u = At(a,1), v = At(a,2), w = At(a,3);
			if(Less(u,v))
				while(!Less(v,u))
				{
					d[c] = u;
					Do(d,x,n - 1,y + 1);
					u = Plus(u,w);
				}
			else
				while(!Greater(v,u))
				{
					d[c] = u;
					Do(d,x,n - 1,y + 1);
					u = Plus(u,w);
				}
				break;
		}
		return;
	}
	Eval(Subs(d,x));
}
var Table(dict_t &d, Var x, size_t n, const var *y)
{
	if(n > 0 && VecQ(y[0]))
	{
		var a = y[0], c = At(a,0);
		size_t m = Size(a);
		for(size_t i = 1; i < m; ++i)
			At(a,i) = Eval(Subs(d,At(a,i)));
		var r;
		switch(m)
		{
		case 1:
			c = Subs(d,c);
			if(IntQ(c) && Z::Sgn(c) > 0)
			{
				size_t m = Z::UI(c);
				r = Vec(m);
				for(size_t i = 0; i < m; ++i)
					At(r,i) = Table(d,x,n - 1,y + 1);
			}
			break;
		case 2:
			if(IntQ(At(a,1)))
			{
				if(Z::Sgn(At(a,1)) > 0)
				{
					size_t m = Z::UI(At(a,1));
					r = Vec(m);
					for(size_t i = 0; i < m; ++i)
					{
						d[c] = Int((uint)i + 1);
						At(r,i) = Table(d,x,n - 1,y + 1);
					}
				}
			}
			else if(VecQ(At(a,1)))
			{
				var u = At(a,1);
				size_t m = Size(u);
				r = Vec(m);
				for(size_t i = 0; i < m; ++i)
				{
					d[c] = At(u,i);
					At(r,i) = Table(d,x,n - 1,y + 1);
				}
			}
			break;
		case 3:
			if(IntQ(At(a,1)) && IntQ(At(a,2)))
			{
				sint u = Z::SI(At(a,1)), v = Z::SI(At(a,2));
				if(u <= v)
				{
					size_t m = v + 1 - u;
					r = Vec(m);
					for(size_t i = 0; i < m; ++i)
					{
						d[c] = Int(u + i);
						At(r,i) = Table(d,x,n - 1,y + 1);
					}
				}
			}
			break;
		case 4:
			var u = At(a,1), v = At(a,2), w = At(a,3);
			r = Vec();
			if(Less(u,v))
				while(!Less(v,u))
				{
					d[c] = u;
					Push(r,Table(d,x,n - 1,y + 1));
					u = Plus(u,w);
				}
			else
				while(!Greater(v,u))
				{
					d[c] = u;
					Push(r,Table(d,x,n - 1,y + 1));
					u = Plus(u,w);
				}
				break;
		}
		return r ? r : Vec();
	}
	return Eval(Subs(d,x));
}
}
void Do(Var x, size_t n, const var *y)
{
    dict_t d;
	Do(d,x,n,y);
}
var Table(Var x, size_t n, const var *y)
{
    dict_t d;
	return Table(d,x,n,y);
}
var Array(Var h, Var b, size_t n, const var *x)
{
	if(n > 0 && IntQ(x[0]) && Z::Sgn(x[0]) > 0)
	{
		size_t m = Z::UI(x[0]);
		var r = Vec(m);
		for(size_t i = 0; i < m; ++i)
		{
			var t = Vec();
			CVec(t) = CVec(b);
			Push(t,Int((uint)i));
			At(r,i) = Array(h,t,n - 1,x + 1);
		}
		return r;
	}
	return Eval(Ex(h,b));
}
var Array(Var h, Var b, size_t n, const var *x, const var *y)
{
	if(n > 0 && IntQ(x[0]) && Z::Sgn(x[0]) > 0)
	{
		size_t m = Z::UI(x[0]);
		var r = Vec(m);
		for(size_t i = 0; i < m; ++i)
		{
			var t = Vec();
			CVec(t) = CVec(b);
			Push(t,Plus(y[0],Int((uint)i)));
			At(r,i) = Array(h,t,n - 1,x + 1,y + 1);
		}
		return r;
	}
	return Eval(Ex(h,b));
}
}
