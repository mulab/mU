#include <mU/Pattern.h>

namespace mU {
//////////////////////////////////////
using namespace Pattern;
namespace
{
	std::stack<Var> FlatHead;
}
var Pat(pos_t &x)
{
	switch(Type(*x))
	{
	case TYPE(vec):
        {
            pos_t p((Var)*x);
            return new chain(new push(),new chain(p),new pop());
        }
	case TYPE(ex):
		{
			var h = Head(*x);
			var b = Body(*x);
			size_t n = Size(b);
			if(h == TAG(Blank))
			{
				Var a = FlatHead.empty() ? 0 : FlatHead.top();
				if(a && (n == 0 || At(b,0) == a))
					return new blank_flat(a,x);
				if(n)
					return new blank_head(At(b,0));
				return new blank();
			}
			else if(h == TAG(BlankSequence))
			{
				if(n)
					return new blanks_head(At(b,0),x);
				return new blanks(x);
			}
			if(h == TAG(Pattern))
			{
				var c = At(b,0);
				var d = Body(At(b,1));
				h = Head(At(b,1));
				n = Size(d);
				if(h == TAG(Blank))
				{
					Var a = FlatHead.empty() ? 0 : FlatHead.top();
					if(a && (n == 0 || At(d,0) == a))
						return new blank_flat(a,x,c);
					if(n)
						return new blank_head(At(d,0),c);
					return new blank(c);
				}
				else if(h == TAG(BlankSequence))
				{
					if(n)
						return new blanks_head(At(d,0),x,c);
					return new blanks(x,c);
				}
			}
			if(h == TAG(Condition))
				return new condition(Pat(At(b,0)),At(b,1));
			return new chain(new head(h),Pat(h,b),new pop());
		}
	}
	return new fix(x);
}
var Pat(Var h, pos_t &x)
{
	std::map<Var,attr_t>::const_iterator
		iter = Attributes.find(h);
	var r;
	if(iter != Attributes.end())
	{
		FlatHead.push(iter->second.count(Flat) ? h : 0);
		if(iter->second.count(Orderless))
			r = new orderless(x);
		else
			r = new chain(x);
	}
	else
	{
		FlatHead.push(0);
		r = new chain(x);
	}
	FlatHead.pop();
	return r;
}
bool MemberQ(Var x, Var y)
{
	var z = Pat(y);
	size_t n = Size(x);
	map_t m;
	for(size_t i = 0; i < n; ++i)
		if(MatchQ(m,z,At(x,i)))
			return true;
	return false;
}
var Dispatch(Var x)
{
	return x;
}
var Replace(Var x, Var y, Var z)
{
	map_t m;
	if(MatchQ(m,y,x))
		return Eval(Subs(m,z));
	return x;
}
var Replace(Var x, Var y)
{
	return Replace(x,Pat(Left(y)),Right(y));
}
var ReplaceAll(Var x, Var y, Var z)
{
	map_t m;
	if(MatchQ(m,y,x))
		return Eval(Subs(m,z));
	if(VecQ(x))
	{
		size_t n = Size(x);
		var r = Vec(n);
		for(size_t i = 0; i < n; ++i)
			At(r,i) = ReplaceAll(At(x,i),y,z);
		return r;
	}
	if(ExQ(x))
		return Eval(Ex(ReplaceAll(Head(x),y,z),ReplaceAll(Body(x),y,z)));
	return x;
}
var ReplaceAll(Var x, Var y)
{
	return ReplaceAll(x,Pat(Left(y)),Right(y));
}
//////////////////////////////////////
}
