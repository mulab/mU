#pragma once
#include "list.h"

namespace maTHmU {
//////////////////////////////////////
class tree;
typedef const tree& TREE;
//////////////////////////////////////
class VAR_API tree : public var {
public:
	static bool type(VAR);
	static tree& cast(VAR);
	tree();
	tree(VAR,UINT);
	tree(VAR,LIST);
	tree(VAR,VAR,VAR);

	void print() const;
	tree copy() const;
	TREE left() const;
	tree& left();
	TREE right() const;
	tree& right();
	operator bool() const;
	VAR operator * () const;
	var& operator * ();
	const list* operator -> () const;
	list* operator -> ();
	TREE operator [] (INT) const;
	tree& operator [] (INT);
};
VAR_API bool operator < (TREE,TREE);
VAR_API bool operator == (TREE,TREE);
VAR_API bool operator > (TREE,TREE);
VAR_API bool operator >= (TREE,TREE);
VAR_API bool operator != (TREE,TREE);
VAR_API bool operator <= (TREE,TREE);
//////////////////////////////////////
namespace Objects {
namespace Tree {
//////////////////////////////////////
extern VAR_API UINT TREE;
VAR_API var Zero();
VAR_API var New(VAR);
VAR_API var New(VAR,UINT);
VAR_API var New(VAR,LIST);
VAR_API var Print(VAR);
VAR_API var& Left(VAR);
VAR_API var& Right(VAR);
VAR_API var& Data(VAR);
VAR_API list& Childs(VAR);
VAR_API var& Child(VAR,INT);
VAR_API INT Cmp(VAR,VAR);
VAR_API BOOL IsTree(VAR);
VAR_API VOID Init();
//////////////////////////////////////
}
}
}
