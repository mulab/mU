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
#include <sys/time.h>
#define API
#endif

#undef ASSIGN
#define ASSIGN(f) CASSIGN(System,f)
#undef VALUE
#define VALUE(f) CVALUE(System,f)
#undef METHOD
#define METHOD(f,sig) CMETHOD(System,f,sig)

namespace mU {
struct timer {
	double value;
#ifdef _MSC_VER
	timer() {
		QueryPerformanceFrequency(&Frequency);
	}
	void start() {
		QueryPerformanceCounter(&timerB);
	}
	void end() {
		QueryPerformanceCounter(&timerE);
		value = (double)(timerE.QuadPart - timerB.QuadPart)
			/ (double)Frequency.QuadPart * 1000000.0;
	}
	LARGE_INTEGER timerB, timerE, Frequency;
#else
	timer() {}
	void start() {
		gettimeofday(&timerB, NULL);
	}
	void end() {
		gettimeofday(&timerE, NULL);
		value = (double)((timerE.tv_usec - timerB.tv_usec)
			+ (timerE.tv_sec - timerB.tv_sec) * 1000000);
	}
	timeval timerB, timerE;
#endif
};
API wstring Path();
API bool Shell(wcs);
API double Timing(Kernel&, var&, const var&);
API bool In(Kernel&, var&, wcs);
API bool Out(Kernel&, const var&, wcs);
API bool OutAppend(Kernel&, const var&, wcs);
API int Order(Kernel&, const var&, const var&);
}
/*
API void SetAttributes(const var&, const var&);
API var Ctx(const wstring&);
API var Sym(const wstring&);
API var Ctx2(wcs);
API var Sym2(wcs);
API var Unique(const var&);
API var context();
API void beginContext(const var&);
API void endContext();
API void beginContextPackage(const var&);
API void endContextPackage();
API void beginContextClass(const var&);
API void endContextClass();
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
API wstring Path();
API var Install(const wstring&);
API bool Uninstall(const var&);
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
API var With(const var&,const var&);
API var Block(const var&,const var&);
API var Module(const var&,const var&);
inline bool OddQ(const var& x) {
    return IntQ(x) && mpz_odd_p(CI(x));
}
inline bool EvenQ(const var& x) {
    return IntQ(x) && mpz_even_p(CI(x));
}
API var Plus(const var&);
API var Plus(const var&, const var&);
API var Times(const var&);
API var Times(const var&, const var&);
API var Power(const var&, const var&);
API var Mod(const var&, const var&);
API void Do(const var&, size, const var*);
API var Table(const var&, size, const var*);
API var Array(const var&, const var&, size, const var*);
API var Array(const var&, const var&, size, const var*, const var*);
API var Evalf(const var&);
API var Evalf(const var&, size);
API var IPart(const var&);
API var Floor(const var&);
API var Ceiling(const var&);
API var Round(const var&);
API void  Expand(const var&, const var&, const var&);
API var  Expand(const var&, const var&);
API var Expand(const var&);
API var const var&iables(const var&);
API void  Coefficient(const var&, const var&, const var&);
API var Coefficient(const var&, const var&);
API var Exponent(const var&, const var&);
API var Deg(const var&, const var&);
API var Coefficients(const var&, const var&);
API var CoefficientList(const var&, const var&);
API var FromCoefficients(const var&, const var&);
API var FromCoefficientList(const var&, const var&);
*/
