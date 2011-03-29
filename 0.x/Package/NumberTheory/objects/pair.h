#pragma once
#include "var.h"

namespace maTHmU {
//////////////////////////////////////
class pair;
typedef const pair& PAIR;
//////////////////////////////////////
class VAR_API pair : public var {
public:
	static bool type(VAR);
	static pair& cast(VAR);
	pair();
	pair(VAR,VAR);

	void print() const;
	pair copy() const;
	VAR a() const;
	var& a();
	VAR b() const;
	var& b();
	operator bool() const;
};
VAR_API bool operator < (PAIR,PAIR);
VAR_API bool operator == (PAIR,PAIR);
VAR_API bool operator > (PAIR,PAIR);
VAR_API bool operator >= (PAIR,PAIR);
VAR_API bool operator != (PAIR,PAIR);
VAR_API bool operator <= (PAIR,PAIR);
//////////////////////////////////////
namespace Objects {
namespace Pair {
//////////////////////////////////////
extern VAR_API UINT PAIR;
VAR_API var Zero();
VAR_API var New(VAR);
VAR_API var New(VAR,VAR);
VAR_API var Print(VAR);
VAR_API var& A(VAR);
VAR_API var& B(VAR);
VAR_API INT Cmp(VAR,VAR);
VAR_API VOID Init();
//////////////////////////////////////
}
}
}