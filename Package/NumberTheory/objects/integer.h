#pragma once
#include "var.h"

namespace maTHmU {
//////////////////////////////////////
class VAR_API Z : public var {
public:
	static bool type(VAR);
	static Z& cast(VAR);
	static INT i(VAR);
	Z();
	Z(INT);
	Z(UINT);
	Z(DOUBLE);
	Z(const CHAR*,UINT);

	void print() const;
	INT sgn() const;
	Z copy() const;
};
VAR_API Z operator ~ (const Z&);
VAR_API Z operator + (const Z&);
VAR_API Z operator - (const Z&);
VAR_API Z operator & (const Z&,const Z&);
VAR_API Z operator | (const Z&,const Z&);
VAR_API Z operator ^ (const Z&,const Z&);
VAR_API Z operator + (const Z&,const Z&);
VAR_API Z operator + (const Z&,UINT);
VAR_API Z operator - (const Z&,const Z&);
VAR_API Z operator - (const Z&,UINT);
VAR_API Z operator * (const Z&,const Z&);
VAR_API Z operator * (const Z&,UINT);
VAR_API Z operator / (const Z&,const Z&);
VAR_API Z operator / (const Z&,UINT);
VAR_API Z operator % (const Z&,const Z&);
VAR_API UINT operator % (const Z&,UINT);
VAR_API bool operator < (const Z&,const Z&);
VAR_API bool operator == (const Z&,const Z&);
VAR_API bool operator > (const Z&,const Z&);
VAR_API bool operator >= (const Z&,const Z&);
VAR_API bool operator != (const Z&,const Z&);
VAR_API bool operator <= (const Z&,const Z&);
//////////////////////////////////////
namespace Objects {
namespace Integer {
//////////////////////////////////////
extern VAR_API UINT INTEGER;
VAR_API var Zero();
VAR_API var One();
VAR_API INT Int(VAR);
VAR_API var New(VAR);
VAR_API var New(INT);
VAR_API var New(UINT);
VAR_API var New(DOUBLE);
VAR_API var NewQ(VAR);
VAR_API var NewF(VAR);
VAR_API var New(const CHAR*,UINT);
VAR_API var Print(VAR);
VAR_API var Abs(VAR);
VAR_API var Com(VAR);
VAR_API var Neg(VAR);
VAR_API var And(VAR,VAR);
VAR_API var Ior(VAR,VAR);
VAR_API var Xor(VAR,VAR);
VAR_API var Add(VAR,VAR);
VAR_API var Add(VAR,UINT);
VAR_API var Sub(VAR,VAR);
VAR_API var Sub(VAR,UINT);
VAR_API var Sub(UINT,VAR);
VAR_API var Mul(VAR,VAR);
VAR_API var Mul(VAR,UINT);
VAR_API var Div(VAR,VAR);
VAR_API var Div(VAR,UINT);
VAR_API var DivExact(VAR,VAR);
VAR_API var DivExact(VAR,UINT);
VAR_API VOID DivMod(VAR,VAR,var&,var&);
VAR_API UINT DivMod(VAR,UINT,var&);
VAR_API var Mod(VAR,VAR);
VAR_API UINT Mod(VAR,UINT);
VAR_API var Sc(VAR,INT);
VAR_API var Gcd(VAR,VAR);
VAR_API UINT Gcd(VAR,UINT);
VAR_API var Lcm(VAR,VAR);
VAR_API var Lcm(VAR,UINT);
VAR_API VOID GcdExt(VAR,VAR,var&,var&,var&);
VAR_API var Pow(VAR,UINT);
VAR_API var Pow(UINT,UINT);
VAR_API var Powm(VAR,VAR);
VAR_API var Powm(VAR,UINT);
VAR_API var Root(VAR,UINT);
VAR_API VOID RootRem(VAR,UINT,var&,var&);
VAR_API var Sqrt(VAR);
VAR_API VOID SqrtRem(VAR,var&,var&);
VAR_API var Inv(VAR,VAR);
VAR_API var Shift(VAR,INT);
VAR_API INT Cmp(VAR,VAR);
VAR_API INT Sgn(VAR);
VAR_API INT Divisible(VAR,VAR);
VAR_API INT Divisible(VAR,UINT);
VAR_API INT Congruent(VAR,VAR,VAR);
VAR_API INT Congruent(VAR,UINT,UINT);
VAR_API VOID Init();
//////////////////////////////////////
}
}
}