#include <mU/Number.h>
#include <mU/String.h>
#include <mU/Pattern.h>
#include <mU/Kernel.h>

namespace mU {
//////////////////////////////////////
var Eval(Var x)
{
	switch(Type(x))
	{
	case TYPE(obj):
	case TYPE(int):
	case TYPE(rat):
	case TYPE(flt):
	case TYPE(str):
		return x;
	case TYPE(sym):
		{
			map_t::const_iterator
				iter = OwnValues.find(x);
			if(iter != OwnValues.end() && iter->second != x)
				return Eval(iter->second);
		}
		return x;
	case TYPE(vec):
		{
			size_t n = Size(x);
			var r = Vec();
			Reserve(r,n);
			for(size_t i = 0; i < n; ++i)
			{
				var c = Eval(At(x,i));
				if(ExQ(c,TAG(Sequence)))
					CVec(r).insert(
					CVec(r).end(),
					CVec(Body(c)).begin(),
					CVec(Body(c)).end());
				else
					Push(r,c);
			}
			return r;
		}
	}
	var h = Eval(Head(x));
	var r;
	if(SymQ(h))
	{
		std::map<Var,attr_t>::const_iterator
			iter = Attributes.find(h);
		if(iter != Attributes.end())
		{
			if(iter->second.count(SequenceHold))
				r = Body(x);
			else
			{
				size_t n = Size(Body(x));
				r = Vec();
				Reserve(r,n);
				for(size_t i = 0; i < n; ++i)
				{
					var c = At(Body(x),i);
					if(ExQ(c,TAG(Sequence)))
						CVec(r).insert(
						CVec(r).end(),
						CVec(Body(c)).begin(),
						CVec(Body(c)).end());
					else
						Push(r,c);
				}
			}
			size_t n = Size(r);
			if(n == 1 && iter->second.count(OneIdentity))
				return Eval(At(r,0));
			if(!iter->second.count(HoldAll))
			{
				if(n > 0 && !iter->second.count(HoldFirst))
					At(r,0) = Eval(At(r,0));
				if(n > 1 && !iter->second.count(HoldRest))
					for(size_t i = 1; i < n; ++i)
						At(r,i) = Eval(At(r,i));
			}
			if(iter->second.count(Listable))
			{
				var t = Thread(h,r);
				if(t) return Eval(t);
			}
			if(iter->second.count(Flat))
			{
				var t = r;
				r = Vec();
				Reserve(r,n);
				Flatten(r,h,t);
			}
			if(iter->second.count(Orderless))
				Sort(r);
		}
		else
			r = Eval(Body(x));
		{
			std::map<Var,dict_t>::const_iterator
				iter = FactValues.find(h);
			if(iter != FactValues.end())
			{
				dict_t::const_iterator
					iter2 = iter->second.find(r);
				if(iter2 != iter->second.end())
					return Eval(iter2->second);
			}
		}
		{
			std::map<Var,def_t>::const_iterator
				iter = DownValues.find(h);
			if(iter != DownValues.end())
			{
				def_t::const_iterator
					iter2 = iter->second.begin();
				map_t m;
				while(iter2 != iter->second.end())
				{
					if(MatchQ(m,iter2->second.first,r))
						return Eval(Subs(m,iter2->second.second));
					++iter2;
				}
			}
		}
		{
			stdext::hash_map<Var,CProc>::const_iterator
				iter = Externals.find(h);
			if(iter != Externals.end())
			{
				var t = iter->second(r);
				if(t) return t;
			}
		}
	}
	else
	{
		r = Eval(Body(x));
		if(ExQ(h) && SymQ(Head(h)))
		{
			{
				std::map<Var,def_t>::const_iterator
					iter = SubValues.find(Head(h));
				if(iter != SubValues.end())
				{
					def_t::const_iterator
						iter2 = iter->second.begin();
					var t = Vec(Body(h),r);
					map_t m;
					while(iter2 != iter->second.end())
					{
						if(MatchQ(m,iter2->second.first,t))
							return Eval(Subs(m,iter2->second.second));
						++iter2;
					}
				}
			}
			{
				stdext::hash_map<Var,COper>::const_iterator
					iter = Functionals.find(Head(h));
				if(iter != Functionals.end())
				{
					var t = iter->second(Body(h),r);
					if(t) return t;
				}
			}
		}
	}
	return Ex(h,r);
}
void Set(Var x, Var y)
{
	if(SymQ(x))
	{
		OwnValues[x] = y;
		return;
	}
	if(VecQ(x))
	{
		size_t n = Size(x);
		for(size_t i = 0; i < n; ++i)
			Set(At(x,i),At(y,i));
		return;
	}
	if(ExQ(x))
	{
		var h = Eval(Head(x));
		if(SymQ(h))
		{
			var b = Body(x);
			if(h == TAG(Part))
			{
				if(SymQ(At(b,0)))
				{
					map_t::const_iterator
						iter = OwnValues.find(At(b,0));
					if(iter != OwnValues.end())
					{
						var c = iter->second;
						size_t n = Size(b);
						for(size_t i = 1; i < n - 1; ++i)
						{
							int j = Z::SI(Eval(At(b,i)));
							if(j == 0) return;
							if(ExQ(c)) c = Body(c);
							if(VecQ(c))
							{
								j < 0 ? j += Size(c) : --j;
								if(j < 0 || j >= Size(c))
									return;
								c = At(c,j);
							}
						}
						int j = Z::SI(Eval(At(b,n - 1)));
						if(j == 0) return;
						if(ExQ(c)) c = Body(c);
						if(VecQ(c))
						{
							j < 0 ? j += Size(c) : --j;
							if(j < 0 || j >= Size(c))
								return;
							At(c,j) = y;
						}
						return;
					}
				}
			}
			else if(h == TAG(Property))
			{
				if(SymQ(At(b,0)) && SymQ(At(b,1)))
					Properties[At(b,0)][At(b,1)] = y;
				return;
			}
			b = Eval(b);
			if(FixQ(b))
				FactValues[h][b] = y;
			else
			{
				if(ExQ(y,TAG(Condition)))
				{
					DownValues[h][Ex(TAG(Condition),Vec(b,Right(y)))] =
						std::make_pair(
						new Pattern::condition(
						new Pattern::chain(
						new Pattern::push(),
						Pat(h,b),
						new Pattern::pop()),Right(y)),
						Left(y));
				}
				else
				{
					DownValues[h][b] =
						std::make_pair(
						new Pattern::chain(
						new Pattern::push(),
						Pat(h,b),
						new Pattern::pop()),
						y);
				}
			}
			return;
		}
		else if(ExQ(h) && SymQ(Head(h)))
		{
			var b = Eval(Body(x));
			var t = Vec(Body(h),b);
			if(ExQ(y,TAG(Condition)))
			{
				SubValues[Head(h)][Ex(TAG(Condition),Vec(t,Right(y)))] =
					std::make_pair(
					new Pattern::condition(
					Pat(t),
					Right(y)),
					Left(y));
			}
			else
			{
				SubValues[Head(h)][t] =
					std::make_pair(
					Pat(t),
					y);
			}
			return;
		}
	}
}
void Unset(Var x)
{
	if(SymQ(x))
	{
		OwnValues.erase(x);
		return;
	}
	if(VecQ(x))
	{
		size_t n = Size(x);
		for(size_t i = 0; i < n; ++i)
			Unset(At(x,i));
		return;
	}
	if(ExQ(x))
	{
		var h = Eval(Head(x));
		if(SymQ(h))
		{
			var b = Body(x);
			if(h == TAG(Property))
			{
				if(SymQ(At(b,0)) && SymQ(At(b,1)))
				{
					std::map<Var,map_t>::iterator
						iter = Properties.find(At(b,0));
					if(iter != Properties.end())
						iter->second.erase(At(b,1));
				}
				return;
			}
			b = Eval(b);
			if(FixQ(b))
			{
				std::map<Var,dict_t>::iterator
					iter = FactValues.find(h);
				if(iter != FactValues.end())
					iter->second.erase(b);
			}
			else
			{
				std::map<Var,def_t>::iterator
					iter = DownValues.find(h);
				if(iter != DownValues.end())
					iter->second.erase(b);
			}
			return;
		}
		else if(ExQ(h) && SymQ(Head(h)))
		{
			var b = Eval(Body(x));
			var t = Vec(Body(h),b);
			std::map<Var,def_t>::iterator
				iter = SubValues.find(Head(h));
			if(iter != SubValues.end())
					iter->second.erase(t);
			return;
		}
	}
}
void Clear(Var x)
{
	OwnValues.erase(x);
	FactValues.erase(x);
	DownValues.erase(x);
	SubValues.erase(x);
	Properties.erase(x);
	Attributes.erase(x);
}
var Thread(Var h, Var b)
{
	size_t n = Size(b);
	size_t m = 0;
	for(size_t i = 0; i < n; ++i)
#undef max
		if(VecQ(At(b,i))) m = std::max(m,Size(At(b,i)));
	if(m == 0) return 0;
	var r = Vec(m);
	for(size_t i = 0; i < m; ++i)
	{
		var c = Vec(n);
		for(size_t j = 0; j < n; ++j)
		{
			var t = At(b,j);
			if(VecQ(t))
#undef min
				At(c,j) = At(t,std::min(i,Size(t) - 1));
			else
				At(c,j) = t;
		}
		At(r,i) = Ex(h,c);
	}
	return r;
}
void Flatten(Var r, Var b)
{
	size_t n = Size(b);
	for(size_t i = 0; i < n; ++i)
	{
		var c = At(b,i);
		if(VecQ(c))
		{
			size_t n = Size(c);
			for(size_t i = 0; i < n; ++i)
				Push(r,At(c,i));
		}
		else
			Push(r,c);
	}
}
void Flatten(Var r, Var h, Var b)
{
	size_t n = Size(b);
	for(size_t i = 0; i < n; ++i)
	{
		var c = At(b,i);
		if(ExQ(c,h))
		{
			c = Body(c);
			size_t n = Size(c);
			for(size_t i = 0; i < n; ++i)
				Push(r,At(c,i));
		}
		else
			Push(r,c);
	}
}
void FlattenAll(Var r, Var b)
{
	size_t n = Size(b);
	for(size_t i = 0; i < n; ++i)
	{
		var c = At(b,i);
		VecQ(c) ? FlattenAll(r,c) : Push(r,c);
	}
}
void FlattenAll(Var r, Var h, Var b)
{
	size_t n = Size(b);
	for(size_t i = 0; i < n; ++i)
	{
		var c = At(b,i);
		ExQ(c,h) ? FlattenAll(r,h,Body(c)) : Push(r,c);
	}
}
bool FixQ(Var x)
{
	if(VecQ(x))
	{
		size_t n = Size(x);
		for(size_t i = 0; i < n; ++i)
			if(!FixQ(At(x,i)))
				return false;
		return true;
	}
	if(ExQ(x))
	{
		var h = Head(x);
		if(h == TAG(Pattern) ||
			h == TAG(Blank) ||
			h == TAG(Condition) ||
			h == TAG(Optional) ||
			h == TAG(PatternTest) ||
			h == TAG(BlankSequence))
			return false;
		return FixQ(Body(x));
	}
	return true;
}
wstring Unique()
{
	static size_t count = 0;
	wostringstream t;
	t << L'$' << ++count;
	return t.str();
}
var Supply(Var w, Var x, Var y)
{
	switch(Type(x))
	{
	case TYPE(obj):
	case TYPE(int):
	case TYPE(rat):
	case TYPE(flt):
	case TYPE(str):
	case TYPE(sym):
		return x;
	case TYPE(vec):
		{
			size_t n = Size(x);
			var r = Vec();
			Reserve(r,n);
			for(size_t i = 0; i < n; ++i)
			{
				var c = At(x,i);
				if(ExQ(c,TAG(SlotSequence)))
				{
					size_t n = Z::UI(Left(c));
					if(n > 0 && n <= Size(y))
					{
						CVec(r).insert(
							CVec(r).end(),
							CVec(y).begin() + n - 1,
							CVec(y).end());
					}
				}
				else
					Push(r,Supply(w,c,y));
			}
			return r;
		}
	}
	var h = Head(x);
	var b = Body(x);
	if(h == TAG(Slot))
	{
		size_t n = Z::UI(At(b,0));
		if(n == 0) return w;
		if(n <= Size(y))
			return At(y,n - 1);
	}
	else if(h == TAG(SlotSequence))
	{
		size_t n = Z::UI(At(b,0));
		if(n > 0 && n <= Size(y))
		{
			var r = Vec();
			CVec(r).assign(
				CVec(y).begin() + n - 1,
				CVec(y).end());
			return r;
		}
	}
	return Ex(Supply(w,h,y),Supply(w,b,y));
}
//////////////////////////////////////
}
