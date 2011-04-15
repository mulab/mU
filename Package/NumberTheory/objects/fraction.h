#pragma once
#include "pair.h"

namespace maTHmU {
//////////////////////////////////////
class frac;
typedef const frac& FRAC;
//////////////////////////////////////
class VAR_API frac : public pair {
public:
	static bool type(VAR);
	static frac& cast(VAR);
	frac();
	frac(VAR,VAR);

	void print() const;
	frac copy() const;
	void reduce() const;
};
VAR_API frac operator + (FRAC);
VAR_API frac operator - (FRAC);
VAR_API frac operator + (FRAC,FRAC);
VAR_API frac operator - (FRAC,FRAC);
VAR_API frac operator * (FRAC,FRAC);
VAR_API frac operator / (FRAC,FRAC);
VAR_API bool operator < (FRAC,FRAC);
VAR_API bool operator == (FRAC,FRAC);
VAR_API bool operator > (FRAC,FRAC);
VAR_API bool operator >= (FRAC,FRAC);
VAR_API bool operator != (FRAC,FRAC);
VAR_API bool operator <= (FRAC,FRAC);
//////////////////////////////////////
namespace Objects {
namespace Fraction {
//////////////////////////////////////
extern VAR_API UINT FRACTION;
VAR_API var Zero();
VAR_API var New(VAR);
VAR_API var New(VAR,VAR);
VAR_API var Print(VAR);
VAR_API var Abs(VAR);
VAR_API var Neg(VAR);
VAR_API var Add(VAR,VAR);
VAR_API var Sub(VAR,VAR);
VAR_API var Mul(VAR,VAR);
VAR_API var Div(VAR,VAR);
VAR_API var Sc(VAR,INT);
VAR_API VOID Reduce(VAR);
VAR_API var& a(VAR);
VAR_API var& b(VAR);
VAR_API INT Cmp(VAR,VAR);
VAR_API VOID Init();
//////////////////////////////////////
}
}
}