#pragma once
#include "string.h"

namespace maTHmU {
//////////////////////////////////////
class sym;
typedef const sym& SYM;
//////////////////////////////////////
class VAR_API sym : public str {
public:
	static bool type(VAR);
	static sym& cast(VAR);
	sym(const CHAR*);
	
	void print() const;
	operator UINT() const;
};
//////////////////////////////////////
namespace Objects {
namespace Symbol {
//////////////////////////////////////
extern VAR_API UINT SYMBOL;
VAR_API var New(const CHAR*);
VAR_API var Print(VAR);
VAR_API UINT Id(VAR);
VAR_API const CHAR* Name(UINT);
VAR_API INT Cmp(VAR,VAR);
VAR_API VOID Init();
//////////////////////////////////////
}
}
}