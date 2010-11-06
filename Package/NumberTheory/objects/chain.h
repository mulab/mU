#pragma once
#include "var.h"

namespace maTHmU {
//////////////////////////////////////
class chain;
typedef const chain& CHAIN;
//////////////////////////////////////
class VAR_API chain : public var {
public:
	static bool type(VAR);
	static chain& cast(VAR);
	chain();
	chain(VAR,CHAIN);

	void print() const;
	chain copy() const;
	CHAIN next() const;
	chain& next();
	void insert(VAR);
	var erase();
	operator bool() const;
	VAR operator * () const;
	var& operator * ();
};
VAR_API bool operator < (CHAIN,CHAIN);
VAR_API bool operator == (CHAIN,CHAIN);
VAR_API bool operator > (CHAIN,CHAIN);
VAR_API bool operator >= (CHAIN,CHAIN);
VAR_API bool operator != (CHAIN,CHAIN);
VAR_API bool operator <= (CHAIN,CHAIN);
//////////////////////////////////////
namespace Objects {
namespace Chain {
//////////////////////////////////////
extern VAR_API UINT CHAIN;
VAR_API var Zero();
VAR_API var New(VAR);
VAR_API var New(VAR,VAR);
VAR_API var Print(VAR);
VAR_API var& Next(VAR);
VAR_API var& Data(VAR);
VAR_API VOID Insert(VAR,VAR);
VAR_API var Erase(VAR);
VAR_API INT Cmp(VAR,VAR);
VAR_API BOOL IsNode(VAR);
VAR_API VOID Init();
//////////////////////////////////////
}
}
}
