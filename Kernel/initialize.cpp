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
	if (Initialized) return;

	Contexts = Tab();
	Global = Ctx(L"Global`");
	System = Ctx(L"System`");

	DEF_SYSTEM_SYM(Null)
	DEF_SYSTEM_SYM(True)
	DEF_SYSTEM_SYM(False)
	DEF_SYSTEM_SYM(Nil)
	DEF_SYSTEM_SYM(Constant)
	DEF_SYSTEM_SYM(Flat)
	DEF_SYSTEM_SYM(HoldAll)
	DEF_SYSTEM_SYM(HoldAllComplete)
	DEF_SYSTEM_SYM(HoldFirst)
	DEF_SYSTEM_SYM(HoldRest)
	DEF_SYSTEM_SYM(Listable)
	DEF_SYSTEM_SYM(Locked)
	DEF_SYSTEM_SYM(NHoldAll)
	DEF_SYSTEM_SYM(NHoldFirst)
	DEF_SYSTEM_SYM(NHoldRest)
	DEF_SYSTEM_SYM(NumericFunction)
	DEF_SYSTEM_SYM(OneIdentity)
	DEF_SYSTEM_SYM(Orderless)
	DEF_SYSTEM_SYM(Protected)
	DEF_SYSTEM_SYM(ReadProtected)
	DEF_SYSTEM_SYM(SequenceHold)
	DEF_SYSTEM_SYM(Stub)
	DEF_SYSTEM_SYM(Temporary)

	DEF_SYSTEM_TAG_SYM(Symbol)
	DEF_SYSTEM_TAG_SYM(List)
	DEF_SYSTEM_TAG_SYM(String)
	DEF_SYSTEM_TAG_SYM(Integer)
	DEF_SYSTEM_TAG_SYM(Rational)
	DEF_SYSTEM_TAG_SYM(Real)
	DEF_SYSTEM_TAG_SYM(Return)
	DEF_SYSTEM_TAG_SYM(Continue)
	DEF_SYSTEM_TAG_SYM(Break)
	DEF_SYSTEM_TAG_SYM(Set)
	DEF_SYSTEM_TAG_SYM(Rule)
	DEF_SYSTEM_TAG_SYM(Minus)
	DEF_SYSTEM_TAG_SYM(Divide)
	DEF_SYSTEM_TAG_SYM(Plus)
	DEF_SYSTEM_TAG_SYM(Times)
	DEF_SYSTEM_TAG_SYM(Power)
	DEF_SYSTEM_TAG_SYM(Integrate)
	DEF_SYSTEM_TAG_SYM(D)
	DEF_SYSTEM_TAG_SYM(Postfix)
	DEF_SYSTEM_TAG_SYM(Prefix)
	DEF_SYSTEM_TAG_SYM(Differential)
	DEF_SYSTEM_TAG_SYM(Pattern)
	DEF_SYSTEM_TAG_SYM(Blank)
	DEF_SYSTEM_TAG_SYM(Optional)
	DEF_SYSTEM_TAG_SYM(Condition)
	DEF_SYSTEM_TAG_SYM(PatternTest)
	DEF_SYSTEM_TAG_SYM(Unevaluated)
	DEF_SYSTEM_TAG_SYM(Derivative)
	DEF_SYSTEM_TAG_SYM(Function)
	DEF_SYSTEM_TAG_SYM(Slot)
	DEF_SYSTEM_TAG_SYM(BlankSequence)
	DEF_SYSTEM_TAG_SYM(SlotSequence)
	DEF_SYSTEM_TAG_SYM(Part)
	DEF_SYSTEM_TAG_SYM(Property)
	DEF_SYSTEM_TAG_SYM(Sequence)
	DEF_SYSTEM_TAG_SYM(RuleDelayed)
	DEF_SYSTEM_TAG_SYM(SetDelayed)
	DEF_SYSTEM_TAG_SYM(CompoundExpression)
	DEF_SYSTEM_TAG_SYM(Sum)
	DEF_SYSTEM_TAG_SYM(Limit)
	DEF_SYSTEM_TAG_SYM(Infinity)
	DEF_SYSTEM_TAG_SYM(Pi)
	DEF_SYSTEM_TAG_SYM(E)
    DEF_SYSTEM_TAG_SYM(I)
    DEF_SYSTEM_TAG_SYM(Sqrt)
    DEF_SYSTEM_TAG_SYM(Radical)
    DEF_SYSTEM_TAG_SYM(Mod)
    DEF_SYSTEM_TAG_SYM(Dot)
    DEF_SYSTEM_TAG_SYM(Pow)
	DEF_SYSTEM_TAG_SYM(Timing)
    DEF_SYSTEM_TAG_SYM(And)
    DEF_SYSTEM_TAG_SYM(Or)
    DEF_SYSTEM_TAG_SYM(Not)
    DEF_SYSTEM_TAG_SYM(If)
    DEF_SYSTEM_TAG_SYM(For)
    DEF_SYSTEM_TAG_SYM(While)
    DEF_SYSTEM_TAG_SYM(Flatten)
    DEF_SYSTEM_TAG_SYM(FlattenAll)
	DEF_SYSTEM_TAG_SYM(SameQ)
    DEF_SYSTEM_TAG_SYM(Less)
    DEF_SYSTEM_TAG_SYM(Equal)
    DEF_SYSTEM_TAG_SYM(Greater)
    DEF_SYSTEM_TAG_SYM(UnsameQ)
    DEF_SYSTEM_TAG_SYM(GreaterEqual)
    DEF_SYSTEM_TAG_SYM(Unequal)
    DEF_SYSTEM_TAG_SYM(LessEqual)
	DEF_SYSTEM_TAG_SYM(FreeQ)
    DEF_SYSTEM_TAG_SYM(MatchQ)
    DEF_SYSTEM_TAG_SYM(MemberQ)
    DEF_SYSTEM_TAG_SYM(With)
    DEF_SYSTEM_TAG_SYM(Block)
    DEF_SYSTEM_TAG_SYM(Module)
    DEF_SYSTEM_TAG_SYM(ReplaceRepeated)
	DEF_SYSTEM_TAG_SYM(Replace)
    DEF_SYSTEM_TAG_SYM(ReplaceAll)
    DEF_SYSTEM_TAG_SYM(OddQ)
    DEF_SYSTEM_TAG_SYM(EvenQ)
    DEF_SYSTEM_TAG_SYM(SetAttributes)
    DEF_SYSTEM_TAG_SYM(StringJoin)
    DEF_SYSTEM_TAG_SYM(Join)
    DEF_SYSTEM_TAG_SYM(Attributes)
	DEF_SYSTEM_TAG_SYM(Get)
    DEF_SYSTEM_TAG_SYM(Put)
    DEF_SYSTEM_TAG_SYM(PutAppend)
    DEF_SYSTEM_TAG_SYM(Install)
    DEF_SYSTEM_TAG_SYM(Uninstall)
    DEF_SYSTEM_TAG_SYM(NumberQ)
    DEF_SYSTEM_TAG_SYM(AtomQ)
    DEF_SYSTEM_TAG_SYM(IntegerQ)
	DEF_SYSTEM_TAG_SYM(SymbolQ)
    DEF_SYSTEM_TAG_SYM(StringQ)
    DEF_SYSTEM_TAG_SYM(Pretty)
    DEF_SYSTEM_TAG_SYM(Input)
    DEF_SYSTEM_TAG_SYM(Print)
    DEF_SYSTEM_TAG_SYM(Clear)
    DEF_SYSTEM_TAG_SYM(BeginPackage)
    DEF_SYSTEM_TAG_SYM(EndPackage)
	DEF_SYSTEM_TAG_SYM(Begin)
    DEF_SYSTEM_TAG_SYM(End)
    DEF_SYSTEM_TAG_SYM(Evaluate)
    DEF_SYSTEM_TAG_SYM(Dispatch)
    DEF_SYSTEM_TAG_SYM(Length)
    DEF_SYSTEM_TAG_SYM(Path)
    DEF_SYSTEM_TAG_SYM(Head)
    DEF_SYSTEM_TAG_SYM(Context)
	DEF_SYSTEM_TAG_SYM(Contexts)
    DEF_SYSTEM_TAG_SYM(ContextPath)
    DEF_SYSTEM_TAG_SYM(Apply)
    DEF_SYSTEM_TAG_SYM(Map)
    DEF_SYSTEM_TAG_SYM(Unset)
    DEF_SYSTEM_TAG_SYM(Full)
    DEF_SYSTEM_TAG_SYM(ToString)
    DEF_SYSTEM_TAG_SYM(ToExpression)
	DEF_SYSTEM_TAG_SYM(Exit)
    DEF_SYSTEM_TAG_SYM(Quit)
    DEF_SYSTEM_TAG_SYM(Hold)
    DEF_SYSTEM_TAG_SYM(Run)
    //DEF_SYSTEM_TAG_SYM(Task)
    //DEF_SYSTEM_TAG_SYM(Kill)
    DEF_SYSTEM_TAG_SYM(Array)
    DEF_SYSTEM_TAG_SYM(Table)
    DEF_SYSTEM_TAG_SYM(Do)
    DEF_SYSTEM_TAG_SYM(Box)
	DEF_SYSTEM_TAG_SYM(N)
    DEF_SYSTEM_TAG_SYM(IntegerPart)
    DEF_SYSTEM_TAG_SYM(Floor)
    DEF_SYSTEM_TAG_SYM(Ceiling)
    DEF_SYSTEM_TAG_SYM(Round)
    DEF_SYSTEM_TAG_SYM(Expand)
    DEF_SYSTEM_TAG_SYM(Variables)
    DEF_SYSTEM_TAG_SYM(Coefficient)
	DEF_SYSTEM_TAG_SYM(Exponent)
    DEF_SYSTEM_TAG_SYM(Deg)
    DEF_SYSTEM_TAG_SYM(CoefficientList)
    DEF_SYSTEM_TAG_SYM(FromCoefficientList)
    DEF_SYSTEM_TAG_SYM(Graphics2D)
    DEF_SYSTEM_TAG_SYM(Graphics3D)
	DEF_SYSTEM_TAG_SYM(Options)
    DEF_SYSTEM_TAG_SYM(StringLength)
    DEF_SYSTEM_TAG_SYM(StringInsert)
    DEF_SYSTEM_TAG_SYM(StringTake)
    DEF_SYSTEM_TAG_SYM(StringDrop)

	DEF_CPROC(Plus)
    DEF_CPROC(Times)
    DEF_CPROC(Dot)

	DEF_WRAPPED_CPROC(Timing)
    DEF_WRAPPED_CPROC(And)
    DEF_WRAPPED_CPROC(Or)
    DEF_WRAPPED_CPROC(Not)
    DEF_WRAPPED_CPROC(If)
    DEF_WRAPPED_CPROC(For)
    DEF_WRAPPED_CPROC(While)
    DEF_WRAPPED_CPROC(Flatten)
    DEF_WRAPPED_CPROC(FlattenAll)
	DEF_WRAPPED_CPROC(SameQ)
    DEF_WRAPPED_CPROC(Less)
    DEF_WRAPPED_CPROC(Equal)
    DEF_WRAPPED_CPROC(Greater)
    DEF_WRAPPED_CPROC(UnsameQ)
    DEF_WRAPPED_CPROC(GreaterEqual)
    DEF_WRAPPED_CPROC(Unequal)
    DEF_WRAPPED_CPROC(LessEqual)
	DEF_WRAPPED_CPROC(FreeQ)
    DEF_WRAPPED_CPROC(MatchQ)
    DEF_WRAPPED_CPROC(MemberQ)
    DEF_WRAPPED_CPROC(With)
    DEF_WRAPPED_CPROC(Block)
    DEF_WRAPPED_CPROC(Module)
    DEF_WRAPPED_CPROC(ReplaceRepeated)
	DEF_WRAPPED_CPROC(Replace)
    DEF_WRAPPED_CPROC(ReplaceAll)
    DEF_WRAPPED_CPROC(OddQ)
    DEF_WRAPPED_CPROC(EvenQ)
    DEF_WRAPPED_CPROC(SetAttributes)
    DEF_WRAPPED_CPROC(StringJoin)
    DEF_WRAPPED_CPROC(Join)
    DEF_WRAPPED_CPROC(Attributes)
	DEF_WRAPPED_CPROC(Get)
    DEF_WRAPPED_CPROC(Put)
    DEF_WRAPPED_CPROC(PutAppend)
    DEF_WRAPPED_CPROC(Install)
    DEF_WRAPPED_CPROC(Uninstall)
    DEF_WRAPPED_CPROC(NumberQ)
    DEF_WRAPPED_CPROC(AtomQ)
    DEF_WRAPPED_CPROC(IntegerQ)
	DEF_WRAPPED_CPROC(SymbolQ)
    DEF_WRAPPED_CPROC(StringQ)
    DEF_WRAPPED_CPROC(Pretty)
    DEF_WRAPPED_CPROC(Input)
    DEF_WRAPPED_CPROC(Print)
    DEF_WRAPPED_CPROC(Clear)
    DEF_WRAPPED_CPROC(BeginPackage)
    DEF_WRAPPED_CPROC(EndPackage)
	DEF_WRAPPED_CPROC(Begin)
    DEF_WRAPPED_CPROC(End)
    DEF_WRAPPED_CPROC(Evaluate)
    DEF_WRAPPED_CPROC(Unevaluated)
    DEF_WRAPPED_CPROC(Dispatch)
    DEF_WRAPPED_CPROC(Length)
    DEF_WRAPPED_CPROC(Path)
    DEF_WRAPPED_CPROC(Head)
    DEF_WRAPPED_CPROC(Context)
	DEF_WRAPPED_CPROC(Contexts)
    DEF_WRAPPED_CPROC(ContextPath)
    DEF_WRAPPED_CPROC(Apply)
    DEF_WRAPPED_CPROC(Map)
    DEF_WRAPPED_CPROC(Unset)
    DEF_WRAPPED_CPROC(Full)
    DEF_WRAPPED_CPROC(ToString)
    DEF_WRAPPED_CPROC(ToExpression)
	DEF_WRAPPED_CPROC(Exit)
    DEF_WRAPPED_CPROC(Quit)
    DEF_WRAPPED_CPROC(Set)
    DEF_WRAPPED_CPROC(SetDelayed)
    DEF_WRAPPED_CPROC(Hold)
    DEF_WRAPPED_CPROC(Run)
    //DEF_WRAPPED_CPROC(Task)
    //DEF_WRAPPED_CPROC(Kill)
    DEF_WRAPPED_CPROC(Array)
    DEF_WRAPPED_CPROC(Table)
    DEF_WRAPPED_CPROC(Do)
	DEF_WRAPPED_CPROC(N)
    DEF_WRAPPED_CPROC(IntegerPart)
    DEF_WRAPPED_CPROC(Floor)
    DEF_WRAPPED_CPROC(Ceiling)
    DEF_WRAPPED_CPROC(Round)
    DEF_WRAPPED_CPROC(Expand)
    DEF_WRAPPED_CPROC(Variables)
    DEF_WRAPPED_CPROC(Coefficient)
	DEF_WRAPPED_CPROC(Exponent)
    DEF_WRAPPED_CPROC(Deg)
    DEF_WRAPPED_CPROC(CoefficientList)
    DEF_WRAPPED_CPROC(FromCoefficientList)
    DEF_WRAPPED_CPROC(StringLength)
	DEF_WRAPPED_CPROC(Set)
    DEF_WRAPPED_CPROC(Part)
    DEF_WRAPPED_CPROC(Property)
    DEF_WRAPPED_CPROC(SetDelayed)
    DEF_WRAPPED_CPROC(CompoundExpression)
    DEF_WRAPPED_CPROC(Power)
    DEF_WRAPPED_CPROC(Mod)

	DEF_WRAPPED_COPER(Function)

	SET_ATTR(CompoundExpression,HoldAll)
    SET_ATTR(Timing,HoldAll)
    SET_ATTR(For,HoldAll)
    SET_ATTR(While,HoldAll)
	SET_ATTR(Plus,OneIdentity)
    SET_ATTR(Plus,Flat)
    SET_ATTR(Plus,Orderless)
    SET_ATTR(Plus,Listable)
	SET_ATTR(Times,OneIdentity)
    SET_ATTR(Times,Flat)
    SET_ATTR(Times,Orderless)
    SET_ATTR(Times,Listable)
	SET_ATTR(Power,OneIdentity)
    SET_ATTR(Power,Listable)
    SET_ATTR(Pi,Constant)
    SET_ATTR(E,Constant)
	SET_ATTR(Set,HoldFirst)
    SET_ATTR(SetDelayed,HoldAll)
    SET_ATTR(Hold,HoldAll)
	SET_ATTR(And,OneIdentity)
    SET_ATTR(And,Flat)
    SET_ATTR(And,HoldAll)
    SET_ATTR(Or,OneIdentity)
    SET_ATTR(Or,Flat)
    SET_ATTR(Or,HoldAll)
	SET_ATTR(If,HoldRest)
    SET_ATTR(Pattern,HoldFirst)
    SET_ATTR(Condition,HoldAll)
    SET_ATTR(PatternTest,HoldRest)
	SET_ATTR(With,HoldAll)
    SET_ATTR(Block,HoldAll)
    SET_ATTR(Module,HoldAll)
	SET_ATTR(SetAttributes,HoldFirst)
    SET_ATTR(StringJoin,Flat)
    SET_ATTR(StringJoin,OneIdentity)
	SET_ATTR(Join,Flat)
    SET_ATTR(Join,OneIdentity)
    SET_ATTR(Attributes,HoldAll)
    SET_ATTR(Clear,HoldAll)
	SET_ATTR(Unevaluated,HoldAll)
    SET_ATTR(Function,HoldAll)
    SET_ATTR(Context,HoldFirst)
    SET_ATTR(Property,HoldAll)
	SET_ATTR(RuleDelayed,HoldRest)
    SET_ATTR(Unset,HoldFirst)
    //SET_ATTR(Task,HoldAll)
    SET_ATTR(Table,HoldAll)
    SET_ATTR(Do,HoldAll)

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
