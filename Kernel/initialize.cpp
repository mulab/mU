#include <mU/Number.h>
#include <mU/String.h>
#include <mU/Pattern.h>
#include <mU/Kernel.h>
#include <mU/Interface.h>

namespace mU {
namespace {
Wrap(CompoundExpression)
{
	var r;
	size_t n = Size(x);
	for(size_t i = 0; i < n; ++i)
	{
		r = Eval(At(x,i));
		if(ExQ(r))
		{
			var h = Head(r);
			if(h == TAG(Return))
			{
				var b = Body(r);
				return Size(b) == 0 ? r : Eval(At(b,0));
			}
			else if(h == TAG(Break) || h == TAG(Continue))
				break;
		}
	}
	return r;
}
Wrap(Flatten)
{
	var c = At(x,0);
	if(VecQ(c))
	{
		var r = Vec();
		Flatten(r,c);
		return r;
	}
	else if(ExQ(c))
	{
		var r = Vec();
		var h = Head(c);
		var b = Body(c);
		Flatten(r,h,b);
		return Ex(h,r);
	}
	return 0;
}
Wrap(FlattenAll)
{
	var c = At(x,0);
	if(VecQ(c))
	{
		var r = Vec();
		FlattenAll(r,c);
		return r;
	}
	else if(ExQ(c))
	{
		var r = Vec();
		var h = Head(c);
		var b = Body(c);
		FlattenAll(r,h,b);
		return Ex(h,r);
	}
	return 0;
}
Wrap(And)
{
	var r = Vec();
	size_t n = Size(x);
	for(size_t i = 0; i < n; ++i)
	{
		var c = Eval(At(x,i));
		if(c == False) return False;
		if(c == True) continue;
		Push(r,c);
	}
	if(Size(r) == 0) return True;
	return 0;
}
Wrap(Or)
{
	var r = Vec();
	size_t n = Size(x);
	for(size_t i = 0; i < n; ++i)
	{
		var c = Eval(At(x,i));
		if(c == True) return True;
		if(c == False) continue;
		Push(r,c);
	}
	if(Size(r) == 0) return False;
	return 0;
}
Wrap(Not)
{
	return At(x,0) == True
		? False
		: At(x,0) == False
		? True
		: 0;
}
Wrap(If)
{
	return At(x,0) == True
		? Eval(At(x,1))
		: At(x,0) == False && Size(x) == 3
		? Eval(At(x,2))
		: Null;
}
Wrap(For)
{
	var r;
	for(Eval(At(x,0));
		Eval(At(x,1)) == True;
		Eval(At(x,2)))
	{
		r = Eval(At(x,3));
		if(ExQ(r))
		{
			var h = Head(r);
			if(h == TAG(Return)) return r;
			if(h == TAG(Break)) break;
			if(h == TAG(Continue)) continue;
		}
	}
	return Null;
}
Wrap(While)
{
	var r;
	while(Eval(At(x,0)) == True)
	{
		r = Eval(At(x,1));
		if(ExQ(r))
		{
			var h = Head(r);
			if(h == TAG(Return)) return r;
			if(h == TAG(Break)) break;
			if(h == TAG(Continue)) continue;
		}
	}
	return Null;
}
Wrap(SameQ)
{
	return Same(At(x,0),At(x,1)) ? True : False;
}
Wrap(Less)
{
	return Less(At(x,0),At(x,1)) ? True : False;
}
Wrap(Equal)
{
	return Equal(At(x,0),At(x,1)) ? True : False;
}
Wrap(Greater)
{
	return Greater(At(x,0),At(x,1)) ? True : False;
}
Wrap(UnsameQ)
{
	return !Same(At(x,0),At(x,1)) ? True : False;
}
Wrap(GreaterEqual)
{
	return !Less(At(x,0),At(x,1)) ? True : False;
}
Wrap(Unequal)
{
	return !Equal(At(x,0),At(x,1)) ? True : False;
}
Wrap(LessEqual)
{
	return !Greater(At(x,0),At(x,1)) ? True : False;
}
Wrap(FreeQ)
{
	return FreeQ(At(x,0),At(x,1)) ? True : False;
}
Wrap(With)
{
	var a = At(x,0);
	size_t n = Size(a);
	map_t m;
	for(size_t i = 0; i < n; ++i)
		if(ExQ(At(a,i),TAG(Set)))
			m[Left(At(a,i))] = Eval(Right(At(a,i)));
	return Eval(Subs(m,At(x,1)));
}
Wrap(Block)
{
	var a = At(x,0);
	size_t n = Size(a);
	std::vector<Var> locals(n);
	std::vector<var> local_values(n);
	for(size_t i = 0; i < n; ++i)
	{
		var c = At(a,i);
		if(ExQ(c,TAG(Set)))
		{
			locals[i] = Left(c);
			local_values[i] = Eval(Left(c));
			Set(Left(c),Eval(Right(c)));
		}
		else if(SymQ(At(a,i)))
		{
			locals[i] = c;
			local_values[i] = Eval(c);
		}
	}
	var r = Eval(At(x,1));
	for(size_t i = 0; i < n; ++i)
		Set(locals[i],local_values[i]);
	return r;
}
Wrap(Module)
{
	wstring s = Unique();
	var a = At(x,0);
	size_t n = Size(a);
	map_t m;
	for(size_t i = 0; i < n; ++i)
	{
		var c = At(a,i);
		if(ExQ(c,TAG(Set)))
		{
			var t = Sym(Name(Left(c)) + s);
			m[Left(c)] = t;
			Set(t,Eval(Right(c)));
		}
		else if(SymQ(At(a,i)))
		{
			var t = Sym(Name(c) + s);
			m[c] = t;
		}
	}
	var r = Eval(Subs(m,At(x,1)));
	map_t::const_iterator
		iter = m.begin();
	while(iter != m.end())
	{
		Unset(iter->second);
		++iter;
	}
	return r;
}
Wrap(MatchQ)
{
    map_t m;
	return MatchQ(m,Pat(At(x,1)),At(x,0)) ? True : False;
}
Wrap(MemberQ)
{
	return MemberQ(At(x,0),At(x,1)) ? True : False;
}
Wrap(Dispatch)
{
	if(VecQ(At(x,0)))
		return Dispatch(At(x,0));
	return Dispatch(x);
}
Wrap(Replace)
{
	var a = At(x,0);
	var b = At(x,1);
	if(VecQ(b))
	{
		size_t n = Size(b);
		for(size_t i = 0; i < n; ++i)
			a = Replace(a,At(b,i));
		return a;
	}
	return Replace(a,b);
}
Wrap(ReplaceAll)
{
	var a = At(x,0);
	var b = At(x,1);
	if(VecQ(b))
	{
		size_t n = Size(b);
		for(size_t i = 0; i < n; ++i)
			a = ReplaceAll(a,At(b,i));
		return a;
	}
	return ReplaceAll(a,b);
}
Wrap(ReplaceRepeated)
{
	var r = WRAP(ReplaceAll)(x);
	while(!Same(r,At(x,0)))
	{
		At(x,0) = r;
		r = WRAP(ReplaceAll)(x);
	}
	return r;
}
Wrap(OddQ)
{
	return OddQ(At(x,0)) ? True : False;
}
Wrap(EvenQ)
{
	return EvenQ(At(x,0)) ? True : False;
}
Wrap(SetAttributes)
{
	var a = At(x,0);
	var b = At(x,1);
	if(VecQ(b))
	{
		size_t n = Size(b);
		for(size_t i = 0; i < n; ++i)
			Attributes[a].insert(At(b,i));
	}
	else
		Attributes[a].insert(b);
	return Null;
}
Wrap(StringJoin)
{
	size_t n = Size(x);
	var c = At(x,0);
	if(StrQ(c))
	{
		var r = Str();
		CStr(r) = CStr(c);
		for(size_t i = 1; i < n; ++i)
			S::AddTo(r,At(x,i));
		return r;
	}
	return 0;
}
Wrap(Join)
{
	size_t n = Size(x);
	var c = At(x,0);
	if(VecQ(c))
	{
		size_t m = Size(c);
		for(size_t i = 1; i < n; ++i)
			m += Size(At(x,i));
		var r = Vec(m);
		vec_t::rep_t::iterator iter = CVec(r).begin();
		std::copy(CVec(c).begin(),CVec(c).end(),iter);
		for(size_t i = 1; i < n; ++i)
			std::copy(CVec(At(x,i)).begin(),CVec(At(x,i)).end(),iter += Size(At(x,i - 1)));
		return r;
	}
	return 0;
}
Wrap(Get)
{
	if(StrQ(At(x,0)))
	{
		var r = ParseFile(CStr(At(x,0)));
		return r ? r : Nil;
	}
	return 0;
}
Wrap(Put)
{
	var a = At(x,0);
	var b = At(x,1);
	if(StrQ(b))
	{
		wofstream f(string(CStr(b).begin(),CStr(b).end()).c_str());
		if(f)
		{
			Println(a,f);
			f.close();
			return True;
		}
	}
	return False;
}
Wrap(PutAppend)
{
	var a = At(x,0);
	var b = At(x,1);
	if(StrQ(b))
	{
		wofstream f(string(CStr(b).begin(),CStr(b).end()).c_str(),wios::app);
		if(f)
		{
			Println(a,f);
			f.close();
			return True;
		}
	}
	return False;
}
Wrap(Attributes)
{
	var r = Vec();
	std::map<Var,attr_t>::const_iterator
		iter = Attributes.find(At(x,0));
	if(iter != Attributes.end())
		CVec(r).assign(iter->second.begin(),iter->second.end());
	return r;
}
Wrap(Timing)
{
	return Timing(At(x,0));
}
Wrap(Path)
{
	return Size(x) ? Str(Path() + CStr(At(x,0))) : Str(Path());
}
Wrap(Install)
{
	return Install(CStr(At(x,0)).c_str());
}
Wrap(Uninstall)
{
	return Uninstall(At(x,0)) ? True : False;
}
Wrap(Run)
{
	return Run(CStr(At(x,0))) ? True : False;
}
//Wrap(Task)
//{
//	return Task(At(x,0));
//}
//Wrap(Kill)
//{
//	return Kill(At(x,0)) ? True : False;
//}
Wrap(NumberQ)
{
	return NumQ(At(x,0)) ? True : False;
}
Wrap(AtomQ)
{
	return AtomQ(At(x,0)) ? True : False;
}
Wrap(IntegerQ)
{
	return IntQ(At(x,0)) ? True : False;
}
Wrap(SymbolQ)
{
	return SymQ(At(x,0)) ? True : False;
}
Wrap(StringQ)
{
	return StrQ(At(x,0)) ? True : False;
}
Wrap(Input)
{
	var r = Size(x) > 0
		? Read(wcin,At(x,0))
		: Read(wcin);
	return r ? r : Nil;
}
Wrap(Print)
{
	Write(wcout,Size(x) == 1 ? static_cast<Var>(At(x,0)) : x);
	return Null;
}
Wrap(Pretty)
{
	return Pretty(At(x,0));
}
Wrap(Unset)
{
	Unset(At(x,0));
	return Null;
}
Wrap(Clear)
{
	size_t n = Size(x);
	for(size_t i = 0; i < n; ++i)
		Clear(At(x,i));
	return Null;
}
Wrap(Evaluate)
{
	return Eval(At(x,0));
}
Wrap(Part)
{
	var c = At(x,0);
	size_t n = Size(x);
	for(size_t i = 1; i < n; ++i)
	{
		int j = Z::SI(At(x,i));
		if(j == 0) return Tag(c);
		if(ExQ(c)) c = Body(c);
		if(VecQ(c))
		{
			j < 0 ? j += Size(c) : --j;
			if(j < 0 || j >= Size(c))
				return c;
			c = At(c,j);
		}
	}
	return c;
}
Wrap(Length)
{
	var c = At(x,0);
	size_t n = 0;
	if(VecQ(c))
		n = Size(c);
	else if(ExQ(c))
		n = Size(Body(c));
	return Int((uint)n);
}
Wrap(Head)
{
	return Tag(At(x,0));
}
Wrap(Context)
{
	var r;
	if(Size(x))
        r = Context(At(x,0));
	else
        r = Context();
	return Str(ContextName[r]);
}
Wrap(Contexts)
{
	size_t n = CTab(Contexts).size();
	var r = Vec(n);
	tab_t::rep_t::const_iterator
		iter = CTab(Contexts).begin();
	for(size_t i = 0; i < n; ++i)
	{
		At(r,i) = Str(iter->first);
		++iter;
	}
	return r;
}
Wrap(ContextPath)
{
	size_t n = ContextPath.top().size();
	var r = Vec(n);
	std::list<Var>::const_iterator
		iter = ContextPath.top().begin();
	for(size_t i = 0; i < n; ++i)
	{
		At(r,i) = Str(ContextName[*iter]);
		++iter;
	}
	return r;
}
Wrap(Begin)
{
	var r = Ctx2(CStr(At(x,0)).c_str());
	Begin(r);
	return Str(ContextName[r]);
}
Wrap(End)
{
	var r = Context();
	End();
	return Str(ContextName[r]);
}
Wrap(BeginPackage)
{
	var r = Ctx2(CStr(At(x,0)).c_str());
	BeginPackage(r);
	return Null;
}
Wrap(EndPackage)
{
	EndPackage();
	return Null;
}
Wrap(Unevaluated)
{
	return At(x,0);
}
Wrap(Apply)
{
	if(VecQ(At(x,1)))
		return Eval(Ex(At(x,0),At(x,1)));
	return 0;
}
Wrap(Map)
{
	if(VecQ(At(x,1)))
	{
		var a = At(x,0);
		var b = At(x,1);
		size_t n = Size(b);
		var r = Vec(n);
		for(size_t i = 0; i < n; ++i)
			At(r,i) = Eval(Ex(a,Vec(At(b,i))));
		return r;
	}
	return 0;
}
Wrap(Property)
{
	std::map<Var,map_t>::const_iterator
		iter = Properties.find(At(x,0));
	if(iter != Properties.end())
	{
		map_t::const_iterator
			iter2 = iter->second.find(At(x,1));
		if(iter2 != iter->second.end())
			return iter2->second;
	}
	return 0;
}
Wrap(Full)
{
	wostringstream t;
	FullPrint(At(x,0),t);
	return Str(t.str());
}
Wrap(ToString)
{
	wostringstream t;
	Print(Pretty(At(x,0)),t);
	return Str(t.str());
}
Wrap(ToExpression)
{
	if(StrQ(At(x,0)))
	{
		var r = ParseString(CStr(At(x,0)));
		return r ? r : Nil;
	}
	return 0;
}
Wrap(Exit)
{
	if(Size(x) && IntQ(At(x,0)))
		exit(Z::SI(At(x,0)));
	exit(-1);
	return Null;
}
Wrap(Quit)
{
	exit(-1);
	return Null;
}
Wrap(Set)
{
	Set(At(x,0),At(x,1));
	return At(x,1);
}
Wrap(SetDelayed)
{
	Set(At(x,0),At(x,1));
	return Null;
}
Wrap(Hold)
{
	return 0;
}
Wrap(Array)
{
	size_t n = Size(x);
	var h = At(x,0);
	var b = Vec();
	size_t m = Size(At(x,1));
	if(n == 2) return Array(h,b,m,&At(At(x,1),0));
	if(n == 3) return Array(h,b,m,&At(At(x,1),0),&At(At(x,2),0));
	return 0;
}
Wrap(Table)
{
	return Table(At(x,0),Size(x) - 1,&At(x,1));
}
Wrap(Do)
{
	Do(At(x,0),Size(x) - 1,&At(x,1));
	return Null;
}
Wrap(N)
{
	size_t n = Size(x);
	if(n == 1) return Evalf(At(x,0));
	if(n == 2 && IntQ(At(x,1)))
		return Evalf(At(x,0),Z::UI(At(x,1)));
	return 0;
}
Wrap(IntegerPart)
{
	return IntegerPart(At(x,0));
}
Wrap(Floor)
{
	return Floor(At(x,0));
}
Wrap(Ceiling)
{
	return Ceiling(At(x,0));
}
Wrap(Round)
{
	return Round(At(x,0));
}
Wrap(Expand)
{
	return Expand(At(x,0));
}
Wrap(Variables)
{
	return Variables(At(x,0));
}
Wrap(Coefficient)
{
	return Coefficient(At(x,0),At(x,1));
}
Wrap(Exponent)
{
	return Exponent(At(x,0),At(x,1));
}
Wrap(Deg)
{
	return Deg(At(x,0),At(x,1));
}
Wrap(CoefficientList)
{
	var c = At(x,1);
	if(VecQ(c))
		return CoefficientList(At(x,0),c);
	return Coefficients(At(x,0),c);
}
Wrap(FromCoefficientList)
{
	var c = At(x,1);
	if(VecQ(c))
		return FromCoefficientList(At(x,0),c);
	return FromCoefficients(At(x,0),c);
}
Wrap(Power)
{
	return Power(At(x,0),At(x,1));
}
Wrap(Mod)
{
	return Mod(At(x,0),At(x,1));
}
Wrap(StringLength)
{
	if(StrQ(At(x,0))) return Int(S::Length(At(x,0)));
	return 0;
}
Wrap(StringInsert)
{
	var c = At(x,0);
	if(StrQ(c))
	{
		uint n = S::Length(c);
		var r = Str();
		CStr(r) = CStr(c);
		if(Size(x) > 2 && StrQ(At(x,1)) && IntQ(At(x,2)))
		{
			int a = Z::SI(At(x,2));
			a < 0 ? a += n : --a;
			if(a < 0 || a >= n)
				return r;
			S::Insert(r,At(x,1),a);
			return r;
		}
	}
	return 0;
}
Wrap(StringTake)
{
	var c = At(x,0);
	if(StrQ(c))
	{
		uint n = S::Length(c);
		var d = At(x,1);
		if(VecQ(d))
		{
			int a = Z::SI(At(d,0));
			a < 0 ? a += n : --a;
			if(a < 0 || a >= n)
				return c;
			if(Size(d) > 1)
			{
				int b = Z::SI(At(d,1));
				b < 0 ? b += n : --b;
				if(b < 0 || b >= n)
					return c;
				return S::Substr(c,a,b < a ? 0 : b - a + 1);
			}
			else
				return S::Substr(c,a,1);
		}
		else if(IntQ(d))
		{
			int a = Z::SI(d);
			return a < 0 ? S::Substr(c,a + n,-a) : S::Substr(c,0,a);
		}
	}
	return 0;
}
Wrap(StringDrop)
{
	var c = At(x,0);
	if(StrQ(c))
	{
		uint n = S::Length(c);
		var r = Str();
		CStr(r) = CStr(c);
		var d = At(x,1);
		if(VecQ(d))
		{
			int a = Z::SI(At(d,0));
			a < 0 ? a += n : --a;
			if(a < 0 || a >= n)
				return r;
			if(Size(d) > 1)
			{
				int b = Z::SI(At(d,1));
				b < 0 ? b += n : --b;
				if(b < 0 || b >= n)
					return r;
				S::Erase(r,a,b < a ? 0 : b - a + 1);
			}
			else
				S::Erase(r,a,1);
		}
		else if(IntQ(d))
		{
			int a = Z::SI(d);
			a < 0 ? S::Erase(r,a + n,-a) : S::Erase(r,0,a);
		}
		return r;
	}
	return 0;
}
Wrap2(Function)
{
	var c = At(x,0);
	if(Size(x) == 1)
		return Eval(Supply(Ex(TAG(Function),c),c,y));
	map_t m;
	if(VecQ(c))
	{
		size_t n = Size(c);
		for(size_t i = 0; i < n; ++i)
			m[At(c,i)] = At(y,i);
	}
	else
		m[c] = At(y,0);
	return Eval(Subs(m,At(x,1)));
}
}
//void(*Types[TYPE_SIZE])(Var);
var Contexts;
stdext::hash_map<Var,const wchar*> ContextName;
std::stack<Var> ContextStack;
std::stack<std::list<Var> > ContextPath;
map_t OwnValues;
std::map<Var,dict_t> FactValues;
std::map<Var,def_t> DownValues;
std::map<Var,def_t> SubValues;
std::map<Var,map_t> Properties;
std::map<Var,attr_t> Attributes;
stdext::hash_map<Var,CProc> CProcs;
stdext::hash_map<Var,COper> COpers;
var
Global, System, Null, True, False, Nil,
Constant, Flat, HoldAll, HoldAllComplete, HoldFirst,
HoldRest, Listable, Locked, NHoldAll, NHoldFirst,
NHoldRest, NumericFunction, OneIdentity, Orderless, Protected,
ReadProtected, SequenceHold, Stub, Temporary,
TAG(Symbol), TAG(List), TAG(String), TAG(Integer), TAG(Rational), TAG(Real),
TAG(Postfix), TAG(Prefix), TAG(Differential),
TAG(Minus), TAG(Divide), TAG(Plus), TAG(Times), TAG(Power),
TAG(Return), TAG(Continue), TAG(Break),
TAG(Set), TAG(Rule), TAG(Pattern), TAG(Blank), TAG(Optional), TAG(Condition),
TAG(PatternTest), TAG(Unevaluated), TAG(Derivative), TAG(Function), TAG(Slot),
TAG(BlankSequence), TAG(SlotSequence), TAG(Part), TAG(Property), TAG(Sequence),
TAG(Sqrt), TAG(Radical), TAG(RuleDelayed), TAG(SetDelayed),
TAG(CompoundExpression), TAG(Integrate), TAG(D), TAG(Sum), TAG(Limit),
TAG(Infinity), TAG(Pi), TAG(E), TAG(I), TAG(Mod), TAG(Dot), TAG(Pow),
TAG(Timing),
TAG(And), TAG(Or), TAG(Not), TAG(If), TAG(For), TAG(While),
TAG(Flatten), TAG(FlattenAll), TAG(SameQ), TAG(Less), TAG(Equal), TAG(Greater),
TAG(UnsameQ), TAG(GreaterEqual), TAG(Unequal), TAG(LessEqual), TAG(FreeQ),
TAG(MatchQ), TAG(MemberQ), TAG(With), TAG(Block), TAG(Module),
TAG(ReplaceRepeated), TAG(Replace), TAG(ReplaceAll), TAG(OddQ), TAG(EvenQ),
TAG(SetAttributes), TAG(StringJoin), TAG(Join), TAG(Attributes),
TAG(Get), TAG(Put), TAG(PutAppend),
TAG(Install), TAG(Uninstall),
TAG(NumberQ), TAG(AtomQ), TAG(IntegerQ), TAG(SymbolQ), TAG(StringQ),
TAG(Pretty), TAG(Input), TAG(Print), TAG(Clear),
TAG(BeginPackage), TAG(EndPackage), TAG(Begin), TAG(End),
TAG(Evaluate), TAG(Dispatch), TAG(Length), TAG(Path), TAG(Head), TAG(Context),
TAG(Contexts), TAG(ContextPath), TAG(Apply), TAG(Map), TAG(Unset), TAG(Full),
TAG(ToString), TAG(ToExpression),
TAG(Exit), TAG(Quit), TAG(Hold), TAG(Run), /*TAG(Task), TAG(Kill),*/ TAG(Array),
TAG(Table), TAG(Do), TAG(Box), TAG(N), TAG(IntegerPart),
TAG(Floor), TAG(Ceiling), TAG(Round),
TAG(Expand), TAG(Variables), TAG(Coefficient),
TAG(Exponent), TAG(Deg), TAG(CoefficientList), TAG(FromCoefficientList),
TAG(Graphics2D), TAG(Graphics3D),
TAG(Options), TAG(StringLength), TAG(StringInsert), TAG(StringTake),
TAG(StringDrop);
void Initialize()
{
	static bool Initialized = false;
	if(Initialized) return;

	Contexts = Tab();
	Global = Ctx(L"Global`");
	System = Ctx(L"System`");
#define T(x) x = Sym(_W(#x),System);
	T(Null)T(True)T(False)T(Nil)
	T(Constant)T(Flat)T(HoldAll)T(HoldAllComplete)T(HoldFirst)
	T(HoldRest)T(Listable)T(Locked)T(NHoldAll)T(NHoldFirst)
	T(NHoldRest)T(NumericFunction)T(OneIdentity)T(Orderless)T(Protected)
	T(ReadProtected)T(SequenceHold)T(Stub)T(Temporary)
#undef T
#define T(x) TAG(x) = Sym(_W(#x),System);
	T(Symbol)T(List)T(String)T(Integer)T(Rational)
	T(Real)T(Return)T(Continue)T(Break)T(Set)T(Rule)
	T(Minus)T(Divide)T(Plus)T(Times)T(Power)T(Integrate)T(D)
	T(Postfix)T(Prefix)T(Differential)T(Pattern)T(Blank)
	T(Optional)T(Condition)T(PatternTest)T(Unevaluated)
	T(Derivative)T(Function)T(Slot)T(BlankSequence)T(SlotSequence)
	T(Part)T(Property)T(Sequence)T(RuleDelayed)T(SetDelayed)
	T(CompoundExpression)T(Sum)T(Limit)T(Infinity)T(Pi)
	T(E)T(I)T(Sqrt)T(Radical)T(Mod)T(Dot)T(Pow)
	T(Timing)T(And)T(Or)T(Not)T(If)T(For)T(While)T(Flatten)T(FlattenAll)
	T(SameQ)T(Less)T(Equal)T(Greater)T(UnsameQ)T(GreaterEqual)T(Unequal)T(LessEqual)
	T(FreeQ)T(MatchQ)T(MemberQ)T(With)T(Block)T(Module)T(ReplaceRepeated)
	T(Replace)T(ReplaceAll)T(OddQ)T(EvenQ)T(SetAttributes)T(StringJoin)T(Join)T(Attributes)
	T(Get)T(Put)T(PutAppend)T(Install)T(Uninstall)T(NumberQ)T(AtomQ)T(IntegerQ)
	T(SymbolQ)T(StringQ)T(Pretty)T(Input)T(Print)T(Clear)T(BeginPackage)T(EndPackage)
	T(Begin)T(End)T(Evaluate)T(Dispatch)T(Length)T(Path)T(Head)T(Context)
	T(Contexts)T(ContextPath)T(Apply)T(Map)T(Unset)T(Full)T(ToString)T(ToExpression)
	T(Exit)T(Quit)T(Hold)T(Run)/*T(Task)T(Kill)*/T(Array)T(Table)T(Do)T(Box)
	T(N)T(IntegerPart)T(Floor)T(Ceiling)T(Round)T(Expand)T(Variables)T(Coefficient)
	T(Exponent)T(Deg)T(CoefficientList)T(FromCoefficientList)T(Graphics2D)T(Graphics3D)
	T(Options)T(StringLength)T(StringInsert)T(StringTake)T(StringDrop)
#undef T
#define T(x) CProcs[TAG(x)] = x;
	T(Plus)T(Times)T(Dot)
#undef T
#define T(x) CProcs[TAG(x)] = WRAP(x);
	T(Timing)T(And)T(Or)T(Not)T(If)T(For)T(While)T(Flatten)T(FlattenAll)
	T(SameQ)T(Less)T(Equal)T(Greater)T(UnsameQ)T(GreaterEqual)T(Unequal)T(LessEqual)
	T(FreeQ)T(MatchQ)T(MemberQ)T(With)T(Block)T(Module)T(ReplaceRepeated)
	T(Replace)T(ReplaceAll)T(OddQ)T(EvenQ)T(SetAttributes)T(StringJoin)T(Join)T(Attributes)
	T(Get)T(Put)T(PutAppend)T(Install)T(Uninstall)T(NumberQ)T(AtomQ)T(IntegerQ)
	T(SymbolQ)T(StringQ)T(Pretty)T(Input)T(Print)T(Clear)T(BeginPackage)T(EndPackage)
	T(Begin)T(End)T(Evaluate)T(Unevaluated)T(Dispatch)T(Length)T(Path)T(Head)T(Context)
	T(Contexts)T(ContextPath)T(Apply)T(Map)T(Unset)T(Full)T(ToString)T(ToExpression)
	T(Exit)T(Quit)T(Set)T(SetDelayed)T(Hold)T(Run)/*T(Task)T(Kill)*/T(Array)T(Table)T(Do)
	T(N)T(IntegerPart)T(Floor)T(Ceiling)T(Round)T(Expand)T(Variables)T(Coefficient)
	T(Exponent)T(Deg)T(CoefficientList)T(FromCoefficientList)T(StringLength)
	T(Set)T(Part)T(Property)T(SetDelayed)T(CompoundExpression)T(Power)T(Mod)
	T(StringInsert)T(StringTake)T(StringDrop)
#undef T
#define T(x,y) Attributes[TAG(x)].insert(y);
	T(CompoundExpression,HoldAll)T(Timing,HoldAll)T(For,HoldAll)T(While,HoldAll)
	T(Plus,OneIdentity)T(Plus,Flat)T(Plus,Orderless)T(Plus,Listable)
	T(Times,OneIdentity)T(Times,Flat)T(Times,Orderless)T(Times,Listable)
	T(Power,OneIdentity)T(Power,Listable)T(Pi,Constant)T(E,Constant)
	T(Set,HoldFirst)T(SetDelayed,HoldAll)T(Hold,HoldAll)
	T(And,OneIdentity)T(And,Flat)T(And,HoldAll)T(Or,OneIdentity)T(Or,Flat)T(Or,HoldAll)
	T(If,HoldRest)T(Pattern,HoldFirst)T(Condition,HoldAll)T(PatternTest,HoldRest)
	T(With,HoldAll)T(Block,HoldAll)T(Module,HoldAll)
	T(SetAttributes,HoldFirst)T(StringJoin,Flat)T(StringJoin,OneIdentity)
	T(Join,Flat)T(Join,OneIdentity)T(Attributes,HoldAll)T(Clear,HoldAll)
	T(Unevaluated,HoldAll)T(Function,HoldAll)T(Context,HoldFirst)T(Property,HoldAll)
	T(RuleDelayed,HoldRest)T(Unset,HoldFirst)/*T(Task,HoldAll)*/T(Table,HoldAll)T(Do,HoldAll)
#undef T
#define T(x) COpers[TAG(x)] = WRAP(x);
	T(Function)
#undef T
	ContextPath.push(std::list<Var>());
	ContextPath.top().push_back(System);
	ContextPath.top().push_back(Global);
	ContextStack.push(Global);
	Initialized = true;
}
}
DLLMAIN(mU::Initialize)

using namespace mU;
#define SYS(x) Sym(_W(#x),System)
CAPI void Install() {
	CProcs[SYS(CProc)] = (CProc)cfunc(cnoload("kernel"), SYS(CProc));
	Begin(System);
	ParseFile(Path() + _W("Kernel/Kernel.u"));
	End();
}
CAPI void Uninstall() {
	wcout << "#Kernel# Uninstall Successfully!" << std::endl;
}
