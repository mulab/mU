#pragma once
#include "Var.h"

namespace mU {
//////////////////////////////////////
#define LOG_2_10 3.3219280948873623478703194294894
#define LOG_10_2 0.3010299956639811952137388947245
namespace Z
{
	API void Set(Var,uint);
	API void Set(Var,sint);
	API void Set(Var,double);
	API void Set(Var,const char*,int);
	API void Set(Var,Var);
	API void SetQ(Var,Var);
	API void SetF(Var,Var);
	API void Abs(Var,Var);
	API void Neg(Var,Var);
	API void Com(Var,Var);
	API void And(Var,Var,Var);
	API void Ior(Var,Var,Var);
	API void Xor(Var,Var,Var);
	API void Add(Var,Var,Var);
	API void Add(Var,Var,uint);
	API void Sub(Var,Var,Var);
	API void Sub(Var,Var,uint);
	API void Sub(Var,uint,Var);
	API void Mul(Var,Var,Var);
	API void Mul(Var,Var,uint);
	API void Mul(Var,Var,int);
	API void Div(Var,Var,Var);
	API void Div(Var,Var,uint);
	API void Mod(Var,Var,Var);
	API uint Mod(Var,uint);
	API void Pow(Var,Var,uint);
	API void Pow(Var,uint,uint);
	API void Inv(Var,Var,Var);
	API int Cmp(Var,Var);
	API int Cmp(Var,uint);
	API int Cmp(Var,int);
	API int Cmp(Var,double);
	API int Sgn(Var);
	API uint UI(Var);
	API sint SI(Var);
	API double D(Var);
};
namespace Q
{
	API void Set(Var,uint,uint);
	API void Set(Var,sint,uint);
	API void Set(Var,double);
	API void Set(Var,const char*,int);
	API void Set(Var,Var);
	API void SetZ(Var,Var);
	API void SetZ(Var,Var,Var);
	API void SetNum(Var,Var);
	API void SetDen(Var,Var);
	API void SetF(Var,Var);
	API void Num(Var,Var);
	API void Den(Var,Var);
	API void Canonicalize(Var);
	API void Abs(Var,Var);
	API void Neg(Var,Var);
	API void Add(Var,Var,Var);
	API void Sub(Var,Var,Var);
	API void Mul(Var,Var,Var);
	API void Div(Var,Var,Var);
	API int Cmp(Var,Var);
	API int Cmp(Var,uint,uint);
	API int Cmp(Var,int,uint);
	API int Sgn(Var);
	API double D(Var);
};
namespace F
{
	API void Set(Var,uint);
	API void Set(Var,sint);
	API void Set(Var,double);
	API void Set(Var,const char*,int);
	API void Set(Var,Var);
	API void SetZ(Var,Var);
	API void SetQ(Var,Var);
	API void Abs(Var,Var);
	API void Neg(Var,Var);
	API void Add(Var,Var,Var);
	API void Sub(Var,Var,Var);
	API void Mul(Var,Var,Var);
	API void Div(Var,Var,Var);
	API void Pow(Var,Var,uint);
	API int Cmp(Var,Var);
	API int Cmp(Var,uint);
	API int Cmp(Var,int);
	API int Cmp(Var,double);
	API int Sgn(Var);
	API uint UI(Var);
	API sint SI(Var);
	API double D(Var);
};
namespace N
{
	API var Abs(Var);
	API var Neg(Var);
	API var AbsTo(Var);
	API var NegTo(Var);
	API var Add(Var,Var);
	API var Sub(Var,Var);
	API var Mul(Var,Var);
	API var Div(Var,Var);
	API var AddTo(Var,Var);
	API var SubTo(Var,Var);
	API var MulTo(Var,Var);
	API var DivTo(Var,Var);
	API int Cmp(Var,Var);
	API int Sgn(Var);
	API double D(Var);
	API var Canonicalize(Var);
}
//////////////////////////////////////
}
