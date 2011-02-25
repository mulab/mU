/**
\file
\brief 有限域Zp上多项式因子分解.
*/
#include "common.h"

namespace mU {

namespace{

void UniSqrFreeDecomZp_Plain(const poly_z & f,std::vector<poly_z> & sqflist,mpz_ptr p)
{
	poly_z fd,u,v1,w1,v1d,h,temp;
	clear_poly_z_list(sqflist);
	poly_z_dform(fd,f);
	UniPolynomialMod(fd,fd,p);
	UniGcdZp(u,f,fd,p);
	UniDivZp(v1,f,u,p);
	UniDivZp(w1,fd,u,p);
	poly_z_dform(v1d,v1);
	UniPolynomialMod(v1d,v1d,p);
	while(1)
	{
		UniSubZ(temp,w1,v1d);
		UniPolynomialMod(temp,temp,p);
		UniGcdZp(h,v1,temp,p);
		sqflist.resize(sqflist.size()+1);
		copy_poly_z(sqflist[sqflist.size()-1],h);
		UniDivZp(temp,v1,h,p);
		copy_poly_z(v1,temp);
		UniSubZ(temp,w1,v1d);
		UniPolynomialMod(temp,temp,p);
		UniDivZp(w1,temp,h,p);
		poly_z_dform(v1d,v1);
		UniPolynomialMod(v1d,v1d,p);
		if(v1.size()==1&&mpz_cmp_si(v1[0],1)==0)break;
	}
	fd.resize(0);u.resize(0);v1.resize(0);w1.resize(0);v1d.resize(0);h.resize(0);temp.resize(0);
	return;
}

//be sure f is pth power of some polynomials in Zp[x]
void pthroot(poly_z & f,mpz_ptr p)
{
	uint n=f.size()-1;
	uint p_uint=mpz_get_ui(p);
	uint m=n/p_uint;
	for(uint i=1;i<=m;i++)
	{
		mpz_set(f[i],f[i*p_uint]);
	}
	f.resize(m+1);
	return ;
}

void power_specially_for_eqldegfactorization(poly_z & a,const poly_z & f,mpz_ptr p,uint d)
{
	poly_z poly_temp1,poly_temp2;
	static mpz_t exp;
	mpz_init(exp);
	copy_poly_z(poly_temp1,a);
	for(uint i=1;i<d;i++)
	{
		PowerMod_Pth_Power(poly_temp1,f,p);
		UniMulZ(poly_temp2,poly_temp1,a);
		UniModZp(a,poly_temp2,f,p);
	}
	mpz_sub_ui(exp,p,1);
	mpz_fdiv_q_2exp(exp,exp,1);
	PowerMod(a,exp,f,p);
	poly_temp1.resize(0);
	poly_temp2.resize(0);
	mpz_clear(exp);
	return ;
}

//todo
void UniEqlDegFacZp_2(poly_z & factor,const poly_z & f,uint d)
{
	return ;
}

//todo using iterated frobenius alg. to improve it?
void UniEqlDegFacZp_Odd(poly_z & factor,const poly_z & f,mpz_ptr p,uint d)
{
	uint deg_bound=f.size()-2;
	poly_z a;
	while(1)
	{
		UniRandomZp(a,deg_bound,p);
		if(a.size()>1)break;
	}
	UniGcdZp(factor,a,f,p);
	if(factor.size()>1&&factor.size()<f.size())
	{
		a.resize(0);
		return ;
	}

	power_specially_for_eqldegfactorization(a,f,p,d);

	//static mpz_t exp;
	//mpz_init(exp);
	//mpz_pow_ui(exp,p,d);
	//mpz_sub_ui(exp,exp,1);
	//mpz_divexact_ui(exp,exp,2);
	//PowerMod(a,exp,f,p);
	//mpz_clear(exp);

	mpz_sub_ui(a[0],a[0],1);
	UniGcdZp(factor,a,f,p);
	if(factor.size()>1&&factor.size()<f.size())
	{
		a.resize(0);
		return; 
	}
	a.resize(0);
	factor.resize(0);
	return ;
}

void UniEqlDegFacZp(std::vector<poly_z> & eqlfactorlist,const poly_z & f,mpz_ptr p,uint d,uint & current)
{
	uint n=f.size()-1;
	if(n==d)
	{
		copy_poly_z(eqlfactorlist[current],f);
		current++;
		return ;
	}
	poly_z g,h;
	while(1)
	{
		UniEqlDegFacZp_Odd(g,f,p,d);
		if(g.size()>=d)break;
	}
	UniEqlDegFacZp(eqlfactorlist,g,p,d,current);
	UniDivZp(h,f,g,p);
	UniEqlDegFacZp(eqlfactorlist,h,p,d,current);
	g.resize(0);h.resize(0);
	return ;
}

bool factor_compare(const poly_z & f,const poly_z & g)
{
	if(f.size()<g.size())return 1;
	if(f.size()>g.size())return 0;
	for(size_t i=f.size()-1;i>=0;i--)
	{
		if(mpz_cmp(f[i],g[i])<0)return 1;
		if(mpz_cmp(f[i],g[i])>0)return 0;
	}
	return 0;
}

}

/**
\brief 快速多项式环中幂算法.
\param f in Zp[x].
\param n 幂次数.
\param p 特征.
*/
void Power(poly_z & f,uint n,mpz_ptr p)
{
	poly_z z,poly_temp;
	copy_poly_z(z,f);
	f.resize(1);mpz_set_ui(f[0],1);
	uint m=n;
	while(1)
	{
		if(m%2==1)
		{
			UniMulZ(poly_temp,f,z);
			UniPolynomialMod(f,poly_temp,p);
			UniMulZ(poly_temp,z,z);
			UniPolynomialMod(z,poly_temp,p);
		}
		else
		{
			UniMulZ(poly_temp,z,z);
			UniPolynomialMod(z,poly_temp,p);
		}
		m>>=1;
		if(m==0)break;
	}
	z.resize(0);
	poly_temp.resize(0);
	return ;
}

/**
\brief 快速多项式模环中幂算法.
\param f in Zp[x].
\param n 幂次数.
\param g 模多项式,in Zp[x].
\param p 特征.
*/
void PowerMod(poly_z & f,mpz_ptr n,const poly_z & g,mpz_ptr p)
{
	poly_z z,poly_temp;
	copy_poly_z(z,f);
	f.resize(1);mpz_set_ui(f[0],1);
	static mpz_t m;
	mpz_init(m);
	mpz_set(m,n);
	while(1)
	{
		if(mpz_odd_p(m)!=0)
		{
			UniMulZ(poly_temp,f,z);
			UniModZp(f,poly_temp,g,p);
			UniMulZ(poly_temp,z,z);
			UniModZp(z,poly_temp,g,p);
		}
		else
		{
			UniMulZ(poly_temp,z,z);
			UniModZp(z,poly_temp,g,p);
		}
		mpz_div_2exp(m,m,1);
		if(mpz_size(m)==0)break;
	}
	mpz_clear(m);
	z.resize(0);
	poly_temp.resize(0);
	return ;
}

/**
\brief 快速多项式模环中p次幂算法.
\param f in Zp[x].
\param g 模多项式,in Zp[x].
\param p 特征.
*/
void PowerMod_Pth_Power(poly_z & f,const poly_z & g,mpz_ptr p)
{
	uint n=f.size()-1,p_uint=mpz_get_ui(p);
	poly_z result;
	result.resize(n*p_uint+1);
	uint i=n*p_uint;
	while(1)
	{
		if(i%p_uint==0)
		{
			mpz_set(result[i],f[i/p_uint]);
		}
		else
		{
			mpz_set_ui(result[i],0);
		}
		if(i==0)break;
		i--;
	}
	UniModZp(f,result,g,p);
	result.resize(0);
	return ;
}

/**
\brief 不同次因子分解.
\param f in Zp[x], monic.
\param p characteristic.
\param disfactorlist 返回的不同次因子
\param deglist 返回的因子次数
*/
void UniDisDegFacZp(std::vector<poly_z> & disfactorlist, std::vector<uint> & deglist,const poly_z & f,mpz_ptr p)
{
	clear_poly_z_list(disfactorlist);
	deglist.resize(0);
	poly_z h0,f0,h,g;
	h0.resize(2);mpz_set_ui(h0[0],0);mpz_set_ui(h0[1],1);
	copy_poly_z(f0,f);
	uint n,n2;
	n=f0.size()-1;n2=n/2;
	copy_poly_z(h,h0);
	uint i=1;
	while(i<=n2)
	{
		//PowerMod_Pth_Power(h,f,p);//This is far slower, so why?
		PowerMod(h,p,f,p);
		UniSubZ(g,h,h0);
		UniGcdZp(g,g,f0,p);
		if(g.size()>1)
		{
			UniDivZp(f0,f0,g,p);
			n=f0.size()-1;n2=n/2;
			disfactorlist.push_back(g);g=poly_z();
			deglist.push_back(i);
		}
		if(f0.size()==1&&mpz_cmp_ui(f0[0],1)==0)break;
		i++;
	}
	if(f0.size()!=1||mpz_cmp_ui(f0[0],1)!=0)
	{
		disfactorlist.push_back(f0);f0=poly_z();
		deglist.push_back(n);
	}
	h0.resize(0);f0.resize(0);h.resize(0);g.resize(0);
	return ;
}

/**
\brief 有限域随机多项式生成.
\param deg_bound 次数的上界.
\param p characteristic.
\param r 随机多项式,其次数不超过deg_bound.
*/
void UniRandomZp(poly_z & r,uint deg_bound,mpz_ptr p)
{
	r.resize(deg_bound+1);
	static mpz_t low,high;
	mpz_init(low);mpz_init(high);
	mpz_set_ui(low,0);mpz_sub_ui(high,p,1);
	for(uint i=0;i<=deg_bound;i++)
	{
		random::randominteger(r[i],low,high);
	}
	UniPolynomialMod(r,r,p);
	return ;
}

/**
\brief 有限域上同次因子分解.
\param f in Zp[x], monic.
\param p characteristic.
\param d 各不可约因子的次数.
\param eqlfactorlist f的同次因子分解结果
*/
void UniEqlDegFacZp(std::vector<poly_z> & eqlfactorlist,const poly_z & f,mpz_ptr p,uint d)
{
	uint n=f.size()-1,current=0;
	clear_poly_z_list(eqlfactorlist);
	eqlfactorlist.resize(n/d);
	UniEqlDegFacZp(eqlfactorlist,f,p,d,current);
	return ;
}

/**
\brief 无平方因子多项式的分解.
\param f 无平方因子多项式, monic.
\param p characteristic.
\return f的不可约因子分解\f$(f_1,f_2,\ldots,f_k)\f$.
*/
void UniFacZp_fsqfmonic(std::vector<poly_z> & faclist,const poly_z & f,mpz_ptr p)
{
	std::vector<poly_z> disfactorlist,eqlfactorlist;
	std::vector<uint> deglist;
	clear_poly_z_list(faclist);
	UniDisDegFacZp(disfactorlist,deglist,f,p);
	for(uint i=0;i<deglist.size();i++)
	{
		UniEqlDegFacZp(eqlfactorlist,disfactorlist[i],p,deglist[i]);
		uint m1=faclist.size(),m2=eqlfactorlist.size();
		faclist.resize(m1+m2);
		for(uint j=0;j<m2;j++)
		{
			faclist[j+m1]=eqlfactorlist[j];
			eqlfactorlist[j]=poly_z();
		}
	}
	std::sort(faclist.begin(),faclist.end(),factor_compare);
	clear_poly_z_list(disfactorlist);
	clear_poly_z_list(eqlfactorlist);
	return ;
}

void UniSqrFreeDecomZp(const poly_z & f,std::vector<poly_z> & sqflist,mpz_ptr p)
{
	std::vector<poly_z> hlist;
	poly_z f1,poly_temp1,poly_temp2;
	clear_poly_z_list(sqflist);
	UniSqrFreeDecomZp_Plain(f,hlist,p);
	f1.resize(1);mpz_set_ui(f1[0],1);
	for(uint i=0;i<hlist.size();i++)
	{
		copy_poly_z(poly_temp1,hlist[i]);
		Power(poly_temp1,i+1,p);
		UniMulZ(poly_temp2,f1,poly_temp1);
		UniPolynomialMod(f1,poly_temp2,p);
	}
	UniDivZp(poly_temp1,f,f1,p);
	if(poly_temp1.size()==1&&mpz_cmp_ui(poly_temp1[0],1)==0)
	{
		sqflist=hlist;
		f1.resize(0);poly_temp1.resize(0);poly_temp2.resize(0);
		return ;
	}
	//todo
	pthroot(poly_temp1,p);
	std::vector<poly_z> slist;
	UniSqrFreeDecomZp(poly_temp1,slist,p);
	uint k=hlist.size(),p_uint=mpz_get_ui(p);
	for(uint i=k;i<p_uint-1;i++)
	{
		hlist.push_back(poly_z());
		hlist[i].resize(1);mpz_set_ui(hlist[i][0],1);
	}
	uint l=slist.size();
	sqflist.resize((l+1)*p_uint);
	for(uint j=1;j<=l;j++)
	{
		poly_temp1.resize(1);mpz_set_ui(poly_temp1[0],1);
		for(uint i=1;i<p_uint;i++)
		{
			UniGcdZp(sqflist[j*p_uint+i-1],hlist[i-1],slist[j-1],p);
			UniMulZ(poly_temp2,poly_temp1,sqflist[j*p_uint+i-1]);
			UniPolynomialMod(poly_temp1,poly_temp2,p);
		}
		UniDivZp(sqflist[j*p_uint-1],slist[j-1],poly_temp1,p);
	}
	for(uint i=1;i<p_uint;i++)
	{
		poly_temp1.resize(1);mpz_set_ui(poly_temp1[0],1);
		for(uint j=1;j<=l;j++)
		{
			UniMulZ(poly_temp2,poly_temp1,sqflist[j*p_uint+i-1]);
			UniPolynomialMod(poly_temp1,poly_temp2,p);
		}
		UniDivZp(sqflist[i-1],hlist[i-1],poly_temp1,p);
	}

	for(uint i=sqflist.size()-1;sqflist[i].size()<=1;i--)
	{
		sqflist[i].resize(0);
		sqflist.erase(sqflist.begin()+i);
	}

	f1.resize(0);poly_temp1.resize(0);poly_temp2.resize(0);
	clear_poly_z_list(hlist);
	clear_poly_z_list(slist);
	return ;
}

/**
\brief 有限域上多项式因子分解.
\param f in Zp[x].
\param p characteristic.
\todo 有限域无平方分解
*/
void UniFacZp(const poly_z & f,mpz_ptr p,mpz_ptr b,std::vector<poly_z> & faclist,std::vector<uint> & exponents)
{
	clear_poly_z_list(faclist);
	exponents.resize(0);
	if(f.size()==0)
	{
		mpz_set_ui(b,1);
		return ;
	}
	if(f.size()==1)
	{
		mpz_set(b,f[0]);
		return ;
	}
	poly_z _f;
	mpz_set(b,f[f.size()-1]);
	UniPolynomialMod(_f,f,p);
	if(mpz_cmp_ui(b,1)!=0)
	{
		static mpz_t inverse;
		mpz_init(inverse);
		mpz_invert(inverse,b,p);
		for(uint i=0;i<_f.size();i++)
		{
			mpz_mul(_f[i],_f[i],inverse);
		}
		UniPolynomialMod(_f,_f,p);
		mpz_clear(inverse);
	}
	std::vector<poly_z> sqffactorlist,sqfmoniclist;

	UniSqrFreeDecomZp(_f,sqffactorlist,p);
	for(uint i=0;i<sqffactorlist.size();i++)
	{
		if(sqffactorlist[i].size()>1)
		{
			UniFacZp_fsqfmonic(sqfmoniclist,sqffactorlist[i],p);
			for(uint j=0;j<sqfmoniclist.size();j++)
			{
				faclist.push_back(sqfmoniclist[j]);
				sqfmoniclist[j]=poly_z();
				exponents.push_back(i+1);
			}
		}
	}
	_f.resize(0);
	clear_poly_z_list(sqffactorlist);
	clear_poly_z_list(sqfmoniclist);
	return ;
}

}
