#include "Matrix.h"

namespace mU {
namespace Matrix {
var Drop(Var M,size_t i,size_t j)//去掉M的第i行和第j列（i，j均从0开始）
{
	size_t m=Size(M);
	size_t n=Size(At(M,0));
	var r=Vec(m-1);
	for(size_t k=0;k<m-1;++k)
	{
		At(r,k)=Vec(n-1);
		for(size_t l=0;l<n-1;++l)
		{
			if(k<i)
				Entry(r,k,l)=(l<j? Entry(M,k,l):Entry(M,k,l+1));
			else
				Entry(r,k,l)=(l<j? Entry(M,k+1,l):Entry(M,k+1,l+1));
		}
	}
	return r;
}
var qIdentityMatrix(size_t n)
{
	var M=Vec(n);
	var one=Rat(1.0),zero=Rat(0.0);
	for(size_t i=0;i<n;++i)
	{
		At(M,i)=Vec(n);
		for(size_t j=0;j<n;++j)
		{
			if(i==j)
				Entry(M,i,j)=one;
			else
				Entry(M,i,j)=zero;
		}
	}
	return M;
}
var iIdentityMatrix(size_t n)
{
	var M=Vec(n);
	var one=Int(1L),zero=Int(0L);
	for(size_t i=0;i<n;++i)
	{
		At(M,i)=Vec(n);
		for(size_t j=0;j<n;++j)
		{
			if(i==j)
				Entry(M,i,j)=one;
			else
				Entry(M,i,j)=zero;
		}
	}
	return M;
}
//Take sub-matrix, rows r1 to r2, columns c1 to c2. The first row number is 1.
var Take(Var M, size_t r1, size_t r2,size_t c1,size_t c2)
{
	size_t r=r2-r1+1, c=c2-c1+1;
	var a=Vec(r);
	for(size_t i=0;i<r;++i)
	{
		At(a,i)=Vec(c);	
		for(size_t j=0;j<c;++j)
			Entry(a,i,j)=Entry(M,r1+i-1,c1+j-1);
	}
	return a;
}
var Take(Var M, size_t c1[], size_t r1,size_t c2[],size_t r2)
{
	var M1=Vec(r1);
	for(size_t i=0;i<r1;++i)
	{
		At(M1,i)=Vec(r2);
		for(size_t j=0;j<r2;++j)
			Entry(M1,i,j)=Entry(M,c1[i],c2[j]);
	}
	return M1;
}
var	qCopy(Var M)
{
	size_t m=Size(M);
	size_t n=Size(At(M,0));
	var r=Vec(m);
	for(size_t i=0;i<m;++i)
	{
		At(r,i)=Vec(n);
		for(size_t j=0;j<n;++j)
			Entry(r,i,j)=Rat(Entry(M,i,j));
	}
	return r;
}
var qPadRight(Var M,size_t m,size_t n)
{
	var zero=Rat(0.0);
	var R=Vec(m);
	size_t mm=Size(M);
	size_t nn=Size(At(M,0));
	for(size_t i=0;i<m;++i)
	{
		At(R,i)=Vec(n);
		for(size_t j=0;j<n;++j)
		{
			if(i<mm&j<nn)
				Entry(R,i,j)=Entry(M,i,j);
			else
				Entry(R,i,j)=zero;
		}
	}
	return R;
}
var getMatrixFromVector(Var vect)
{
	var r=Vec(1);
	At(r,0)=Vec(Size(vect));
	for (size_t i=0;i<Size(vect);i++)
	{			
		Entry(r,0,i)=At(vect,i);
	}
	return r;
}
var getVectorFromMatrix(Var mat)
{
	var r=Vec(Size(At(mat,0)));
	for (size_t i=0;i<Size(At(mat,0));i++)
	{
		At(r,i)=Entry(mat,0,i);
	}
	return r;
}
}
}