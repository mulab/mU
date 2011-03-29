#include "Matrix.h"

namespace mU {
namespace Matrix {
bool MatrixQ(Var m)
{
	if (!VecQ(m))
		return false;
	if (!VecQ(At(m,0)))
		return false;
	size_t rowN=Size(m);
	size_t colN=Size(At(m,0));
	for (size_t i=0;i<rowN;i++)
	{
		if (Size(At(m,i))!=colN)
			return false;
		for (size_t j=0;j<colN;j++)
		{
			if (
				!IntQ(Entry(m,i,j)) &&
				!RatQ(Entry(m,i,j)) &&
				!FltQ(Entry(m,i,j))
				)
				return false;
		}
	}
	return true;
}
var Transpose(Var x)
{
	size_t m=Size(x);
	size_t n=Size(At(x,0));
	var r=Vec(n);
	for(size_t i=0;i<n;++i)
	{
		var &c =At(r,i);
		c=Vec(m);
		for(size_t j=0;j<m;++j)
		{
			At(c,j)=Entry(x,j,i);
		}
	}
	
	return r;
}
//scalar product of vector v
var qVMul(Var v,Var x)
{
	size_t n=Size(v);
	var r=Vec(n);
	for(size_t i=0;i<n;++i)
	{
		At(r,i)=Rat();
		Q::Mul(At(r,i),x,At(v,i));
	}
	return r;
}
//division of vector v over x
var qVDiv(Var v,Var x)
{
	size_t n=Size(v);
	var r=Vec(n);
	for(size_t i=0;i<n;++i)
	{
		At(r,i)=Rat();
		Q::Div(At(r,i),At(v,i),x);
	}
	return r;
}
//sum of number x and vector v
var qVAdd(Var v,Var x)
{
	size_t n=Size(v);
	var r=Vec(n);
	for(size_t i=0;i<n;++i)
	{
		At(r,i)=Rat();
		Q::Add(At(r,i),x,At(v,i));
	}
	return r;
}
//subtraction
var qVSub(Var v,Var x)
{
	size_t n=Size(v);
	var r=Vec(n);
	for(size_t i=0;i<n;++i)
	{
		At(r,i)=Rat();
		Q::Sub(At(r,i),At(v,i),x);
	}
	return r;
}
//sum of matrices
var qAdd(Var x, Var y)
{
	size_t m = Size(x);
	size_t n = Size(At(x,0));
	var r = Vec(m);
	for(size_t i = 0; i < m; ++i)
	{
		var &a = At(x,i);
		var &b = At(y,i);
		var &c = At(r,i);
		c = Vec(n);
		for(size_t j = 0; j < n; ++j)
		{
			At(c,j) = Rat();
			Q::Add(At(c,j),At(a,j),At(b,j));
		}
	}
	return r;
}
var qSub(Var x, Var y)
{
	size_t m = Size(x);
	size_t n = Size(At(x,0));
	var r = Vec(m);
	for(size_t i = 0; i < m; ++i)
	{
		var &a = At(x,i);
		var &b = At(y,i);
		var &c = At(r,i);
		c = Vec(n);
		for(size_t j = 0; j < n; ++j)
		{
			At(c,j) = Rat();
			Q::Sub(At(c,j),At(a,j),At(b,j));
		}
	}
	return r;
}
var qScalar(Var x, Var y)
{
	size_t m = Size(y);
	size_t n = Size(At(y,0));
	var r = Vec(m);
	for(size_t i = 0; i < m; ++i)
	{
		var &c = At(r,i);
		c = Vec(n);
		for(size_t j = 0; j < n; ++j)
		{
			At(c,j)=Rat();
			Q::Mul(At(c,j),x,Entry(y,i,j));
		}
	}
	return r;
}
var qVDot(Var x, Var y)
{
	size_t n = Size(x);
	var r = Rat(0.0);
	for(size_t i = 0; i < n; ++i)
	{
		var t = Rat();
		Q::Mul(t,At(x,i),At(y,i));
		Q::Add(r,r,t);
	}
	return r;
}
var qDot(Var x,Var y)
{
	size_t m=Size(x);
	size_t p=Size(y);
	size_t n=Size(At(y,0));
	var r=Vec(m);
	for(size_t i=0;i<m;++i)
	{
		var &c = At(r,i);
		c=Vec(n);
		for(size_t j=0;j<n;++j)
		{
			var r = Rat(0.0);
			for(size_t k=0;k<p;++k)
			{
				var t = Rat();
				Q::Mul(t,Entry(x,i,k),Entry(y,k,j));
				Q::Add(r,r,t);
			}
			At(c,j)=r;
		}
	}
	return r;
}
var qTr(Var x)
{
	size_t m = Size(x);
	var r = Rat(0.0);
	for(size_t i = 0; i < m; ++i)
	{
		Q::Add(r,r,Entry(x,i,i));
	}
	return r;
}

var qKroneckerProduct(Var x,Var y)
{
	size_t m=Size(x);
	size_t n=Size(At(x,0));
	size_t p=Size(y);
	size_t q=Size(At(y,0));
	var r=Vec(m*p);
	for(size_t i=0;i<m*p;++i)
	{
		At(r,i)=Vec(n*q);
	}
	for(size_t i=0;i<m;++i)
	{
		for(size_t j=0;j<n;++j)
		{
			for(size_t t=0;t<p;++t)
			{
				for(size_t s=0;s<q;++s)
				{
					var c=Rat();
					Q::Mul(c,Entry(x,i,j),Entry(y,t,s));
					Entry(r,i*p+t,j*q+s)=c;
				}
			}
		}
	}
	return r;
}
var Add(Var x, Var y)
{
	size_t m = Size(x);
	size_t n = Size(At(x,0));
	var r = Vec(m);
	for(size_t i = 0; i < m; ++i)
	{
		var &a = At(x,i);
		var &b = At(y,i);
		var &c = At(r,i);
		c = Vec(n);
		for(size_t j = 0; j < n; ++j)
		{
			At(c,j)=Plus(At(a,j),At(b,j));
		}
	}
	return r;
}
var Sub(Var x,Var y)
{
	size_t m = Size(x);
	size_t n = Size(At(x,0));
	var r = Vec(m);
	for(size_t i = 0; i < m; ++i)
	{
		var &a = At(x,i);
		var &b = At(y,i);
		var &c = At(r,i);
		c = Vec(n);
		for(size_t j = 0; j < n; ++j)
		{
			var &bb=At(b,j);
			var minusOne=Rat(-1.0);
			var minusBb=Times(minusOne,bb);
			At(c,j)=Plus(At(a,j),minusBb);
		}
	}
	return r;
}
var Scalar(Var x, Var y)
{
	size_t m = Size(y);
	size_t n = Size(At(y,0));
	var r = Vec(m);
	for(size_t i = 0; i < m; ++i)
	{
		var &c = At(r,i);
		c = Vec(n);
		for(size_t j = 0; j < n; ++j)
		{
			At(c,j)=Times(x,Entry(y,i,j));
		}
	}
	return r;
}
var VDot(Var x, Var y)
{
	size_t n = Size(x);
	var r = Vec(n);
	for(size_t i = 0; i < n; ++i)
		At(r,i)=Times(At(x,i),At(y,i));
	//Expand(r);
	return Expand(Plus(r));
}
var Dot(Var x,Var y)
{
	size_t m=Size(x);
	size_t p=Size(y);
	size_t n=Size(At(y,0));
	var r=Vec(m);
	for(size_t i=0;i<m;++i)
	{
		var &c = At(r,i);
		c=Vec(n);
		for(size_t j=0;j<n;++j)
		{
			var d=Vec(p);
			for(size_t k=0;k<p;++k)
				At(d,k)=Times(Entry(x,i,k),Entry(y,k,j));
			//Expand(d);
			At(At(r,i),j)=Expand(Plus(d));
		}
	}
	return r;
}
var Tr(Var x)
{
	size_t m = Size(x);
	var r = Vec(m);
	for(size_t i = 0; i < m; ++i)
		At(r,i)=Entry(x,i,i);
	//Sort(r);
	return Expand(Plus(r));
}
var KroneckerProduct(Var x,Var y)
{
	size_t m=Size(x);
	size_t n=Size(At(x,0));
	size_t p=Size(y);
	size_t q=Size(At(y,0));
	var r=Vec(m*p);
	for(size_t i=0;i<m*p;++i)
	{
		At(r,i)=Vec(n*q);
	}
	for(size_t i=0;i<m;++i)
	{
		for(size_t j=0;j<n;++j)
		{
			for(size_t t=0;t<p;++t)
			{
				for(size_t s=0;s<q;++s)	
					Entry(r,i*p+t,j*q+s)=Times(Entry(x,i,j),Entry(y,t,s));
			}
		}
	}
	return r;
}

}
}