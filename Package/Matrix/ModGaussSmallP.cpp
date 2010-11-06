#include "Matrix.h"

namespace mU {
namespace Matrix {
//用无符号整型数组表示矩阵，是本文件中函数的处理对象。
typedef std::vector<std::vector<uint> > Mat;
//在函数开始前应通过模运算将输入矩阵转换成uint矩阵输入，从而本文件中的函数只处理uint型矩阵。
//利用大整数矩阵M通过模小素数prime得到小整数矩阵R，用无符号整型数组表示。
void mMod(Mat &r,Var M,uint prime)
{
	size_t m=Size(M);
	size_t n=Size(At(M,0));
	r.resize(m);
	for(size_t i=0;i<m;++i)
	{
		r[i].resize(n);
		for(size_t j=0;i<n;++j)
			r[i][j]=Z::Mod(Entry(M,i,j),prime);
	}
}
//M一般矩阵
//M非奇异，则对M进行PLU分解，返回List{Au,p,norm},其中Au密集存储LU分解的结果，而p为置换阵的信息，norm存储矩阵范数的估值，对于精确矩阵，非奇异则标记为1，奇异情形标记为-1。
//M奇异，继续进行Row echelon约化。
//prime is prime.
void mLU(Mat& Au,std::vector<uint>& pi,uint m,uint n,uint prime)
{
	pi.resize(m);
	std::vector<int> p(m);
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
			if(Au[j][col])
			{
				p[row]=j;
				fl=true;
				break;
			}
		}
		if(fl)
		{
			if(row!=p[row])
				std::swap(Au[row],Au[p[row]]);
			for(size_t k=row+1;k<m;++k)
			{
				if(Au[k][col])
				{
					Au[k][col]=DivMod(Au[k][col],Au[row][col],prime);
					if(col!=n-1)
					{
						for(size_t l=col+1;l<n;++l)
							Au[k][l]=SubMod(Au[k][l],Au[k][col]*Au[row][l],prime);
					}
				}
			}
			++row;
		}
	}
	for(size_t i=0;i<m;++i)
	{
		if(i!=p[i])
			std::swap(pi[i],pi[p[i]]);
	}
}
//Au为传入的增广矩阵。
//solution需预先resize成为m*pb大小的矩阵。
bool mLinearSolve(Mat& solution,Mat& M,Mat& b,uint m,uint pm,uint pb,uint prime)
{
	size_t n=pm+pb;
	Mat Au(m);
	for(size_t i=0;i<m;++i)
	{
		Au[i].resize(n);
		for(size_t j=0;j<pm;++j)
			Au[i][j]=M[i][j];
		for(size_t j=pm;j<n;++j)
			Au[i][j]=b[i][j-pm];
	}
	std::vector<int> p(m);
	for(size_t i=0;i<m;++i)
		p[i]=i;
	size_t row=0;
	for(size_t col=0;col<pm;++col)
	{
		if(row==m-1)
			break;
		bool fl=false;
		for(size_t j=row;j<m;++j)
		{
			if(Au[j][col])
			{
				p[row]=j;
				fl=true;
				break;
			}
		}
		if(fl)
		{
			if(row!=p[row])
				std::swap(Au[row],Au[p[row]]);
			for(size_t k=row+1;k<m;++k)
			{
				if(Au[k][col])
				{
					uint t;
					t=DivMod(Au[k][col],Au[row][col],prime);
					Au[k][col]=0;
					if(col!=n-1)
					{
						for(size_t l=col+1;l<n;++l)
							Au[k][l]=SubMod(Au[k][l],t*Au[row][l],prime);
					}
				}
			}
			++row;
		}
	}
	uint ele;
	bool mark=false;
	int j;
	for(int i=m-1;i>=0;--i)
	{
		for(j=0;j<n;++j)
		{
			if(Au[i][j])
			{
				ele=Au[i][j];
				mark=true;
				break;
			}
		}
		if(mark)
		{
			if(j>=pm)return false;
			else 
			{
				for(int l=j;l<n;++l)
					Au[i][l]=DivMod(Au[i][l],ele,prime);
				if(i!=0)
				{
					for(int k=i-1;k>=0;--k)
					{
						for(int jj=n-1;jj>=j;--jj)
							Au[k][jj]=SubMod(Au[k][jj],Au[i][jj]*Au[k][j],prime);
					}
				}
			}
		}
	}
	int col=-1;
#undef min
	size_t temp=std::min(m,pm);
	for(int i=0;i<temp;++i)
	{
		for(j=col+1;j<pm;++j)
		{
			if(Au[i][j])
			{
				col=j;
				for(int k=0;k<pb;++k)
					solution[j][k]=Au[i][pm+k];
				break;
			}
			else
			{
				for(int k=0;k<pb;++k)
					solution[j][k]=0;
			}
			if(j==pm-1)return true;
		}
	}
	for(size_t ii=col+1;ii<pm;++ii)
	{
		for(size_t jj=0;jj<pb;++jj)
			solution[ii][jj]=0;
	}
	return true;
}
void mRowReduce(Mat& Au,uint m,uint n,uint prime)
{
	std::vector<int> p(m);
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
			if(Au[j][col])
			{
				p[row]=j;
				fl=true;
				break;
			}
		}
		if(fl)
		{
			if(row!=p[row])
				std::swap(Au[row],Au[p[row]]);
			for(size_t k=row+1;k<m;++k)
			{
				if(Au[k][col])
				{
					uint t=DivMod(Au[k][col],Au[row][col],prime);
					Au[k][col]=0;
					if(col!=n-1)
					{
						for(size_t l=col+1;l<n;++l)
							Au[k][l]=SubMod(Au[k][l],t*Au[row][l],prime);
					}
				}
			}
			++row;
		}
	}
	uint ele;
	bool mark=false;
	int j;
	for(int i=m-1;i>=0;--i)
	{
		for(j=0;j<n;++j)
			if(Au[i][j])
			{
				ele=Au[i][j];
				mark=true;
				break;
			}
			if(mark)
			{
				for(int l=j;l<n;++l)
					Au[i][l]=DivMod(Au[i][l],ele,prime);
				if(i!=0)
				{
					for(int k=i-1;k>=0;--k)
					{
						for(int jj=n-1;jj>=j;--jj)
							Au[k][jj]=SubMod(Au[k][jj],Au[i][jj]*Au[k][j],prime);
					}
				}
			}
	}
}
//用NullSpace矩阵表示零空间的一组基，其大小由函数内部决定。
void mNullSpace(Mat& solutions,Mat& Au,uint m,uint n,uint prime)
{
	mRowReduce(Au,m,n,prime);
	if((m>=n)&&(Au[n-1][n-1]))//列独立阵
		return;
	std::vector<int> ind(n+1),dep(n+1);
	ind[0]=-1;
	size_t j,r=0,t=0;
	for(size_t i=1;i<=m&&i<=n;++i)
	{
		for(j=ind[i-1]+1;j<n;++j)
		{
			if(Au[i-1][j])
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
	solutions.resize(t);
	for(int k=t-1;k>=0;--k)
	{
		solutions[t-1-k].resize(n);
		solutions[t-1-k][dep[k+1]]=1;
		for(int l=0;l<t;++l)
		{
			if(l!=k)
				solutions[t-1-k][dep[l+1]]=0;
		}
		for(int l=0;l<r;++l)
			solutions[t-1-k][ind[l+1]]=prime-Au[l][dep[k+1]];
	}
}
bool mInverse(Mat& R,Mat& M,uint n,uint prime)
{
	Mat I(n);
	for(size_t i=0;i<n;++i)
	{
		I[i].resize(n);
		for(size_t j=0;j<n;++j)
		{
			if(i!=j)
				I[i][j]=0;
			else
				I[i][j]=1;
		}
	}
	return mLinearSolve(R,M,I,n,n,n,prime);
}
uint mDet(Mat& Au,uint n,uint prime)
{
	bool sign=true;//行交换的奇偶性
	std::vector<int> p(n);
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
			if(Au[j][col])
			{
				p[row]=j;
				fl=true;
				break;
			}
		}
		if(!fl)
			return 0;
		else
		{
			if(row!=p[row])
			{
				std::swap(Au[row],Au[p[row]]);
				sign=!sign;
			}
			for(size_t k=row+1;k<n;++k)
			{
				if(Au[k][col])
				{
					uint t=DivMod(Au[k][col],Au[row][col],prime);
					Au[k][col]=0;
					if(col!=n-1)
					{
						for(size_t l=col+1;l<n;++l)
							Au[k][l]=SubMod(Au[k][l],t*Au[row][l],prime);
					}
				}
			}
			++row;
		}
	}
	uint det=1;
	for(size_t ii=0;ii<n;++ii)
		det=MulMod(det,Au[ii][ii],prime);
	return det;
}
size_t mMatrixRank(Mat& Au,uint m,uint n,uint prime)
{
	std::vector<int> p(m);
	for(size_t i=0;i<m;++i)
		p[i]=i;
	size_t row=0;
	for(size_t col=0;col<n;++col)
	{
		if(row==m-1)
		{	
			bool flag=false;
			for(size_t i=col;i<n;++i)
				if(Au[row][i])++row;
			break;
		}
		bool fl=false;
		for(size_t j=row;j<m;++j)
		{
			if(Au[j][col])
			{
				p[row]=j;
				fl=true;
				break;
			}
		}
		if(fl)
		{
			if(row!=p[row])
				std::swap(Au[row],Au[p[row]]);
			for(size_t k=row+1;k<m;++k)
			{
				if(Au[k][col])
				{
					uint t=DivMod(Au[k][col],Au[row][col],prime);
					Au[k][col]=0;
					if(col!=n-1)
					{
						for(size_t l=col+1;l<n;++l)
							Au[k][l]=SubMod(Au[k][l],t*Au[row][l],prime);
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
