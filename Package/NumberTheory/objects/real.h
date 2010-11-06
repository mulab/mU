#pragma once
#include "var.h"

namespace maTHmU {
//////////////////////////////////////
class VAR_API F : public var {
public:
	static bool type(VAR);
	static F& cast(VAR);
	static DOUBLE d(VAR);
	F();
	F(INT);
	F(UINT);
	F(DOUBLE);
	F(const CHAR*,UINT);

	void print() const;
	INT sgn() const;
	F copy() const;
};
VAR_API F operator + (const F&);
VAR_API F operator - (const F&);
VAR_API F operator + (const F&,const F&);
VAR_API F operator + (const F&,UINT);
VAR_API F operator - (const F&,const F&);
VAR_API F operator - (const F&,UINT);
VAR_API F operator - (UINT,const F&);
VAR_API F operator * (const F&,const F&);
VAR_API F operator * (const F&,UINT);
VAR_API F operator / (const F&,const F&);
VAR_API F operator / (const F&,UINT);
VAR_API F operator / (UINT,const F&);
VAR_API bool operator < (const F&,const F&);
VAR_API bool operator == (const F&,const F&);
VAR_API bool operator > (const F&,const F&);
VAR_API bool operator >= (const F&,const F&);
VAR_API bool operator != (const F&,const F&);
VAR_API bool operator <= (const F&,const F&);
//////////////////////////////////////
namespace Objects {
namespace Real {
//////////////////////////////////////
extern VAR_API UINT REAL;
extern VAR_API BOOL SCIENTIFIC;
VAR_API var Zero();
VAR_API var One();
VAR_API DOUBLE Double(VAR);
VAR_API var Input();
VAR_API var New(VAR);
VAR_API var New(INT);
VAR_API var New(UINT);
VAR_API var New(DOUBLE);
VAR_API var NewZ(VAR);
VAR_API var NewQ(VAR);
VAR_API var New(const CHAR*,UINT);
VAR_API var Print(VAR);
VAR_API var Abs(VAR);
VAR_API var Neg(VAR);
VAR_API var Add(VAR,VAR);
VAR_API var Add(VAR,UINT);
VAR_API var Sub(VAR,VAR);
VAR_API var Sub(VAR,UINT);
VAR_API var Sub(UINT,VAR);
VAR_API var Mul(VAR,VAR);
VAR_API var Mul(VAR,UINT);
VAR_API var Div(VAR,VAR);
VAR_API var Div(VAR,UINT);
VAR_API var Div(UINT,VAR);
VAR_API var Sc(VAR,INT);
VAR_API var Shift(VAR,INT);
VAR_API INT Cmp(VAR,VAR);
VAR_API INT Sgn(VAR);
VAR_API var Pow(VAR,UINT);
VAR_API var Sqrt(VAR);
VAR_API var Sqrt(UINT);
VAR_API UINT Prec(VAR);
VAR_API void Prec(VAR,UINT);
VAR_API UINT DefaultPrec();
VAR_API void DefaultPrec(UINT);
VAR_API var Ceil(VAR);
VAR_API var Floor(VAR);
VAR_API var Trunc(VAR);
VAR_API VOID Init();
//////////////////////////////////////
}
}
}