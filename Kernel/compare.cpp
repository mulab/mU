#include <mU/Number.h>
#include <mU/String.h>
#include <mU/Kernel.h>

namespace mU {
//////////////////////////////////////
int Compare(Var x, Var y)
{
	if(Type(x) < Type(y)) return -1;
	if(Type(x) > Type(y)) return 1;
	switch(Type(x))
	{
	case TYPE(obj):
	case TYPE(sym): return x - y;
	case TYPE(int): return Z::Cmp(x,y);
	case TYPE(rat): return Q::Cmp(x,y);
	case TYPE(flt): return F::Cmp(x,y);
	case TYPE(str): return S::Cmp(x,y);
	case TYPE(vec):
		{
			if(Size(x) < Size(y)) return -1;
			if(Size(y) < Size(x)) return 1;
			int flag = 0;
			size_t n = Size(x);
			for(size_t i = 0; i < n; ++i)
				if(flag = Compare(At(x,i),At(y,i)))
					break;
			return flag;
		}
	case TYPE(ex):
		{
			int flag = Compare(Head(x),Head(y));
			return  flag ? flag : Compare(Body(x),Body(y));
		}
	}
	return 0;
}

int Order(Var x, Var y)
{
	if(Type(x) < Type(y))
		return NumQ(x) && NumQ(y) ? N::Cmp(x,y) : -1;
	if(Type(x) > Type(y))
		return NumQ(x) && NumQ(y) ? N::Cmp(x,y) : 1;
	switch(Type(x))
	{
	case TYPE(obj): return x - y;
	case TYPE(int): return Z::Cmp(x,y);
	case TYPE(rat): return Q::Cmp(x,y);
	case TYPE(flt): return F::Cmp(x,y);
	case TYPE(str): return S::Cmp(x,y);
	case TYPE(sym): return x == y ? 0 : wcscmp(Name(x),Name(y));
	case TYPE(vec):
		{
			if(Size(x) < Size(y)) return -1;
			if(Size(y) < Size(x)) return 1;
			int flag = 0;
			size_t n = Size(x);
			for(size_t i = 0; i < n; ++i)
				if(flag = Order(At(x,i),At(y,i)))
					break;
			return flag;
		}
	case TYPE(ex):
		{
			int flag = Order(Head(x),Head(y));
			return  flag ? flag : Order(Body(x),Body(y));
		}
	}
	return 0;
}

bool FreeQ(Var x, Var y)
{
	if(Same(x,y)) return false;
	if(VecQ(x))
	{
		size_t n = Size(x);
		for(size_t i = 0; i < n; ++i)
			if(!FreeQ(At(x,i),y))
				return false;
		return true;
	}
	if(ExQ(x))
		return FreeQ(Head(x),y) && FreeQ(Body(x),y);
	return true;
}
//////////////////////////////////////
}
