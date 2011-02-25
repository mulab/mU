#include <mU/Function.h>

namespace mU {
namespace Matrix {
inline var &Entry(Var x, size_t i, size_t j) { return At(At(x,i),j); }
inline bool next_comb(size_t *c,size_t n, size_t r)
{
	int i = r;
	while(i-- && c[i] >= n - r + i);
	if(i < 0) return false;
	++c[i];
	for(int j = i + 1; j < r; ++j) c[j] = c[j-1] + 1;
	return true;
}
//MatrixConstruct.cpp
var qIdentityMatrix(size_t);
var iIdentityMatrix(size_t);
var Take(Var,size_t,size_t,size_t,size_t);
var qCopy(Var);
var qPadRight(Var M,size_t m,size_t n);
var Drop(Var,size_t,size_t);
var getMatrixFromVector(Var);
var getVectorFromMatrix(Var);
//BLAS.cpp
bool MatrixQ(Var);//判断表达式是否为矩阵（包括各种数据类型的矩阵）
var Transpose(Var);
var qVAdd(Var,Var);
var qVSub(Var,Var);
var qVMul(Var,Var);
var qVDiv(Var,Var);
var qAdd(Var,Var);
var qSub(Var,Var);
var qVDot(Var,Var);
var qDot(Var,Var);
var qTr(Var);
var qKroneckerProduct(Var,Var);

var Add(Var,Var);
var Sub(Var,Var);
var Scalar(Var,Var);
var VDot(Var,Var);
var Dot(Var,Var);
var Tr(Var);
var KroneckerProduct(Var,Var);
//gauss.cpp
var qLU(Var);
var LinearSolve(Var,Var);//右端矩阵，若输入为右端向量，则应转为列矩阵
var qRowReduce(Var);
var qNullSpace(Var);//若输入为整数矩阵，则计算完之后应通过适当的倍乘将向量转为整数向量。
var qDet(Var);
var qMinors(Var);//方阵
var qMinors(Var,size_t);//方阵
var qMinor(Var,size_t*,size_t*,size_t);
var qInverse(Var);
size_t qMatrixRank(Var);
var LeastSquares(Var,Var);
//ModulusGauss.cpp
inline void AddMod(Var r,Var m,Var n,Var p)
{
	var a=Int(),b=Int();
	Z::Mod(a,m,p);
	Z::Mod(b,n,p);
	Z::Add(r,a,b);
	Z::Mod(r,r,p);
}
inline void SubMod(Var r,Var m,Var n,Var p)
{
	var a=Int(),b=Int();
	Z::Mod(a,m,p);
	Z::Mod(b,n,p);
	Z::Sub(r,a,b);
	Z::Mod(r,r,p);
}
inline void MulMod(Var r,Var m,Var n,Var p)
{
	var a=Int(),b=Int();
	Z::Mod(a,m,p);
	Z::Mod(b,n,p);
	Z::Mul(r,a,b);
	Z::Mod(r,r,p);
}
inline void DivMod(Var r,Var m,Var n,Var p)
{
	var a=Int(),b=Int();
	Z::Mod(a,m,p);
	Z::Mod(b,n,p);
	Z::Inv(r,b,p);
	MulMod(r,r,a,p);
}
var mMod(Var M,Var p);
void mMod(Var r,Var M,Var p);
var mLU(Var M,Var p);
var mLinearSolve(Var,Var,Var);
var mRowReduce(Var M,Var prime);
var mNullSpace(Var M,Var prime);
var mInverse(Var M,Var prime);
var mDet(Var M,Var prime);
size_t mMatrixRank(Var M,Var prime);
//ModGaussSmallP.cpp
inline uint AddMod(uint m,uint n,uint p){return (m+n)%p;}
inline uint SubMod(uint m,uint n,uint p){return (m-n)%p;}
inline uint MulMod(uint m,uint n,uint p){return (m*n)%p;}
inline uint DivMod(uint m,uint n,uint p)
{
	var mv=Int(m),nv=Int(n),pv=Int(p),r=Int();
	DivMod(r,mv,nv,pv);
	return Z::Mod(r,p);
}
var mMod(Var M,uint p);
void mMod(Var R,Var M,uint p);
var mLU(Var M,uint p);
var mLinearSolve(Var,Var,uint p);
var mRowReduce(Var M,uint prime);
var mNullSpace(Var M,uint prime);
var mInverse(Var M,uint prime);
var mDet(Var M,uint prime);
size_t mMatrixRank(Var M,uint prime);
//Strassen.cpp
var StrassenMM(Var M,Var N);
}
}