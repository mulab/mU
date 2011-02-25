#include "Matrix.h"

namespace mU {
namespace Matrix {
// var mMod(Var M,Var p)
// {
// 	size_t m=Size(M);
// 	size_t n=Size(At(M,0));
// 	var r=Vec(m);
// 	for(size_t i=0;i<m;++i)
// 	{
// 		At(r,i)=Vec(n);
// 		for(size_t j=0;i<n;++j)
// 		{
// 			Entry(r,i,j)=Int();
// 			Z::Mod(Entry(r,i,j),Entry(M,i,j),p);
// 		}
// 	}
// 	return r;
// }
void mMod(Var r,Var M,Var p)
{
	size_t m=Size(M);
	size_t n=Size(At(M,0));
	r=Vec(m);
	for(size_t i=0;i<m;++i)
	{
		At(r,i)=Vec(n);
		for(size_t j=0;i<n;++j)
		{
			Entry(r,i,j)=Int();
			Z::Mod(Entry(r,i,j),Entry(M,i,j),p);
		}
	}
}
//M一般矩阵
//M非奇异，则对M进行PLU分解，返回List{Au,p,norm},其中Au密集存储LU分解的结果，而p为置换阵的信息，norm存储矩阵范数的估值，对于精确矩阵，非奇异则标记为1，奇异情形标记为-1。
//M奇异，继续进行Row echelon约化。
//prime is prime.
var mLU(Var M,Var prime)
{
	size_t m=Size(M);
	size_t n=Size(At(M,0));
	var Au;
	mMod(Au,M,prime);
	std::vector<sint> p(m), pi(m);
	for(size_t i=0;i<m;++i)
	{
		p[i]=i;
		pi[i]=i+1;
	}
	size_t row=0;
	for(size_t col=0;col<n;++col)
	{
		if(row==m-1)
			break;
		bool fl=false;
		for(size_t j=row;j<m;++j)
		{
			if(Z::Sgn(Entry(Au,j,col))!=0)
			{
				p[row]=j;
				fl=true;
				break;
			}
		}
		if(fl)
		{
			if(row!=p[row])
				std::swap(At(Au,row),At(Au,p[row]));
			for(size_t k=row+1;k<m;++k)
			{
				if(Z::Sgn(Entry(Au,k,col)))
				{
					DivMod(Entry(Au,k,col),Entry(Au,k,col),Entry(Au,row,col),prime);
					if(col!=n-1)
					{
						for(size_t l=col+1;l<n;++l)
							SubMod(Entry(Au,k,l),Entry(Au,k,l),N::Mul(Entry(Au,k,col),Entry(Au,row,l)),prime);
					}
				}
			}
			++row;
		}
		//Println(Au);
	}
	for(size_t i=0;i<m;++i)
	{
		if(i!=p[i])
			std::swap(pi[i],pi[p[i]]);
	}
	var pv=Vec(m);
	for(size_t i=0;i<m;++i)
		At(pv,i)=Int(pi[i]);
	return Vec(Au,pv,Int(1L));
}
var mLinearSolve(Var M, Var b, Var prime)
{
	size_t m=Size(M);
	size_t pm=Size(At(M,0));
	size_t pb=Size(At(b,0));
	size_t n=pm+pb;
	var Au=Vec(m);
	std::vector<int> p(m);
	for(size_t i=0;i<m;++i)
	{
		p[i]=i;
		At(Au,i)=Vec(n);
		for(size_t j=0;j<pm;++j)
		{
			Entry(Au,i,j)=Int();
			Z::Mod(Entry(Au,i,j),Entry(M,i,j),prime);
		}
		for(size_t j=0;j<pb;++j)
		{
			Entry(Au,i,j+pm)=Int();
			Z::Mod(Entry(Au,i,pm+j),Entry(b,i,j),prime);
		}
	}
	size_t row=0;
	for(size_t col=0;col<pm;++col)
	{
		if(row==m-1)
			break;
		var temp=Int(0L);
		bool fl=false;
		for(size_t j=row;j<m;++j)
		{
			if(Z::Sgn(Entry(Au,j,col))!=0)
			{
				p[row]=j;
				fl=true;
				break;
			}
		}
		if(fl)
		{
			if(row!=p[row])
				std::swap(At(Au,row),At(Au,p[row]));
			for(size_t k=row+1;k<m;++k)
			{
				if(Z::Sgn(Entry(Au,k,col)))
				{
					var t=Int();
					DivMod(t,Entry(Au,k,col),Entry(Au,row,col),prime);
					Entry(Au,k,col)=Int(0L);
					if(col!=n-1)
					{
						for(size_t l=col+1;l<n;++l)
							SubMod(Entry(Au,k,l),Entry(Au,k,l),N::Mul(t,Entry(Au,row,l)),prime);
					}
				}
			}
			++row;
		}
	}
	var ele;
	bool mark=false;
	int j;
	for(int i=m-1;i>=0;--i)
	{
		for(j=0;j<n;++j)
		{
			if(Q::Sgn(Entry(Au,i,j)))
			{
				ele=Int(Entry(Au,i,j));
				mark=true;
				break;
			}
		}
		if(mark)
		{
			if(j>=pm)return Au;
			else 
			{
				for(int l=j;l<n;++l)
					DivMod(Entry(Au,i,l),Entry(Au,i,l),ele,prime);
				if(i!=0)
				{
					for(int k=i-1;k>=0;--k)
					{
						for(int jj=n-1;jj>=j;--jj)
							SubMod(Entry(Au,k,jj),Entry(Au,k,jj),N::Mul(Entry(Au,i,jj),Entry(Au,k,j)),prime);
					}
				}
			}
		}
	}
	var solution=Vec(pm);
	for(size_t i=0;i<pm;++i)
		At(solution,i)=Vec(pb);
	var zero=Int(0L);
	int col=-1;
#undef min
	size_t temp=std::min(m,pm);
	for(int i=0;i<temp;++i)
	{
		for(j=col+1;j<pm;++j)
		{
			if(Z::Sgn(Entry(Au,i,j)))
			{
				col=j;
				for(int k=0;k<pb;++k)
					Entry(solution,j,k)=Entry(Au,i,pm+k);
				break;
			}
			else
			{
				for(int k=0;k<pb;++k)
					Entry(solution,j,k)=zero;
			}
			if(j==pm-1)return solution;
		}
	}
	for(size_t ii=col+1;ii<pm;++ii)
	{
		for(size_t jj=0;jj<pb;++jj)
			Entry(solution,ii,jj)=zero;
	}
	return solution;
}
var mRowReduce(Var M,Var prime)
{
	size_t m=Size(M);
	size_t n=Size(At(M,0));
	var Au;
	var zero=Int(0L);
	std::vector<sint> p(m);
	mMod(Au,M,prime);
	for(size_t i=0;i<m;++i)
		p[i]=i;
	size_t row=0;
	for(size_t col=0;col<n;++col)
	{
		if(row==m-1)
			break;
		bool fl=false;
		for(size_t j=row;j<m;++j)
		{
			if(Z::Sgn(Entry(Au,j,col))!=0)
			{
				p[row]=j;
				fl=true;
				break;
			}
		}
		if(fl)
		{
			if(row!=p[row])
				std::swap(At(Au,row),At(Au,p[row]));
			for(size_t k=row+1;k<m;++k)
			{
				if(Z::Sgn(Entry(Au,k,col)))
				{
					var t=Int();
					DivMod(t,Entry(Au,k,col),Entry(Au,row,col),prime);
					Entry(Au,k,col)=zero;
					if(col!=n-1)
					{
						for(size_t l=col+1;l<n;++l)
							SubMod(Entry(Au,k,l),Entry(Au,k,l),N::Mul(t,Entry(Au,row,l)),prime);
					}
				}
			}
			++row;
		}
	}
	var ele;
	bool mark=false;
	int j;
	for(int i=m-1;i>=0;--i)
	{
		for(j=0;j<n;++j)
			if(Z::Sgn(Entry(Au,i,j)))
			{
				ele=Int(Entry(Au,i,j));
				mark=true;
				break;
			}
			if(mark)
			{
				for(int l=j;l<n;++l)
					DivMod(Entry(Au,i,l),Entry(Au,i,l),ele,prime);
				if(i!=0)
				{
					for(int k=i-1;k>=0;--k)
					{
						for(int jj=n-1;jj>=j;--jj)
							SubMod(Entry(Au,k,jj),Entry(Au,k,jj),N::Mul(Entry(Au,i,jj),Entry(Au,k,j)),prime);
					}
				}
			}
	}
	return Au;
}
var mNullSpace(Var M,Var prime)
{
	size_t m=Size(M);
	size_t n=Size(At(M,0));
	var Au=mRowReduce(M,prime);
	if((m>=n)&&(Z::Sgn(Entry(Au,n-1,n-1))))//列独立阵
		return Vec();
	std::vector<int> ind(n+1),dep(n+1);
	ind[0]=-1;
	size_t j,r=0,t=0;
	for(size_t i=1;i<=m&&i<=n;++i)
	{
		for(j=ind[i-1]+1;j<n;++j)
		{
			if(Z::Sgn(Entry(Au,i-1,j)))
			{
				++r;
				ind[i]=j;
				break;
			}
			else
			{
				++t;
				dep[t]=j;
			}
		}
		if(j>=n-1)break;
	}
	if(j<n-1)
	{
		t+=n-j-1;
		for(int ii=t-n+j+2;ii<=t;++ii)
			dep[ii]=j+ii;
	}
	var solutions=Vec(t);
	var zero=Int(0L);
	for(int k=t-1;k>=0;--k)
	{

		At(solutions,t-1-k)=Vec(n);
		Entry(solutions,t-1-k,dep[k+1])=Int(1L);
		for(int l=0;l<t;++l)
		{
			if(l!=k)
				Entry(solutions,t-1-k,dep[l+1])=zero;
		}
		for(int l=0;l<r;++l)
			Entry(solutions,t-1-k,ind[l+1])=N::Neg(Entry(Au,l,dep[k+1]));
	}
	return solutions;
}
var mInverse(Var M,Var prime)
{
	size_t n=Size(M);
	var I=iIdentityMatrix(n);
	return mLinearSolve(M,I,prime);
}
var mDet(Var M,Var prime)
{
	var zero=Int(0L);
	size_t n=Size(M);
	bool sign=true;//行交换的奇偶性
	var Au;
	mMod(Au,M,prime);
	std::vector<sint> p(n);
	for(size_t i=0;i<n;++i)
		p[i]=i;
	size_t row=0;
	for(size_t col=0;col<n;++col)
	{
		if(row==n-1)
			break;
		bool fl=false;
		for(size_t j=row;j<n;++j)
		{
			if(Z::Sgn(Entry(Au,j,col))!=0)
			{
				p[row]=j;
				fl=true;
				break;
			}
		}
		if(!fl)
			return zero;
		else
		{
			if(row!=p[row])
			{
				std::swap(At(Au,row),At(Au,p[row]));
				sign=!sign;
			}
			for(size_t k=row+1;k<n;++k)
			{
				if(Z::Sgn(Entry(Au,k,col)))
				{
					var t=Int();
					DivMod(t,Entry(Au,k,col),Entry(Au,row,col),prime);
					Entry(Au,k,col)=zero;
					if(col!=n-1)
					{
						for(size_t l=col+1;l<n;++l)
							SubMod(Entry(Au,k,l),Entry(Au,k,l),N::Mul(t,Entry(Au,row,l)),prime);
					}
				}
			}
			++row;
		}
	}
	var det=Int(1L);
	for(size_t ii=0;ii<n;++ii)
		MulMod(det,det,Entry(Au,ii,ii),prime);
	return det;
}
size_t mMatrixRank(Var M,Var prime)
{
	size_t m=Size(M);
	size_t n=Size(At(M,0));
	var Au;
	mMod(Au,M,prime);
	var zero=Int(0L);
	std::vector<sint> p(m);
	for(size_t i=0;i<m;++i)
		p[i]=i;
	size_t row=0;
	for(size_t col=0;col<n;++col)
	{
		if(row==m-1)
		{	
			bool flag=false;
			for(size_t i=col;i<n;++i)
				if(Z::Sgn(Entry(Au,row,i)))++row;
			break;
		}
		bool fl=false;
		for(size_t j=row;j<m;++j)
		{
			if(Z::Sgn(Entry(Au,j,col))!=0)
			{
				p[row]=j;
				fl=true;
				break;
			}
		}
		if(fl)
		{
			if(row!=p[row])
				std::swap(At(Au,row),At(Au,p[row]));
			for(size_t k=row+1;k<m;++k)
			{
				if(Z::Sgn(Entry(Au,k,col)))
				{
					var t=Int();
					DivMod(t,Entry(Au,k,col),Entry(Au,row,col),prime);
					Entry(Au,k,col)=zero;
					if(col!=n-1)
					{
						for(size_t l=col+1;l<n;++l)
							SubMod(Entry(Au,k,l),Entry(Au,k,l),N::Mul(t,Entry(Au,row,l)),prime);
					}
				}
			}
			++row;
		}
	}
	return row;
}

}
}
