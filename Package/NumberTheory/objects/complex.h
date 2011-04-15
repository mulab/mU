#pragma once
#include "pair.h"

namespace maTHmU {
//////////////////////////////////////
class VAR_API C : public pair {
public:
	static bool type(VAR);
	static C& cast(VAR);
	C();
	C(VAR,VAR);

	void print() const;
	C copy() const;
};
VAR_API C operator ~ (const C&);
VAR_API C operator - (const C&);
VAR_API C operator + (const C&,const C&);
VAR_API C operator - (const C&,const C&);
VAR_API C operator * (const C&,const C&);
VAR_API C operator / (const C&,const C&);
VAR_API bool operator < (const C&,const C&);
VAR_API bool operator == (const C&,const C&);
VAR_API bool operator > (const C&,const C&);
VAR_API bool operator >= (const C&,const C&);
VAR_API bool operator != (const C&,const C&);
VAR_API bool operator <= (const C&,const C&);
//////////////////////////////////////
namespace Objects {
namespace Complex {
//////////////////////////////////////
extern VAR_API UINT COMPLEX;
VAR_API var Zero();
VAR_API var New(VAR);
VAR_API var New(VAR,VAR);
VAR_API var Print(VAR);
VAR_API var Norm(VAR);
VAR_API var Conjugate(VAR);
VAR_API var Neg(VAR);
VAR_API var Add(VAR,VAR);
VAR_API var Sub(VAR,VAR);
VAR_API var Mul(VAR,VAR);
VAR_API var Div(VAR,VAR);
VAR_API var Sc(VAR,INT);
VAR_API INT Cmp(VAR,VAR);
VAR_API VOID Init();
//////////////////////////////////////
}
}
}