#include <mU/String.h>

namespace mU {
//////////////////////////////////////
namespace S
{
void Set(Var x, const wchar *y)
{
	STR_REP(x).assign(y);
}
void Add(Var x, Var y, Var z)
{
	STR_REP(x) = STR_REP(y) + STR_REP(z);
}
void AddTo(Var x, Var y)
{
	STR_REP(x) += STR_REP(y);
}
int Cmp(Var x, Var y)
{
	return STR_REP(x).compare(STR_REP(y));
}
uint Length(Var x)
{
	return STR_REP(x).length();
}
void Insert(Var x, Var y, uint n)
{
	STR_REP(x).insert(n,STR_REP(y));
}
var Substr(Var x, uint y, uint n)
{
	return Str(STR_REP(x).substr(y,n));
}
void Erase(Var x, uint y, uint n)
{
	STR_REP(x).erase(y,n);
}
}
//////////////////////////////////////
}
