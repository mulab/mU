#pragma once
#include "var.h"

namespace maTHmU {
//////////////////////////////////////
class list;
typedef const list& LIST;
//////////////////////////////////////
class VAR_API list : public var {
public:
	static bool type(VAR);
	static list& cast(VAR);
	static list map(UN_OP,VAR);
	static list map(BIN_OP,VAR,VAR);
	static list map(TER_OP,VAR,VAR,VAR);
	list();
	list(UINT,const var* =0);
	list(VAR,VAR);
	list(VAR,VAR,VAR);
	list(VAR,VAR,VAR,VAR);
	list(VAR,VAR,VAR,VAR,VAR);
	list(VAR,VAR,VAR,VAR,VAR,VAR);

	void print() const;
	list copy() const;
	UINT size() const;
	void resize(UINT);
	void insert(VAR,INT);
	var erase(INT);
	list& push(VAR);
	var pop();
	void fill(VAR);
	list mid(INT,INT) const;
	VAR at(INT) const;
	var& at(INT);
	VAR left() const;
	var& left();
	VAR right() const;
	var& right();
	const var* begin() const;
	const var* end() const;
	var* begin();
	var* end();
	void sort(LESS_OP=0) const;
	list map(UN_OP) const;
	operator bool() const;
	VAR operator [] (UINT) const;
	var& operator [] (UINT);
	list& operator += (LIST);
};
VAR_API list operator & (LIST,LIST);
VAR_API list operator | (LIST,LIST);
VAR_API list operator + (LIST,LIST);
VAR_API list operator - (LIST,LIST);
VAR_API bool operator < (LIST,LIST);
VAR_API bool operator == (LIST,LIST);
VAR_API bool operator > (LIST,LIST);
VAR_API bool operator >= (LIST,LIST);
VAR_API bool operator != (LIST,LIST);
VAR_API bool operator <= (LIST,LIST);
//////////////////////////////////////
namespace Objects {
namespace List {
//////////////////////////////////////
extern VAR_API UINT LIST;
VAR_API var Zero();
VAR_API var New(VAR);
VAR_API var New(UINT,const var* =0);
VAR_API var New(VAR,VAR);
VAR_API var New(VAR,VAR,VAR);
VAR_API var New(VAR,VAR,VAR,VAR);
VAR_API var New(VAR,VAR,VAR,VAR,VAR);
VAR_API var New(VAR,VAR,VAR,VAR,VAR,VAR);
VAR_API var Print(VAR);
VAR_API var Mid(VAR,INT,INT);
VAR_API VOID Append(var&,VAR);
VAR_API var Join(VAR,VAR);
VAR_API var Complement(VAR,VAR);
VAR_API var Intersection(VAR,VAR);
VAR_API var Union(VAR,VAR);
VAR_API VOID Sort(VAR,LESS_OP=0);
VAR_API var Map(UN_OP,VAR);
VAR_API var Map(BIN_OP,VAR,VAR);
VAR_API var Map(TER_OP,VAR,VAR,VAR);
VAR_API UINT Size(VAR);
VAR_API VOID Resize(VAR,UINT);
VAR_API var& Left(VAR);
VAR_API var& Right(VAR);
VAR_API var* Begin(VAR);
VAR_API var* End(VAR);
VAR_API var& At(VAR,INT);
VAR_API VOID Insert(VAR,VAR,INT);
VAR_API var Erase(VAR,INT);
VAR_API var& Push(VAR,VAR);
VAR_API var Pop(VAR);
VAR_API VOID Fill(VAR,VAR);
VAR_API INT Cmp(VAR,VAR);
VAR_API VOID Init();
//////////////////////////////////////
}
}
}
