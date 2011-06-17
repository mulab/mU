#include <cassert>
#include <locale>
#include <mU/Number.h>
#include <mU/String.h>
#include <mU/Pattern.h>
#include <mU/Kernel.h>

namespace mU {
//////////////////////////////////////
namespace {

var EvalSym(Var symbol)
{
	map_t::const_iterator iter = OwnValues.find(symbol);
	if (iter != OwnValues.end() && iter->second != symbol)
		return Eval(iter->second);
	else
		return symbol;
}

var FlattenSequence(Var vector, bool evaluate)
{
	size_t n = Size(vector);
	var r = Vec();
	Reserve(r,n);
	for(size_t i = 0; i < n; ++i)
	{
		var c = evaluate ? Eval(At(vector, i)) : At(vector, i);
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

var EvalVec(Var vector)
{
	return FlattenSequence(vector, true);
}

bool SearchFactValues(var &result, var head, var body)
{
	std::map<Var,dict_t>::const_iterator
		iter = FactValues.find(head);
	if(iter != FactValues.end())
	{
		dict_t::const_iterator
			iter2 = iter->second.find(body);
		if(iter2 != iter->second.end())
		{
			result = Eval(iter2->second);
			return true;
		}
	}
	return false;
}

bool SearchDownValues(var &result, var head, var body)
{
	std::map<Var,def_t>::const_iterator
		iter = DownValues.find(head);
	if(iter != DownValues.end())
	{
		const def_t &definitions = iter->second;
		def_t::const_iterator
			iter2 = definitions.begin();
		map_t m;
		while(iter2 != definitions.end())
		{
			if(MatchQ(m,iter2->second.first,body))
			{
				result = Eval(Subs(m,iter2->second.second));
				return true;
			}
			++iter2;
		}
	}
	return false;
}

bool SearchCProcs(var &result, var head, var body)
{
	stdext::hash_map<Var,CProc>::const_iterator
		iter = CProcs.find(head);
	if(iter != CProcs.end())
	{
		result = iter->second(body);
		if (result)
		{
			return true;
		}
	}
	return false;
}

bool SearchSubValues(var &result, var head, var body)
{
	std::map<Var,def_t>::const_iterator
		iter = SubValues.find(Head(head));
	if(iter != SubValues.end())
	{
		def_t::const_iterator
			iter2 = iter->second.begin();
		var t = Vec(Body(head),body);
		map_t m;
		while(iter2 != iter->second.end())
		{
			if(MatchQ(m,iter2->second.first,t))
			{
				result = Eval(Subs(m,iter2->second.second));
				return true;
			}
			++iter2;
		}
	}
	return false;
}

bool SearchCOpers(var &result, var head, var body)
{
	stdext::hash_map<Var,COper>::const_iterator
		iter = COpers.find(Head(head));
	if(iter != COpers.end())
	{
		result = iter->second(Body(head),body);
		if (result)
		{
			return true;
		}
	}
	return false;
}

bool HandleAttributes(var &result, Var expression, const attr_t &attributes,
						var head, var &body)
{
	if(attributes.count(SequenceHold))
	{
		body = Body(expression);
	}
	else
	{
		body = FlattenSequence(Body(expression), false);
	}
	size_t n = Size(body);
	if(n == 1 && attributes.count(OneIdentity))
	{
		result = Eval(At(body,0));
		return true;
	}
	if(!attributes.count(HoldAll))
	{
		if(n > 0 && !attributes.count(HoldFirst))
			At(body,0) = Eval(At(body,0));
		if(n > 1 && !attributes.count(HoldRest))
			for(size_t i = 1; i < n; ++i)
				At(body,i) = Eval(At(body,i));
	}
	if(attributes.count(Listable))
	{
		var t = Thread(head,body);
		if(t)
		{
			result = Eval(t);
			return true;
		}
	}
	if(attributes.count(Flat))
	{
		var t = body;
		body = Vec();
		Reserve(body,n);
		Flatten(body,head,t);
	}
	if(attributes.count(Orderless))
		Sort(body);

	return false;
}

var EvalEx(Var expression)
{
	var head = Eval(Head(expression));
	var body;
	var result;
	bool found;
	if(SymQ(head))
	{
		std::map<Var,attr_t>::const_iterator
			iter = Attributes.find(head);
		if(iter != Attributes.end())
		{
			if (HandleAttributes(result, expression, iter->second, head, body))
			{
				return result;
			}
		}
		else
		{
			body = Eval(Body(expression));
		}

		if (SearchFactValues(result, head, body))
		{
			return result;
		}

		if (SearchDownValues(result, head, body))
		{
			return result;
		}

		if (SearchCProcs(result, head, body))
		{
			return result;
		}
	}
	else
	{
		body = Eval(Body(expression));

		if(ExQ(head) && SymQ(Head(head)))
		{
			if (SearchSubValues(result, head, body))
			{
				return result;
			}

			if (SearchCOpers(result, head, body))
			{
				return result;
			}
		}
	}
	return Ex(head,body);
}

}

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
		return EvalSym(x);
	case TYPE(vec):
		return EvalVec(x);
	case TYPE(ex):
		return EvalEx(x);
	default:
		assert(false);		// TODO: ±¨¸æ´íÎó
		return Null;
	}
}

namespace {
// TODO: each return statement here should be replaced by a throw statement.
void SetPart(var x_indices, var y)
{
	if(SymQ(At(x_indices,0)))
	{
		const var x = At(x_indices,0);
		map_t::const_iterator iter = OwnValues.find(x);
		if(iter != OwnValues.end())
		{
			var c = iter->second;
			size_t n = Size(x_indices);
			for(size_t i = 1; i < n - 1; ++i)
			{
				int j = Z::SI(Eval(At(x_indices,i)));
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
			int j = Z::SI(Eval(At(x_indices,n - 1)));
			if(j == 0) return;
			if(ExQ(c)) c = Body(c);
			if(VecQ(c))
			{
				j < 0 ? j += Size(c) : --j;
				if(j < 0 || j >= Size(c))
					return;
				At(c,j) = y;
			}
		}
		else
		{
			// TODO: more specific exception class here
			throw Exception("Part assignment failed!");
		}
	}
	else
	{
		// TODO: we need more specific exception class here.
		throw Exception("first argument to Part should be a symbol!");
	}
}

void SetDownValue(var head, var body, var y)
{
	if(ExQ(y,TAG(Condition)))
	{
		DownValues[head][Ex(TAG(Condition),Vec(body,Right(y)))] =
				std::make_pair(
						new Pattern::condition(
								new Pattern::chain(
										new Pattern::push(),
										Pat(head,body),
										new Pattern::pop()),
										Right(y)),
										Left(y));
	}
	else
	{
		DownValues[head][body] =
				std::make_pair(
						new Pattern::chain(
								new Pattern::push(),
								Pat(head,body),
								new Pattern::pop()),
								y);
	}
}

void SetSubValue(var head, var body, var y)
{
	var b = Eval(body);
	var t = Vec(Body(head),b);
	if(ExQ(y,TAG(Condition)))
	{
		SubValues[Head(head)][Ex(TAG(Condition),Vec(t,Right(y)))] =
				std::make_pair(
						new Pattern::condition(
								Pat(t),
								Right(y)),
								Left(y));
	}
	else
	{
		SubValues[Head(head)][t] =
				std::make_pair(
						Pat(t),
						y);
	}
}

}

void Set(Var x, Var y)
{
	if(SymQ(x))
	{
		OwnValues[x] = y;
	}
	else if(VecQ(x))
	{
		size_t n = Size(x);
		for(size_t i = 0; i < n; ++i)
			Set(At(x,i),At(y,i));
	}
	else if(ExQ(x))
	{
		var head = Eval(Head(x));
		if(SymQ(head))
		{
			var body = Body(x);
			if(head == TAG(Part))
			{
				SetPart(body, y);
			}
			else if(head == TAG(Property))
			{
				if(SymQ(At(body,0)) && SymQ(At(body,1)))
					Properties[At(body,0)][At(body,1)] = y;
			}
			else
			{
				body = Eval(body);
				if(FixQ(body))
				{
					FactValues[head][body] = y;
				}
				else
				{
					SetDownValue(head, body, y);
				}
			}
		}
		else if(ExQ(head) && SymQ(Head(head)))
		{
			var body = Body(x);
			SetSubValue(head, body, y);
		}
		else
		{
			// TODO: more specific exception class
			throw Exception("no assignment performed!");
		}
	}
	else
	{
		// TODO: more specific exception class
		throw Exception("no assignment performed!");
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
	t.imbue(std::locale("C"));
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
