#pragma once
#include "list.h"

namespace maTHmU {
//////////////////////////////////////
class poly;
typedef const poly& POLY;
//////////////////////////////////////
class VAR_API poly : public list {
public:
	static bool type(VAR);
	static poly& cast(VAR);
	poly();
	poly(LIST);

	void print() const;
	poly copy() const;
	INT deg() const;
	var lc() const;
	poly dform() const;
	var operator () (VAR) const;
};
VAR_API poly operator + (POLY);
VAR_API poly operator - (POLY);
VAR_API poly operator + (POLY,POLY);
VAR_API poly operator - (POLY,POLY);
VAR_API poly operator * (POLY,POLY);
VAR_API poly operator / (POLY,POLY);
VAR_API poly operator % (POLY,POLY);
VAR_API bool operator < (POLY,POLY);
VAR_API bool operator == (POLY,POLY);
VAR_API bool operator > (POLY,POLY);
VAR_API bool operator >= (POLY,POLY);
VAR_API bool operator != (POLY,POLY);
VAR_API bool operator <= (POLY,POLY);
//////////////////////////////////////
namespace Objects {
namespace Polynomial {
//////////////////////////////////////
extern VAR_API UINT POLYNOMIAL;
VAR_API var Zero();
VAR_API var Super(VAR);
VAR_API var New(VAR);
VAR_API var Print(VAR);
VAR_API var Abs(VAR);
VAR_API var Neg(VAR);
VAR_API var Add(VAR,VAR);
VAR_API var Sub(VAR,VAR);
VAR_API var Mul(VAR,VAR);
VAR_API var Div(VAR,VAR);
VAR_API var Mod(VAR,VAR);
VAR_API var Pow(VAR,UINT);
VAR_API var Sc(VAR,INT);
VAR_API VOID DivMod(VAR,VAR,var&,var&);
VAR_API INT Cmp(VAR,VAR);
VAR_API INT Deg(VAR);
VAR_API var Lc(VAR);
VAR_API var Eval(VAR,VAR);
VAR_API var Dform(VAR);
VAR_API VOID Init();
//////////////////////////////////////
}
}
}