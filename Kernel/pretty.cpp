#include <mU/Number.h>
#include <mU/Kernel.h>

namespace mU {
//////////////////////////////////////
var Pretty(Var x)
{
	switch(Type(x))
	{
	case TYPE(int):
		if(Z::Sgn(x) < 0)
		{
			var r = Int();
			Z::Neg(r,x);
			return Ex(TAG(Minus),Vec(r));
		}
		break;
	case TYPE(rat):
		{
			Q::Canonicalize(x);
			var a = Int();
			var b = Int();
			Q::Num(a,x);
			Q::Den(b,x);
			if(mpz_cmp_ui(CInt(b),1) == 0) return Pretty(a);
			if(Z::Sgn(a) < 0)
			{
				Z::Neg(a,a);
				return Ex(TAG(Minus),Vec(Ex(TAG(Divide),Vec(a,b))));
			}
			return Ex(TAG(Divide),Vec(a,b));
		}
		break;
	case TYPE(flt):
		if(F::Sgn(x) < 0)
		{
			var r = Flt();
			F::Neg(r,x);
			return Ex(TAG(Minus),Vec(r));
		}
		break;
	case TYPE(vec):
		{
			size_t n = Size(x);
			var r = Vec(n);
			for(size_t i = 0; i < n; ++i)
				At(r,i) = Pretty(At(x,i));
			return r;
		}
		break;
	case TYPE(ex):
		{
			var h = Pretty(Head(x));
			var b = Pretty(Body(x));
			size_t n = Size(b);
			if(h == TAG(Power))
			{
				var c = At(b,1);
				if(ExQ(c,TAG(Minus)))
				{
					return OneQ(Left(c))
						? Ex(TAG(Divide),Vec(Int(1L),At(b,0)))
						: Ex(TAG(Divide),Vec(Int(1L),Pretty(Ex(TAG(Power),Vec(At(b,0),Left(c))))));
				}
				else if(ExQ(c,TAG(Divide)) && OneQ(Left(c)) && IntQ(Right(c)))
				{
					var e = Right(c);
					return mpz_cmp_ui(CInt(e),2) == 0
						? Ex(TAG(Sqrt),Vec(At(b,0)))
						: Ex(TAG(Radical),Vec(At(b,0),e));
				}
			}
			else if(h == TAG(Times))
			{
				var r[2];
				r[0] = Vec();
				r[1] = Vec();
				int sgn = 1;
				for(size_t i = 0; i < n; ++i)
				{
					var c = At(b,i);
					if(ExQ(c))
					{
						h = Head(c);
						if(h == TAG(Minus))
						{
							sgn = -sgn;
							c = At(Body(c),0);
							if(ExQ(c,TAG(Divide)))
							{
								c = Body(c);
								var a = At(c,0);
								if(ExQ(a,TAG(Minus)))
								{
									sgn = -sgn;
									Push(r[0],At(Body(a),0));
								}
								else
									Push(r[0],a);
								var b = At(c,1);
								if(ExQ(b,TAG(Minus)))
								{
									sgn = -sgn;
									Push(r[0],At(Body(b),0));
								}
								else
									Push(r[1],b);
							}
							else
								Push(r[0],c);
						}
						else if(h == TAG(Divide))
						{
							c = Body(c);
							var a = At(c,0);
							if(ExQ(a,TAG(Minus)))
							{
								sgn = -sgn;
								Push(r[0],At(Body(a),0));
							}
							else
								Push(r[0],a);
							var b = At(c,1);
							if(ExQ(b,TAG(Minus)))
							{
								sgn = -sgn;
								Push(r[0],At(Body(b),0));
							}
							else
								Push(r[1],b);
						}
						else
							Push(r[0],c);
					}
					else
						Push(r[0],c);
				}
				var t[3];
				Sort(r[0]);
				t[0] = Times(r[0]);
				Sort(r[1]);
				t[1] = Times(r[1]);
				t[2] = OneQ(t[1])
					? t[0]
				: Ex(TAG(Divide),Vec(t[0],t[1]));
				return sgn == 1
					? t[2]
				: Ex(TAG(Minus),Vec(t[2]));
			}
			return Ex(h,b);
		}
		break;
	}
	return x;
}
//////////////////////////////////////
}
