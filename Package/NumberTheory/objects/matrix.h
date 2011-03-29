#pragma once
#include "var.h"

namespace maTHmU {
//////////////////////////////////////
class mat;
typedef const mat& MAT;
//////////////////////////////////////
class VAR_API mat : public var {
public:
	static bool type(VAR);
	static mat& cast(VAR);
	mat();
	mat(UINT,UINT,const var* =0);
	mat(UINT,UINT,var,...);

	void print() const;
	UINT rows() const;
	UINT cols() const;
	mat copy() const;
	operator bool() const;
	VAR operator () (UINT,UINT) const;
	var& operator () (UINT,UINT);
};
VAR_API mat operator ~ (MAT);
VAR_API mat operator + (MAT);
VAR_API mat operator - (MAT);
VAR_API mat operator + (MAT,MAT);
VAR_API mat operator - (MAT,MAT);
VAR_API mat operator * (MAT,MAT);
VAR_API bool operator < (MAT,MAT);
VAR_API bool operator == (MAT,MAT);
VAR_API bool operator > (MAT,MAT);
VAR_API bool operator >= (MAT,MAT);
VAR_API bool operator != (MAT,MAT);
VAR_API bool operator <= (MAT,MAT);
//////////////////////////////////////
namespace Objects {
namespace Matrix {
//////////////////////////////////////
extern VAR_API UINT MATRIX;
VAR_API var Zero();
VAR_API var New(VAR);
VAR_API var New(UINT,UINT,const var* =0);
VAR_API var New(UINT,UINT,var,...);
VAR_API var Print(VAR);
VAR_API var Transpose(VAR);
VAR_API var Norm(VAR);
VAR_API var Abs(VAR);
VAR_API var Neg(VAR);
VAR_API var Add(VAR,VAR);
VAR_API var Sub(VAR,VAR);
VAR_API var Mul(VAR,VAR);
VAR_API var Pow(VAR,UINT);
VAR_API var Sc(VAR,INT);
VAR_API INT Cmp(VAR,VAR);
VAR_API UINT Rows(VAR);
VAR_API UINT Cols(VAR);
VAR_API var& Coe(VAR,UINT,UINT);
VAR_API VOID Init();
//////////////////////////////////////
}
}
}