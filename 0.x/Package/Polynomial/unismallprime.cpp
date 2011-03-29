/**
\file
\brief 小素数Berlekamp算法.
*/
#include "common.h"
#include <iostream>

namespace mU {

namespace{

typedef std::vector< std::vector<int> > mat_int;
typedef std::vector<int> poly_int;

void poly_z_to_poly_int(poly_int & r,const poly_z & f)
{
	r.resize(f.size());
	for(size_t i=0;i<f.size();i++)
	{
		r[i]=mpz_get_si(f[i]);
	}
	return ;
}

void poly_int_to_poly_z(poly_z & r,const poly_int & f)
{
	r.resize(f.size());
	for(size_t i=0;i<f.size();i++)
	{
		mpz_set_si(r[i],f[i]);
	}
	return ;
}

void UniPolynomialMod_SmallPrime(poly_int & r,const poly_int & f,int m)
{
	if(f.size()==0)
	{
		r.resize(0);
		return ;
	}
	r.resize(f.size());
	int rem;
	int m2=m/2;
	int i;
	for(i=0;i<f.size();i++)
	{
		rem=f[i]%m;
		if(rem>m2)rem=rem-m;
		if(rem<-m2)rem=rem+m;
		r[i]=rem;
	}
	for(i=r.size()-1;i>=0 && r[i]==0;i--);
	r.resize(i+1);
	return ;
}

void UniDivZp_SmallPrime(poly_int & q,const poly_int & f,const poly_int & g,int p)
{
	poly_int rem=f;
	int k,n;
	q.resize(0);
	n=g.size()-1;
	k=rem.size()-1-n;
	if(k<0)return ;
	q.resize(k+1,0);
	mpz_t temp1,temp2;
	mpz_init(temp1);mpz_init(temp2);
	mpz_set_si(temp1,g[n]);mpz_set_si(temp2,p);
	mpz_invert(temp1,temp1,temp2);
	int inverse=mpz_get_si(temp1);
	mpz_clear(temp1);mpz_clear(temp2);
	do
	{
		int q0=rem[n+k]*inverse%p;
		if(q0!=0)
		{
			q[k]=q0;
			for(int i=0;i<n;i++)
			{
				uint j=n+k-1-i;
				rem[j]=rem[j]-q0*g[j-k];
			}
		}
	} while (k--);
	UniPolynomialMod_SmallPrime(q,q,p);
	return ;
}

void UniModZp_SmallPrime(poly_int & r,const poly_int & f,const poly_int & g,int p)
{
	r=f;
	int k,n;
	n=g.size()-1;
	k=r.size()-1-n;
	if(k<0)return ;
	mpz_t temp1,temp2;
	mpz_init(temp1);mpz_init(temp2);
	mpz_set_si(temp1,g[n]);mpz_set_si(temp2,p);
	mpz_invert(temp1,temp1,temp2);
	int inverse=mpz_get_si(temp1);
	mpz_clear(temp1);mpz_clear(temp2);
	do
	{
		int q0=r[n+k]*inverse%p;
		if(q0!=0)
		{
			for(int i=0;i<n;i++)
			{
				uint j=n+k-1-i;
				r[j]=r[j]-q0*g[j-k];
			}
		}
	} while (k--);
	r.resize(n);
	UniPolynomialMod_SmallPrime(r,r,p);
	return ;
}

void PowerMod_Pth_Power_SmallPrime(poly_int & r,const poly_int & f,const poly_int & g,int p)
{
	int n=f.size()-1;
	poly_int rr;
	rr.resize(n*p+1,0);
	for(int i=n;i>=0;i--)
	{
		rr[i*p]=f[i];

	}
	UniModZp_SmallPrime(r,rr,g,p);
	return ;
}

void mul_poly_int(poly_int & r,const poly_int & x, const poly_int & y)
{
	if(x.size()==0||y.size()==0)
	{
		r.resize(0);
		return ;
	}
	r.resize(x.size()+y.size()-1);
	for(int i=0;i<r.size();i++)r[i]=0;
	for(int i=0;i<x.size();i++)
		for(int j=0;j<y.size();j++)
			r[i+j]=r[i+j]+x[i]*y[j];
	return ;
}

void UniGcdZp_SmallPrime(poly_int & u,const poly_int & f,const poly_int & g,int p)
{
	u=f;
	poly_int v=g,r;
	while(v.size()>= 1){
		UniModZp_SmallPrime(r,u,v,p);
		u=v;v=r;
	}
	int lc=u[u.size()-1];
	mpz_t temp1,temp2;
	mpz_init(temp1);mpz_init(temp2);
	mpz_set_si(temp1,lc);mpz_set_si(temp2,p);
	mpz_invert(temp1,temp1,temp2);
	int inverse=mpz_get_si(temp1);
	mpz_clear(temp1);mpz_clear(temp2);
	for(int i=0;i<u.size();i++)
	{
		u[i]=(u[i]*inverse)%p;
	}
	UniPolynomialMod_SmallPrime(u,u,p);
	return ;
}

void UniPetriMatrixZp_SmallPrime(mat_int & r,const poly_int & f,int p)
{
	int n=f.size()-1;
	r.resize(n);
	for (int i = 0; i < n; ++i)
	{
		r[i].resize(n);
	}
	poly_int x1;x1.push_back(0);x1.push_back(1);
	PowerMod_Pth_Power_SmallPrime(x1,x1,f,p);
	poly_int xi;xi.push_back(1);
	for(int j=0;j<n;j++)r[j][0]=0;
	r[0][0]=1;
	poly_int xi1;
	for(int i=1;i<n;i++)
	{
		for(int j=0;j<n;j++)r[j][i]=0;
		mul_poly_int(xi1,xi,x1);
		UniPolynomialMod_SmallPrime(xi1,xi1,p);
		UniModZp_SmallPrime(xi,xi1,f,p);
		for(int j=0;j<xi.size();j++)r[j][i]=xi[j];
	}
	return ;
}

void NullSpaceZp_SmallPrime(std::vector<poly_int> & basis,mat_int & m,uint n,int p)
{
	int tempz,inverse;
	std::vector<int> marks(0);
	std::vector<bool> exchangable(n,true);
	for(int i=0;i<n;i++)
	{
		if(m[i][i]==0)
		{
			for(int j=0;j<n;j++)
			{
				if(!exchangable[j])continue;
				if(m[j][i]==0)continue;
				for(int k=0;k<n;k++)
				{//to use swap in algorithm?
					tempz=m[i][k];
					m[i][k]=m[j][k];
					m[j][k]=tempz;
				}
				break;
			}
		}
		if(m[i][i]!=0)
		{
			mpz_t temp1,temp2;
			mpz_init(temp1);mpz_init(temp2);
			mpz_set_si(temp1,m[i][i]);mpz_set_si(temp2,p);
			mpz_invert(temp1,temp1,temp2);
			inverse=mpz_get_si(temp1);
			mpz_clear(temp1);mpz_clear(temp2);
			for(int j=0;j<n;j++)
			{
				if(j==i)continue;
				if (m[j][i]==0) continue;
				tempz=m[j][i]*inverse%p;
				m[j][i]=0;
				for(int k=i+1;k<n;k++)
				{
					m[j][k]=(m[j][k]-tempz*m[i][k])%p;
				}
			}
			m[i][i]=1;
			for(int k=i+1;k<n;k++)
			{
				m[i][k]=m[i][k]*inverse%p;
			}
			exchangable[i]=false;
		}
		else marks.push_back(i);
	}

	poly_int one_base;
	for(int i=0;i<marks.size();i++)
	{
		int j=marks[i];
		one_base.resize(n);
		for(int k=0;k<n;k++)one_base[k]=0;
		one_base[j]=1;
		for(int k=0;k<n;k++)
		{
			if(m[k][j]==0)continue;
			one_base[k]=-m[k][j];
		}
		UniPolynomialMod_SmallPrime(one_base,one_base,p);
		basis.push_back(one_base);
	}
	return ;
}

}

/**
\brief Berlekamp算法.
\param f 有限域上多项式.monic
\param p characteristic. small prime
\param faclist 返回因子list.
*/
void BerlekampZp_SmallPrime(const poly_z & f,mpz_ptr p,std::vector<poly_z> & faclist)
{
	int n=f.size()-1;
	clear_poly_z_list(faclist);
	std::vector<poly_int> basis_int;
	int k,r;
	mat_int matq;
	int p_int=mpz_get_si(p);
	poly_int f_int;
	poly_z_to_poly_int(f_int,f);
	//str("matrix begin").print();
	UniPetriMatrixZp_SmallPrime(matq,f_int,p_int);

	//for(int i=0;i<n;i++)
	//{
	//	for(int j=0;j<n;j++)
	//	{
	//		std::cout<<matq[i][j]<<" ";
	//	}
	//	std::cout<<"\n";
	//}

	//str("matrix end").print();
	for(int i=0;i<n;i++)matq[i][i]=matq[i][i]-1;
	NullSpaceZp_SmallPrime(basis_int,matq,n,p_int);
	//str("nullspace end").print();

	r=basis_int.size();
	//for(size_t i=0;i<r;i++)
	//{
	//	poly_z test;
	//	poly_int_to_poly_z(test,basis_int[i]);
	//	test.print();
	//}
	//
	mat_int result_int;
	poly_int bk_int,g_int;
	int a_int;
	int r_temp;
	result_int.push_back(f_int);
	while(result_int.size()<r)
	{
		random::randominteger(k,0,r-1);
		bk_int=basis_int[k];
		random::randominteger(a_int,0,p_int-1);
		if(bk_int.size()<=1)continue;
		bk_int[0]-=a_int;
		r_temp=result_int.size();
		for(int i=0;i<r_temp;i++)
		{
			UniGcdZp_SmallPrime(g_int,bk_int,result_int[i],p_int);
			if(g_int.size()>1&&result_int[i].size()>g_int.size())
			{
				UniDivZp_SmallPrime(result_int[i],result_int[i],g_int,p_int);
				result_int.push_back(g_int);
			}
		}
	}
	faclist.resize(r);
	for(size_t i=0;i<r;i++)
	{
		poly_int_to_poly_z(faclist[i],result_int[i]);
	}
	//str("berlekamp end").print();
}

}
