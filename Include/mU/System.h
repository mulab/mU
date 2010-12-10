#pragma once
#include "Interface.h"
#include "Kernel.h"

#undef API
#ifdef _MSC_VER
#ifdef SYSTEM_EXPORTS
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#else
#define API
#endif

#undef ASSIGN
#define ASSIGN(f) CASSIGN(System,f)
#undef VALUE
#define VALUE(f) CVALUE(System,f)
#undef METHOD
#define METHOD(f,sig) CMETHOD(System,f,sig)

namespace mU {
API var Attributes(Kernel&, sym);
API bool FreeQ(const var&, const var&);
bool Input(Kernel&, var&, wcs);
inline bool Input(Kernel& k, var& r, const wstring& s) {
	return Input(k, r, s.c_str());
} 
bool Output(Kernel&, const var&, wcs);
inline bool Output(Kernel& k, const var& r, const wstring& s) {
	return Output(k, r, s.c_str());
} 
bool OutputAppend(Kernel&, const var&, wcs);
inline bool OutputAppend(Kernel& k, const var& r, const wstring& s) {
	return OutputAppend(k, r, s.c_str());
} 
API double Timing(Kernel&, var&, const var&);
template <class T>
var With(const var& x, const T& m) {
	typename T::const_iterator
		iter = m.find(x);
	if (iter != m.end())
		return iter->second;
	if (x.isTuple()) {
		Tuple* r = x.tuple().clone();
		for (uint i = 0; i < x.tuple().size; ++i)
			r->tuple[i] = With(x.tuple()[i], m);
		return r;
	}
	return x;
}
}
/*
API void SetAttributes(const var&, const var&);
API void beginClass(const var&);
API void endClass();
API var Property(const var&,const var&);
API var Property(const var&);
API var Method(const var&,const var&);
API var Method(const var&);
API var Read(wistream&, const var& = 0);
API var Parse(wistream& = wcin);
API var ParseString(const wstring&);
API var ParseFile(const wstring&);
API var Pretty(const var&);
API void Write(wostream&, const var&);
API void Print(const var&, wostream& = wcout, size = 0);
API void Println(const var&, wostream& = wcout);
API void FullPrint(const var&, wostream& = wcout);
API void BoxPrint(const var&, wostream& = wcout, size = 0);
API bool Run(const wstring&);
API var Task(const var&);
API bool Suspend(const var&);
API bool Resume(const var&);
API bool Stop(const var&);
API void Flatten(const var&, const var&);
API void Flatten(const var&, const var&, const var&);
API void FlattenAll(const var&, const var&);
API void FlattenAll(const var&, const var&, const var&);
API bool FixQ(const var&);
API var Supply(const var&, const var&, const var&);
API var Thread(const var&, const var&);
API var Join(const var&);
inline bool OddQ(const var& x) {
    return IntQ(x) && mpz_odd_p(CI(x));
}
inline bool EvenQ(const var& x) {
    return IntQ(x) && mpz_even_p(CI(x));
}
API var Power(const var&, const var&);
API var Mod(const var&, const var&);
API void Do(const var&, size, const var*);
API var Table(const var&, size, const var*);
API var Array(const var&, const var&, size, const var*);
API var Array(const var&, const var&, size, const var*, const var*);
API var IntegerPart(const var&);
API var Floor(const var&);
API var Ceiling(const var&);
API var Round(const var&);
API void  Expand(const var&, const var&, const var&);
API var  Expand(const var&, const var&);
API var Expand(const var&);
API var Variables(const var&);
API void  Coefficient(const var&, const var&, const var&);
API var Coefficient(const var&, const var&);
API var Exponent(const var&, const var&);
API var Deg(const var&, const var&);
API var Coefficients(const var&, const var&);
API var CoefficientList(const var&, const var&);
API var FromCoefficients(const var&, const var&);
API var FromCoefficientList(const var&, const var&);
*/
