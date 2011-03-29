#include <mU/String.h>

namespace mU {
//////////////////////////////////////
namespace S
{
#define T(x) (((str_t*)(x))->rep)
void Set(Var x, const wchar *y)
{
	T(x).assign(y);
}
void Add(Var x, Var y, Var z)
{
		T(x) = T(y) + T(z);
}
void AddTo(Var x, Var y)
{
	T(x) += T(y);
}
int Cmp(Var x, Var y)
{
	return T(x).compare(T(y));
}
uint Length(Var x)
{
	return T(x).length();
}
void Insert(Var x, Var y, uint n)
{
	T(x).insert(n,T(y));
}
var Substr(Var x, uint y, uint n)
{
	return Str(T(x).substr(y,n));
}
void Erase(Var x, uint y, uint n)
{
	T(x).erase(y,n);
}
#undef T
}
//////////////////////////////////////
}
