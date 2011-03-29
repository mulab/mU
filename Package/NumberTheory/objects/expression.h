#pragma once
#include "list.h"

namespace maTHmU {
//////////////////////////////////////
class expr;
typedef const expr& EXPR;
//////////////////////////////////////
class VAR_API expr : public list {
public:
	static bool type(VAR);
	static expr& cast(VAR);
	static var& head(VAR);
	static list body(VAR);
	expr();
	expr(LIST);
	expr(VAR,LIST);

	void print() const;
	expr copy() const;
	void sort(LESS_OP=0) const;
};
VAR_API bool operator < (EXPR,EXPR);
VAR_API bool operator == (EXPR,EXPR);
VAR_API bool operator > (EXPR,EXPR);
VAR_API bool operator >= (EXPR,EXPR);
VAR_API bool operator != (EXPR,EXPR);
VAR_API bool operator <= (EXPR,EXPR);
//////////////////////////////////////
namespace Objects {
namespace Expression {
//////////////////////////////////////
extern VAR_API UINT EXPRESSION;
VAR_API var Zero();
VAR_API var Super(VAR);
VAR_API var New(VAR);
VAR_API var New(VAR,LIST);
VAR_API var Print(VAR);
VAR_API INT Cmp(VAR,VAR);
VAR_API VOID Sort(VAR,LESS_OP=0);
VAR_API var& Head(VAR);
VAR_API list Body(VAR);
VAR_API VOID Init();
//////////////////////////////////////
}
}
}