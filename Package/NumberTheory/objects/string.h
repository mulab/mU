#pragma once
#include "var.h"

namespace maTHmU {
//////////////////////////////////////
class str;
typedef const str& STR;
//////////////////////////////////////
class VAR_API str : public var {
public:
	static bool type(VAR);
	static str& cast(VAR);
	str();
	str(const CHAR*);

	void print() const;
	str copy() const;
	str substr(UINT,UINT) const;
	UINT length() const;
	const CHAR* data() const;
	operator const CHAR* () const;
	str& operator += (STR);
};
VAR_API str operator + (STR,STR);
VAR_API bool operator < (STR,STR);
VAR_API bool operator == (STR,STR);
VAR_API bool operator > (STR,STR);
VAR_API bool operator >= (STR,STR);
VAR_API bool operator != (STR,STR);
VAR_API bool operator <= (STR,STR);
//////////////////////////////////////
namespace Objects {
namespace String {
//////////////////////////////////////
extern VAR_API UINT STRING;
VAR_API var Zero();
VAR_API var New(VAR);
VAR_API var New(const CHAR*);
VAR_API var Print(VAR);
VAR_API VOID Append(var&,VAR);
VAR_API var Join(VAR,VAR);
VAR_API var Substr(VAR,UINT,UINT);
VAR_API UINT Length(VAR);
VAR_API const CHAR* Data(VAR);
VAR_API const CHAR* Cstr(VAR);
VAR_API INT Cmp(VAR,VAR);
VAR_API VOID Init();
//////////////////////////////////////
}
}
}