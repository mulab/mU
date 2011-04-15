#include "Matrix.h"

namespace mU{
namespace Matrix{
var StrassenDMM(Var A,Var B,size_t m,size_t p,size_t n,size_t bound)//m为M行数，n为N列数，p为n行数，bound为递归下界
{		
	if(m<=bound||n<=bound||p<=bound)
		return Matrix::Dot(A,B);
	else
	{
		var A11=Take(A,1,m/2,1,p/2);
		var A12=Take(A,1,m/2,p/2+1,p);
		var A21=Take(A,m/2+1,m,1,p/2);
		var A22=Take(A,m/2+1,m,p/2+1,p);
		var B11=Take(B,1,p/2,1,n/2);
		var B12=Take(B,1,p/2,n/2+1,n);
		var B21=Take(B,p/2+1,p,1,n/2);
		var B22=Take(B,p/2+1,p,n/2+1,n);

		var S1=Matrix::Add(A21,A22);
		var S2=Matrix::Sub(S1,A11);
		var S3=Matrix::Sub(A11,A21);
		var S4=Matrix::Sub(A12,S2);
		var T1=Matrix::Sub(B12,B11);
		var T2=Matrix::Sub(B22,T1);
		var T3=Matrix::Sub(B22,B12);
		var T4=Matrix::Sub(T2,B21);

		var P1=Matrix::StrassenDMM(A11,B11,m/2,p/2,n/2,bound);
		var P2=Matrix::StrassenDMM(A12,B21,m/2,p/2,n/2,bound);
		var P3=Matrix::StrassenDMM(S4,B22,m/2,p/2,n/2,bound);
		var P4=Matrix::StrassenDMM(A22,T4,m/2,p/2,n/2,bound);
		var P5=Matrix::StrassenDMM(S1,T1,m/2,p/2,n/2,bound);
		var P6=Matrix::StrassenDMM(S2,T2,m/2,p/2,n/2,bound);
		var P7=Matrix::StrassenDMM(S3,T3,m/2,p/2,n/2,bound);

		var U1=Matrix::Add(P1,P2);
		var U2=Matrix::Add(P1,P6);
		var U3=Matrix::Add(U2,P7);
		var U4=Matrix::Add(U2,P5);
		var U5=Matrix::Add(U4,P3);
		var U6=Matrix::Sub(U3,P4);
		var U7=Matrix::Add(U3,P5);

		var r=Vec(m);
		for (size_t i=0;i<m;++i)
		{
			var &c=At(r,i);
			c=Vec(n);
		}
		for (size_t i=0;i<m/2;++i)
		{
			for (size_t j=0;j<n/2;++j)
			{
				Entry(r,i,j)=Entry(U1,i,j);
			}
		}
		for (size_t i=0;i<m/2;++i)
		{
			for (size_t j=n/2;j<n;++j)
			{
				Entry(r,i,j)=Entry(U5,i,j-n/2);
			}
		}
		for (size_t i=m/2;i<m;++i)
		{
			for (size_t j=0;j<n/2;++j)
			{
				Entry(r,i,j)=Entry(U6,i-m/2,j);
			}
		}
		for (size_t i=m/2;i<m;++i)
		{
			for (size_t j=n/2;j<n;++j)
			{
				Entry(r,i,j)=Entry(U7,i-m/2,j-n/2);
			}
		}
		return r;
	}
}
var StrassenMM(Var M,Var N)
{
	const size_t bound=64;//递归下界
	size_t m=Size(M),p=Size(N),n=Size(At(N,0));
	if (m<bound || p<bound || n<bound)
	{
		return Matrix::Dot(M,N);
	}
 	size_t mm=bound,pp=bound,nn=bound;
 	while(mm<m)mm*=2;
 	while(pp<p)pp*=2;
 	while(nn<n)nn*=2;
 	var A=qPadRight(M,mm,pp);
 	var B=qPadRight(N,pp,nn);
	return Take(StrassenDMM(A,B,mm,pp,nn,bound),1,m,1,n);
}
}
}
