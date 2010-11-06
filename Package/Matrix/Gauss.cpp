#include "Matrix.h"

namespace mU{
namespace Matrix{
var LinearSolve(Var M, Var b)
{
	bool vect=false;//用来判断b是否为一个横向量
	var b_normal;

	//若b为横向量，将它转化为竖向量，最后输出的解再转化为横向两，
	//这是为了与Mathematica统一
	if (!VecQ(At(b,0)))
	{			
		b_normal=Transpose(getMatrixFromVector(b));
		vect=true;
	}
	else
		b_normal=b;	


	size_t m=Size(M);
	size_t pm=Size(At(M,0));
	size_t pb=Size(At(b_normal,0));
	size_t n=pm+pb;
	var Au=Vec(m);
	std::vector<sint> p(m);
	var zero=Rat(0.0);
	

	//Au为拓展矩阵
	for(size_t i=0;i<m;++i)
	{
		//flag[i]=true;
		p[i]=i;
		At(Au,i)=Vec(n);
		for(size_t j=0;j<pm;++j)
		{
			if (IntQ(Entry(M,i,j)))
			{
				Entry(Au,i,j)=Rat();
				Q::SetZ(Entry(Au,i,j),Entry(M,i,j));				
			}else
				Entry(Au,i,j)=Entry(M,i,j);
		}
		for(size_t j=0;j<pb;++j)
			Entry(Au,i,pm+j)=Entry(b_normal,i,j);
	}


	
	//打洞
	size_t row=0;
	for(size_t col=0;col<pm;++col)
	{
		if(row==m-1)
			break;
		var temp;
		bool fl=false;//表示至少此列有一个非零元素
		for(size_t j=row;j<m;++j)
		{
			if(N::Sgn(Entry(Au,j,col))!=0)
			{
				temp=N::Abs(Entry(Au,j,col));
				p[row]=j;
				fl=true;
				break;
			}
		}
		if(fl)
		{
			//寻找此列最小非零元
			for(size_t j=p[row];j<m;++j)
			{
				var c=N::Abs(Entry(Au,j,col));
				if(N::Sgn(c)&&N::Cmp(c,temp)<0)
				{
					temp=c;
					p[row]=j;
				}
			}

			//判断是否需要进行行变换
			if(row!=p[row])
				std::swap(At(Au,row),At(Au,p[row]));


			
			//打洞
			for(size_t k=row+1;k<m;++k)
			{
				if(N::Sgn(Entry(Au,k,col))!=0)
				{	
					var t=Rat();
					t=N::Div(Entry(Au,k,col),Entry(Au,row,col));
					t=Times(Int(-1L),t);

					Entry(Au,k,col)=zero;
					for(size_t l=col+1;l<n;++l)
					{
						Entry(Au,k,l)=Expand(Plus(Entry(Au,k,l),Times(t,Entry(Au,row,l))));
					}					
				}
			}
			++row;
		}
	}


	//向上打洞
	var ele;
	bool mark=false;//表示某一行是否有非零元
	int j;
	for(int i=m-1;i>=0;--i)
	{
		for(j=0;j<n;++j)
		{
			if (!ZeroQ(Entry(Au,i,j)))
			{				
				mark=true;
				break;
			}
		}
		

		//如果有非零元
		if(mark)
		{
			if(j>=pm)
				return 0;//判断无解
			else 
			{
				ele=Entry(Au,i,j);
				ele=N::Div(Rat(1.0),ele);
				
				for(int l=j;l<n;++l)
					Entry(Au,i,l)=Times(Entry(Au,i,l),ele);
				if(i!=0)
				{
					for(int k=i-1;k>=0;--k)
					{
						for(int jj=n-1;jj>=j;--jj)
						{
							Entry(Au,k,jj)=Expand(Plus(Entry(Au,k,jj),Times(Int(-1L),Times(Entry(Au,i,jj),Entry(Au,k,j)))));
						}
					}
				}
			}
		}
	}


	//结果输出
	var solution=Vec(pm);
	for(size_t i=0;i<pm;++i)
		At(solution,i)=Vec(pb);
	int column=-1;
#undef min
	size_t temp=std::min(m,pm);
	for(int i=0;i<temp;++i)
	{
		for(int j=column+1;j<pm;++j)
		{
			if(N::Sgn(Entry(Au,i,j)))
			{
				column=j;
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
	for(size_t ii=column+1;ii<pm;++ii)
	{
		for(size_t jj=0;jj<pb;++jj)
			Entry(solution,ii,jj)=zero;
	}
	if (vect)
	{
		solution=getVectorFromMatrix(Transpose(solution));
	}
	return solution;
}
//M一般矩阵
//M非奇异，则对M进行PLU分解，返回List{Au,p,norm},其中Au密集存储LU分解的结果，而p为置换阵的信息，norm存储矩阵范数的估值，对于精确矩阵，非奇异则标记为1，奇异情形标记为-1。
//M奇异，继续进行Row echelon约化。
var qLU(Var M)
{
	size_t m=Size(M);
	size_t n=Size(At(M,0));
	//int flag=1;//标记奇异与否
	var Au=Vec(m);
	std::vector<sint> p(m), pi(m);
	for(size_t i=0;i<m;++i)
	{
		p[i]=i;
		pi[i]=i+1;
		At(Au,i)=Vec(n);
		for(size_t j=0;j<n;++j)
			Entry(Au,i,j)=Rat(Entry(M,i,j));
	}
	//Println(Au);
	size_t row=0;
	for(size_t col=0;col<n;++col)
	{
		if(row==m-1)
			break;
		var temp=Rat(0.0);
		bool fl=false;
		//std::cout<<fl;
		for(size_t j=row;j<m;++j)
		{
			if(Q::Sgn(Entry(Au,j,col))!=0)
			{
				temp=N::Abs(Entry(Au,j,col));
				p[row]=j;
				fl=true;
				break;
			}
		}
		//std::cout<<fl;
		//Println(Au);
		//for(size_t i=0;i<m;++i)
		//std::cout<<p[i];
		//Println(temp);
		if(fl)
		{
			for(size_t j=p[row];j<m;++j)
			{
				var c=N::Abs(Entry(Au,j,col));
				if(Q::Sgn(c)!=0&&Q::Cmp(c,temp)<0)
				{
					temp=Rat(c);
					p[row]=j;
				}
			}
			//for(size_t i=0;i<m;++i)
			//	std::cout<<p[i]<<row<<col;
			//Println(Au);
			if(row!=p[row])
				std::swap(At(Au,row),At(Au,p[row]));
			//Println(Au);
			for(size_t k=row+1;k<m;++k)
			{
				if(Q::Sgn(Entry(Au,k,col)))
				{
					Q::Div(Entry(Au,k,col),Entry(Au,k,col),Entry(Au,row,col));
					if(col!=n-1)
					{
						for(size_t l=col+1;l<n;++l)
							Q::Sub(Entry(Au,k,l),Entry(Au,k,l),N::Mul(Entry(Au,k,col),Entry(Au,row,l)));
					}
					//Println(Au);
				}
			}
			++row;
		}
		//else
		//	flag=0;
		//Println(Au);
		//std::cout<<flag;
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
var qRowReduce(Var M)
{
	size_t m=Size(M);
	size_t n=Size(At(M,0));
	var Au=Vec(m);
	std::vector<sint> p(m);
	
	for(size_t i=0;i<m;++i)
	{
		p[i]=i;
		At(Au,i)=Vec(n);
		for(size_t j=0;j<n;++j)
		{
			if (IntQ(Entry(M,i,j)))
			{
				Entry(Au,i,j)=Rat();
				Q::SetZ(Entry(Au,i,j),Entry(M,i,j));
			}
			else
				Entry(Au,i,j)=Entry(M,i,j);
		}
	}

	size_t row=0;
	for(size_t col=0;col<n;++col)
	{
		if(row==m-1)
			break;
		var temp=Rat(0.0);
		bool fl=false;
		//std::cout<<fl;
		for(size_t j=row;j<m;++j)
		{
			if(N::Sgn(Entry(Au,j,col))!=0)
			{
				temp=N::Abs(Entry(Au,j,col));
				p[row]=j;
				fl=true;
				break;
			}
		}
		if(fl)
		{
			for(size_t j=p[row];j<m;++j)
			{
				var c=N::Abs(Entry(Au,j,col));
				if(N::Sgn(c)&&N::Cmp(c,temp)<0)
				{
					temp=c;
					p[row]=j;
				}
			}
			if(row!=p[row])
				std::swap(At(Au,row),At(Au,p[row]));
			for(size_t k=row+1;k<m;++k)
			{
				if(N::Sgn(Entry(Au,k,col)))
				{
					var t=Rat();
					t=N::Div(Entry(Au,k,col),Entry(Au,row,col));
					Entry(Au,k,col)=Rat(0.0);
					if(col!=n-1)
					{
						for(size_t l=col+1;l<n;++l)
							Entry(Au,k,l)=N::Sub(Entry(Au,k,l),N::Mul(t,Entry(Au,row,l)));
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
			if(N::Sgn(Entry(Au,i,j)))
			{
				ele=Entry(Au,i,j);
				mark=true;
				break;
			}
			if(mark)
			{
				for(int l=j;l<n;++l)
					Entry(Au,i,l)=N::Div(Entry(Au,i,l),ele);
				if(i!=0)
				{
					for(int k=i-1;k>=0;--k)
					{
						for(int jj=n-1;jj>=j;--jj)
							Entry(Au,k,jj)=N::Sub(Entry(Au,k,jj),N::Mul(Entry(Au,i,jj),Entry(Au,k,j)));
					}
				}
			}
	}
	return Au;
}
var qNullSpace(Var M)
{
	size_t m=Size(M);
	size_t n=Size(At(M,0));
	var Au=qRowReduce(M);
	if((m>=n)&&(N::Sgn(Entry(Au,n-1,n-1))))//列独立阵
		return Vec();
//	std::cout<<0;
	std::vector<sint> ind(n+1),dep(n+1);
	ind[0]=-1;
	size_t j,r=0,t=0;
	for(size_t i=1;i<=m&&i<=n;++i)
	{
		for(j=ind[i-1]+1;j<n;++j)
		{
			if(N::Sgn(Entry(Au,i-1,j)))
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
	var zero=Rat(0.0);
	for(int k=t-1;k>=0;--k)
	{

		At(solutions,t-1-k)=Vec(n);
		//std::cout<<n<<dep[k+1];
		Entry(solutions,t-1-k,dep[k+1])=Rat(1.0);
		//Println(solutions);
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
var qInverse(Var M)
{
	size_t n=Size(M);
	var I=qIdentityMatrix(n);
	return LinearSolve(M,I);
}
var qDet(Var M)
{
	size_t n=Size(M);
	bool sign=true;//行交换的奇偶性
	var Au=Vec(n);
	std::vector<sint> p(n);
	//std::vector<bool> flag(m);

	
	for(size_t i=0;i<n;++i)
	{
		//flag[i]=true;
		p[i]=i;
		At(Au,i)=Vec(n);
		for(size_t j=0;j<n;++j)
		{
			if (IntQ(Entry(M,i,j)))
			{
				Entry(Au,i,j)=Rat();
				Q::SetZ(Entry(Au,i,j),Entry(M,i,j));
			}
			else
				Entry(Au,i,j)=Entry(M,i,j);
		}
	}

	size_t row=0;
	for(size_t col=0;col<n;++col)
	{
		if(row==n-1)
			break;
		var temp=Rat(0.0);
		bool fl=false;
		for(size_t j=row;j<n;++j)
		{
			if(N::Sgn(Entry(Au,j,col))!=0)
			{
				temp=N::Abs(Entry(Au,j,col));
				p[row]=j;
				fl=true;
				break;
			}
		}
		
		if(!fl)
			return Rat(0.0);
		else
		{
			for(size_t j=p[row];j<n;++j)
			{
				var c=N::Abs(Entry(Au,j,col));
				if(N::Sgn(c) && N::Cmp(c,temp)<0)
				{
					temp=c;
					p[row]=j;
				}
			}
			if(row!=p[row])
			{
				std::swap(At(Au,row),At(Au,p[row]));
				sign=!sign;
			}
			for(size_t k=row+1;k<n;++k)
			{
				if(N::Sgn(Entry(Au,k,col)))
				{
					var t=N::Div(Entry(Au,k,col),Entry(Au,row,col));
					Entry(Au,k,col)=Rat(0.0);
					if(col!=n-1)
					{
						for(size_t l=col+1;l<n;++l)
							Entry(Au,k,l)=N::Sub(Entry(Au,k,l),N::Mul(t,Entry(Au,row,l)));
					}
				}
			}
			++row;
		}
	}
	var det=Rat(1.0);
	for(size_t ii=0;ii<n;++ii)
		det=N::Mul(det,Entry(Au,ii,ii));
	if (sign)
		return det;
	else
		return N::Neg(det);
}
var qMinors(Var M)
{
	size_t n=Size(M);
	var r=Vec(n);
	for(size_t i=0;i<n;++i)
	{
		At(r,i)=Vec(n);
		for(size_t j=0;j<n;++j)
			Entry(r,i,j)=qDet(Drop(M,n-1-i,n-1-j));
	}
	return M;
}
var qMinors(var M, size_t k)
{
	size_t m=Size(M);
	size_t n=Size(At(M,0));
	size_t *c=new size_t[k];
	size_t *d=new size_t[k];
	for(size_t i=0;i<k;++i)
		c[i]=d[i]=i;
	size_t row=Z::UI(Function::Binomial(Int((uint)m),k)),
		col=Z::UI(Function::Binomial(Int((uint)n),k));
	var R=Vec(row);
	size_t i=0;
	do
	{
		At(R,i)=Vec(col);
		size_t j=0;
		do
		{	
			Entry(R,i,j)=qMinor(M,c,d,k);
			++j;
		}while(next_comb(d,n,k));
		++i;
	}while(next_comb(c,m,k));
	delete []c;
	delete []d;
	return R;
}
var qMinor(Var M,size_t *c,size_t *d,size_t k)
{
	var temp=Vec(k);
	for(size_t i=0;i<k;++i)
	{
		At(temp,i)=Vec(k);
		for(size_t j=0;j<k;++j)
			Entry(temp,i,j)=Entry(M,c[i],d[j]);
	}
	return qDet(temp);
}
size_t qMatrixRank(Var M)
{
	size_t m=Size(M);
	size_t n=Size(At(M,0));
	var Au=Vec(m);
	std::vector<sint> p(m);
	for(size_t i=0;i<m;++i)
	{
		p[i]=i;
		At(Au,i)=Vec(n);
		for(size_t j=0;j<n;++j)
		{
			if (IntQ(Entry(M,i,j)))
			{
				Entry(Au,i,j)=Rat();
				Q::SetZ(Entry(Au,i,j),Entry(M,i,j));
			}
			else
				Entry(Au,i,j)=Entry(M,i,j);
		}
	}
	size_t row=0;
	for(size_t col=0;col<n;++col)
	{
		if(row==m)
		{	
			break;
		}
		var temp=Rat(0.0);
		bool fl=false;
		//std::cout<<fl;
		for(size_t j=row;j<m;++j)
		{
			if(N::Sgn(Entry(Au,j,col))!=0)
			{
				temp=N::Abs(Entry(Au,j,col));
				p[row]=j;
				fl=true;
				break;
			}
		}
		if(fl)
		{
			for(size_t j=p[row];j<m;++j)
			{
				var c=N::Abs(Entry(Au,j,col));
				if(N::Sgn(c)&&N::Cmp(c,temp)<0)
				{
					temp=c;
					p[row]=j;
				}
			}
			if(row!=p[row])
				std::swap(At(Au,row),At(Au,p[row]));
			for(size_t k=row+1;k<m;++k)
			{
				if(N::Sgn(Entry(Au,k,col)))
				{
					var t=Rat();
					t=N::Div(Entry(Au,k,col),Entry(Au,row,col));
					Entry(Au,k,col)=Rat(0.0);
					if(col!=n-1)
					{
						for(size_t l=col+1;l<n;++l)
							Entry(Au,k,l)=N::Sub(Entry(Au,k,l),N::Mul(t,Entry(Au,row,l)));
					}
				}
			}
			
			++row;
		}
	}
	
	return row;
}
var LeastSquares(Var M,Var b)
{
	bool vect=false;
	var b_normal;
	if (!VecQ(At(b,0)))
	{
		b_normal=Transpose(getMatrixFromVector(b));
		vect=true;
	}
	else
		b_normal=b;
	var Mt=Transpose(M);
	var MtM=Matrix::Dot(Mt,M);
	var Mtb=Matrix::Dot(Mt,b_normal);

	if (vect)
	{
		return getVectorFromMatrix(Transpose(LinearSolve(MtM,Mtb)));
	}
	return LinearSolve(MtM,Mtb);
}
}
}
