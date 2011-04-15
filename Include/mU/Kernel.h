#pragma once
#include "Var.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#ifdef _MSC_VER
#define DLLMAIN_DECL
#else
#ifdef __MINGW32__
#define DLLMAIN_DECL extern "C"
#else
#error "unknown toolchain"
#endif
#endif
#define DLLMAIN(x)\
DLLMAIN_DECL BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)\
{\
switch (ul_reason_for_call)\
{\
case DLL_PROCESS_ATTACH: x(); break;\
case DLL_THREAD_ATTACH:\
case DLL_THREAD_DETACH:\
case DLL_PROCESS_DETACH:\
	break;\
}\
return TRUE;\
}
namespace mU {
//////////////////////////////////////
struct timer
{
	timer() { QueryPerformanceFrequency(&Frequency); }
	void start() { QueryPerformanceCounter(&timerB); }
	void end()
	{
		QueryPerformanceCounter(&timerE);
		value = (double)(timerE.QuadPart - timerB.QuadPart)
		/ (double)Frequency.QuadPart * 1000000.0;
	}
	LARGE_INTEGER timerB, timerE, Frequency;
	double value;
};
#else
#define DLLMAIN(x) extern "C" void DllMain(){x();}
#include <sys/time.h>
namespace mU {
//////////////////////////////////////
struct timer
{
	timer() {}
	void start() { gettimeofday(&timerB,NULL); }
	void end()
	{
        gettimeofday(&timerE,NULL);
        value = (double)((timerE.tv_usec - timerB.tv_usec)
        + (timerE.tv_sec - timerB.tv_sec) * 1000000);
	}
    timeval timerB, timerE;
    double value;
};
#endif
API void Initialize();
API int Compare(Var,Var);
inline bool Before(Var x, Var y) { return Compare(x,y) < 0; }
inline bool Same(Var x, Var y) { return Compare(x,y) == 0; }
inline bool After(Var x, Var y) { return Compare(x,y) > 0; }
struct Before2 { inline bool operator()(Var x, Var y) const { return Before(x,y); } };
struct After2 { inline bool operator()(Var x, Var y) const { return After(x,y); } };
API bool FreeQ(Var,Var);
API int Order(Var,Var);
inline bool Less(Var x, Var y) { return Order(x,y) < 0; }
inline bool Equal(Var x, Var y) { return Order(x,y) == 0; }
inline bool Greater(Var x, Var y) { return Order(x,y) > 0; }
struct Less2 { inline bool operator()(Var x, Var y) const { return Less(x,y); } };
struct Greater2 { inline bool operator()(Var x, Var y) const { return Greater(x,y); } };
inline void Sort(Var x) { std::sort(CVec(x).begin(),CVec(x).end(),Less); }
typedef std::map<Var,var> map_t;
typedef std::map<var,var,Before2> dict_t;
typedef std::map<var,std::pair<var,var>,Before2> def_t;
typedef std::set<Var> attr_t;
typedef var(*CProc)(Var);
typedef var(*COper)(Var,Var);
API map_t OwnValues;
API std::map<Var,dict_t> FactValues;
API std::map<Var,def_t> DownValues;
API std::map<Var,def_t> SubValues;
API std::map<Var,map_t> Properties;
API std::map<Var,attr_t> Attributes;
API stdext::hash_map<Var,CProc> CProcs;
API stdext::hash_map<Var,COper> COpers;
API var Eval(Var);
API void Set(Var,Var);
API void Unset(Var);
API void Clear(Var);
API var Thread(Var,Var);
API void Flatten(Var,Var);
API void Flatten(Var,Var,Var);
API void FlattenAll(Var,Var);
API void FlattenAll(Var,Var,Var);
API bool FixQ(Var);
API wstring Unique();
API var Supply(Var,Var,Var);
template <typename T>
var Subs(const T& m, Var x)
{
	typename T::const_iterator
        iter = m.find(x);
	if(iter != m.end())
		return iter->second;
	if(VecQ(x))
	{
		size_t n = Size(x);
		var r = Vec(n);
		for(size_t i = 0; i < n; ++i)
			At(r,i) = Subs(m,At(x,i));
		return r;
	}
	if(ExQ(x))
		return Ex(Subs(m,Head(x)),Subs(m,Body(x)));
	return x;
}
API var Contexts;
API stdext::hash_map<Var,const wchar*> ContextName;
API std::stack<Var> ContextStack;
API std::stack<std::list<Var> > ContextPath;
inline var Context() { return ContextStack.top(); }
inline void Begin(Var x) { ContextStack.push(x); }
inline void End() { ContextStack.pop(); }
API void BeginPackage(Var);
API void EndPackage();
API var Ctx(const wstring&);
API var Sym(const wstring&);
API var Ctx2(const wchar*);
API var Sym2(const wchar*);
API var Optimi(Var);
API var Read(wistream&,Var = 0);
API var Parse(wistream& = wcin);
inline var ParseString(const wstring &x)
{
    wistringstream i(x);
    return Parse(i);
}
inline var ParseFile(const wstring &x)
{
    wifstream i(string(x.begin(),x.end()).c_str());
    return Parse(i);
}
API var Pretty(Var);
API void Write(wostream&,Var);
API void Print(Var,wostream& = wcout,size_t = 0);
inline void Println(Var x, wostream &f = wcout)
{
	Print(Pretty(x),f);
	f << std::endl;
}
API void FullPrint(Var,wostream& = wcout);
API void BoxPrint(Var,wostream& = wcout,size_t = 0);
#define TAG(x) tag_##x
#define WRAP(x) wrap_##x
#define Wrap(f) var WRAP(f)(Var x)
#define Wrap2(f) var WRAP(f)(Var x, Var y)
API var
Global, System, Null, True, False, Nil,
Constant, Flat, HoldAll, HoldAllComplete, HoldFirst,
HoldRest, Listable, Locked, NHoldAll, NHoldFirst,
NHoldRest, NumericFunction, OneIdentity, Orderless, Protected,
ReadProtected, SequenceHold, Stub, Temporary,
TAG(Symbol), TAG(List), TAG(String), TAG(Integer), TAG(Rational), TAG(Real),
TAG(Postfix), TAG(Prefix), TAG(Differential), TAG(Minus), TAG(Divide), TAG(Plus),
TAG(Times), TAG(Power), TAG(Return), TAG(Continue), TAG(Break), TAG(Set), TAG(Rule),
TAG(Pattern), TAG(Blank), TAG(Optional), TAG(Condition), TAG(PatternTest),
TAG(Unevaluated), TAG(Derivative), TAG(Function), TAG(Slot), TAG(BlankSequence),
TAG(SlotSequence), TAG(Part), TAG(Property), TAG(Sequence), TAG(Sqrt), TAG(Radical),
TAG(RuleDelayed), TAG(SetDelayed), TAG(CompoundExpression), TAG(Integrate), TAG(D), TAG(Sum), TAG(Limit),
TAG(Infinity), TAG(Pi), TAG(E), TAG(I), TAG(Mod), TAG(Dot), TAG(Pow),
TAG(Timing), TAG(And), TAG(Or), TAG(Not), TAG(If), TAG(For), TAG(While), TAG(Flatten), TAG(FlattenAll), 
TAG(SameQ), TAG(Less), TAG(Equal), TAG(Greater), TAG(UnsameQ), TAG(GreaterEqual), TAG(Unequal), TAG(LessEqual), 
TAG(FreeQ), TAG(MatchQ), TAG(MemberQ), TAG(With), TAG(Block), TAG(Module), TAG(ReplaceRepeated), 
TAG(Replace), TAG(ReplaceAll), TAG(OddQ), TAG(EvenQ), TAG(SetAttributes), TAG(StringJoin), TAG(Join), TAG(Attributes), 
TAG(Get), TAG(Put), TAG(PutAppend), TAG(Install), TAG(Uninstall), TAG(NumberQ), TAG(AtomQ), TAG(IntegerQ), 
TAG(SymbolQ), TAG(StringQ), TAG(Pretty), TAG(Input), TAG(Print), TAG(Clear), TAG(BeginPackage), TAG(EndPackage), 
TAG(Begin), TAG(End), TAG(Evaluate), TAG(Dispatch), TAG(Length), TAG(Path), TAG(Head), TAG(Context), 
TAG(Contexts), TAG(ContextPath), TAG(Apply), TAG(Map), TAG(Unset), TAG(Full), TAG(ToString), TAG(ToExpression), 
TAG(Exit), TAG(Quit), TAG(Hold), TAG(Run), TAG(Task), TAG(Kill), TAG(Array), TAG(Table), TAG(Do), TAG(Box),
TAG(N), TAG(IntegerPart), TAG(Floor), TAG(Ceiling), TAG(Round), TAG(Expand), TAG(Variables), TAG(Coefficient), 
TAG(Exponent), TAG(Deg), TAG(CoefficientList), TAG(FromCoefficientList), TAG(Graphics2D), TAG(Graphics3D),
TAG(Options), TAG(StringLength), TAG(StringInsert), TAG(StringTake), TAG(StringDrop);
inline var Tag(Var x)
{
	switch(Type(x))
	{
	case TYPE(obj): return ((obj_t*)x)->tag();
	case TYPE(int): return TAG(Integer);
	case TYPE(rat): return TAG(Rational);
	case TYPE(flt): return TAG(Real);
	case TYPE(str): return TAG(String);
	case TYPE(sym): return TAG(Symbol);
	case TYPE(vec): return TAG(List);
	default:
		return Head(x);
	}
}
inline bool ZeroQ(Var x)
{
	return (IntQ(x) && mpz_cmp_ui(CInt(x),0L) == 0)
		|| (RatQ(x) && mpq_cmp_ui(CRat(x),0L,1L) == 0)
		|| (FltQ(x) && mpf_cmp_ui(CFlt(x),0L) == 0)
		;
}
inline bool OneQ(Var x)
{
	return (IntQ(x) && mpz_cmp_ui(CInt(x),1L) == 0)
		|| (RatQ(x) && mpq_cmp_ui(CRat(x),1L,1L) == 0)
		|| (FltQ(x) && mpf_cmp_ui(CFlt(x),1L) == 0)
		;
}
inline bool NOneQ(Var x)
{
	return (IntQ(x) && mpz_cmp_si(CInt(x),-1L) == 0)
		|| (RatQ(x) && mpq_cmp_si(CRat(x),-1L,1L) == 0)
		|| (FltQ(x) && mpf_cmp_si(CFlt(x),-1L) == 0)
		;
}
inline bool OddQ(Var x) { return IntQ(x) && mpz_odd_p(CInt(x)); }
inline bool EvenQ(Var x) { return IntQ(x) && mpz_even_p(CInt(x)); }
API var Plus(Var);
API var Plus(Var,Var);
API var Times(Var);
API var Times(Var,Var);
API var Power(Var,Var);
API var Mod(Var,Var);
API var Dot(Var);
API var Dot(Var,Var);
API void Do(Var,size_t,const var*);
API var Table(Var,size_t,const var*);
API var Array(Var,Var,size_t,const var*);
API var Array(Var,Var,size_t,const var*,const var*);
API var Evalf(Var);
API var Evalf(Var,size_t);
API var IntegerPart(Var);
API var Floor(Var);
API var Ceiling(Var);
API var Round(Var);
inline var Timing(Var x)
{
	timer t;
	t.start();
	var r = Eval(x);
	t.end();
	return Vec(Int(t.value),r);
}
API wstring Path();
API var Install(const wstring&);
API bool Uninstall(Var);
API bool Run(const wstring&);
API var Task(Var);
API bool Kill(Var);
API void  Expand(Var,Var,Var);
API var  Expand(Var,Var);
API var Expand(Var);
API var Variables(Var);
API void  Coefficient(Var,Var,Var);
API var Coefficient(Var,Var);
API var Exponent(Var,Var);
API var Deg(Var,Var);
API var Coefficients(Var,Var);
API var CoefficientList(Var,Var);
API var FromCoefficients(Var,Var);
API var FromCoefficientList(Var,Var);
//////////////////////////////////////
}
