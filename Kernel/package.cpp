#include <mU/Number.h>
#include <mU/String.h>
#include <mU/Kernel.h>
#include <mU/Parser.h>

namespace mU {
//////////////////////////////////////
void BeginPackage(Var x)
{
	Begin(x);
	ContextPath.push(std::list<Var>());
	ContextPath.top().push_back(x);
	ContextPath.top().push_back(System);
}
void EndPackage()
{
	ContextPath.pop();
	std::list<Var>::iterator iter =
		std::find(
		ContextPath.top().begin(),
		ContextPath.top().end(),Context());
	if(iter != ContextPath.top().end())
		ContextPath.top().erase(iter);
	ContextPath.top().push_front(Context());
	End();
}
var Ctx(const wstring &x)
{
	std::pair<tab_t::rep_t::iterator,bool> iter =
		CTab(Contexts).insert(
		tab_t::rep_t::value_type(x,var()));
	if(iter.second)
	{
		ContextName[iter.first->second = Tab()]
		= iter.first->first.c_str();
	}
	return iter.first->second;
}
var Sym(const wstring &x)
{
	std::pair<tab_t::rep_t::iterator,bool> curr =
		CTab(Context()).insert(
		tab_t::rep_t::value_type(x,var()));
	if(!curr.second)
		return curr.first->second;
	for(std::list<Var>::const_iterator
		iter = ContextPath.top().begin();
		iter != ContextPath.top().end(); ++iter)
	{
		if(*iter == Context()) continue;
		tab_t::rep_t::const_iterator iter2 =
			CTab(*iter).find(x);
		if(iter2 != CTab(*iter).end())
		{
			CTab(Context()).erase(curr.first);
			return iter2->second;
		}
	}
	return curr.first->second
		= new sym_t(curr.first->first.c_str(),Context());
}
var Ctx2(const wchar *x)
{
	if(x[0] == L'`')
	{
		wstring t = ContextName[Context()];
		t.append(++x);
		return Ctx(t);
	}
	return Ctx(x);
}
var Sym2(const wchar *x)
{
	const wchar *c = wcsrchr(x,L'`');
	if(c)
	{
		wstring t;
		if(x[0] == L'`')
		{
			t.append(ContextName[Context()]);
			++x;
		}
		t.append(x,++c);
		return Sym(c,Ctx(t));
	}
	return Sym(x);
}
var Optimi(Var x)
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
			var r = Vec(n);
			for(size_t i = 0; i < n; ++i)
				At(r,i) = Optimi(At(x,i));
			return r;
		}
		break;
	}
	var h = Optimi(Head(x));
	var b = Optimi(Body(x));
	size_t n = Size(b);
	if(SymQ(h))
	{
		std::map<Var,attr_t>::const_iterator
			iter = Attributes.find(h);
		if(iter != Attributes.end())
		{
			if(n == 1 && iter->second.count(OneIdentity))
				return At(b,0);
			if(iter->second.count(Flat))
			{
				var t = Vec();
				Reserve(t,n);
				Flatten(t,h,b);
				b = t;
			}
			if(iter->second.count(Orderless))
				Sort(b);
		}
		if(n == 1)
		{
			var c = At(b,0);
			if(h == TAG(Minus))
			{
				if(IntQ(c))
				{
					var r = Int();
					Z::Neg(r,c);
					return r;
				}
				else if(FltQ(b))
				{
					var r = Flt();
					F::Neg(r,c);
					return r;
				}
				return Ex(TAG(Times),Vec(Int(-1L),c));
			}
			else if(h == TAG(Differential))
				return Ex(Ex(TAG(Derivative),Vec(Int(1L))),Vec(c));
		}
		else if(n == 2)
		{
			var c = At(b,0), d = At(b,1);
			if(h == TAG(Divide))
				return Ex(TAG(Times),Vec(c,Ex(TAG(Power),Vec(d,Int(-1L)))));
			else if(h == TAG(Postfix))
				return Ex(d,Vec(c));
			else if(h == TAG(Prefix))
				return Ex(c,Vec(d));
		}
	}
	return Ex(h,b);
}

var Read(wistream &f, Var x)
{
	wstring t;
	if(x)
	{
		if(SymQ(x))
		{
			if(x == TAG(Integer))
			{
				f >> t;
				return Int(string(t.begin(),t.end()).c_str(),10);
			}
			else if(x == TAG(Rational))
			{
				f >> t;
				return Rat(string(t.begin(),t.end()).c_str(),10);
			}
			else if(x == TAG(Real))
			{
				f >> t;
				return Flt(string(t.begin(),t.end()).c_str(),10);
			}
			else if(x == TAG(String))
			{
				var r = Str();
				std::getline(f,CStr(r));
				return r;
			}
			else if(x == TAG(Symbol))
			{
				f >> t;
				return Sym2(t.c_str());
			}
		}
		else if(StrQ(x) && CStr(x).size() == 1)
		{
			std::getline(f,t,CStr(x)[0]);
			return ParseString(t);
		}
	}
	std::getline(f,t);
	return ParseString(t);
}
var Parse(wistream &f)
{
	var r;
	if(f)
	{
		parser x(f);
		while(x.lookahead != parser::EOI)
		{
			try { x.parse(); }
			catch(std::exception &e)
			{
				wcerr
					<< L"Parse:"
					<< e.what()	<< L'='
					<< x.lineno << L','
					<< x.column << std::endl;
				return r;
			}
			r = Eval(Optimi(x.result()));
		}
	}
	return r;
}
//////////////////////////////////////
}
