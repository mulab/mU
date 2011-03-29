#pragma once
#include "Var.h"

namespace mU {
//////////////////////////////////////
namespace S
{
	API void Set(Var,const wchar*);
	API void Add(Var,Var,Var);
	API void AddTo(Var,Var);
	API int Cmp(Var,Var);
	API uint Length(Var);
	API void Insert(Var,Var,uint);
	API var Substr(Var,uint,uint);
	API void Erase(Var,uint,uint);
};
//////////////////////////////////////
}
