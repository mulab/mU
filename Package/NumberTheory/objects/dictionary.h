#pragma once
#include "var.h"

namespace maTHmU {
//////////////////////////////////////
class dict;
typedef const dict& DICT;
//////////////////////////////////////
class VAR_API dict : public var {
public:
	static bool type(VAR);
	static dict& cast(VAR);
	dict();

	void print() const;
	dict copy() const;
	LESS_OP pred() const;
	LESS_OP& pred();
	UINT size() const;
	UINT count(VAR) const;
	VAR operator [] (VAR) const;
	var& operator [] (VAR);
};
VAR_API bool operator < (DICT,DICT);
VAR_API bool operator == (DICT,DICT);
VAR_API bool operator > (DICT,DICT);
VAR_API bool operator >= (DICT,DICT);
VAR_API bool operator != (DICT,DICT);
VAR_API bool operator <= (DICT,DICT);
//////////////////////////////////////
namespace Objects {
namespace Dictionary {
//////////////////////////////////////
extern VAR_API UINT DICTIONARY;
VAR_API var Zero();
VAR_API var New(VAR);
VAR_API var Print(VAR);
VAR_API LESS_OP& Pred(VAR);
VAR_API UINT Size(VAR);
VAR_API UINT Count(VAR,VAR);
VAR_API var& At(VAR,VAR);
VAR_API INT Cmp(VAR,VAR);
VAR_API VOID Init();
//////////////////////////////////////
}
}
}