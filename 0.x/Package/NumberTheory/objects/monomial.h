#pragma once
#include "var.h"

namespace maTHmU {
//////////////////////////////////////
class mono;
typedef const mono& MONO;
//////////////////////////////////////
class VAR_API mono : public var {
public:
	static bool type(VAR);
	static mono& cast(VAR);
	mono();
	mono(UINT,const UINT*,const INT*);
	mono(UINT,...);

	void print() const;
	mono copy() const;
	UINT size() const;
	INT operator [] (UINT) const;
	INT& operator [] (UINT);
};
VAR_API mono operator & (MONO,MONO);
VAR_API mono operator | (MONO,MONO);
VAR_API mono operator * (MONO,MONO);
VAR_API mono operator / (MONO,MONO);
VAR_API bool operator < (MONO,MONO);
VAR_API bool operator == (MONO,MONO);
VAR_API bool operator > (MONO,MONO);
VAR_API bool operator >= (MONO,MONO);
VAR_API bool operator != (MONO,MONO);
VAR_API bool operator <= (MONO,MONO);
//////////////////////////////////////
namespace Objects {
namespace Monomial {
//////////////////////////////////////
extern VAR_API UINT MONOMIAL;
VAR_API var Zero();
VAR_API var New(VAR);
VAR_API var New(UINT,const UINT*,const INT*);
VAR_API var New(UINT,...);
VAR_API var Print(VAR);
VAR_API var Mul(VAR,VAR);
VAR_API var Div(VAR,VAR);
VAR_API var Gcd(VAR,VAR);
VAR_API var Lcm(VAR,VAR);
VAR_API UINT Size(VAR);
VAR_API INT& Exp(VAR,UINT);
VAR_API INT Cmp(VAR,VAR);
VAR_API VOID Init();
//////////////////////////////////////
}
}
}