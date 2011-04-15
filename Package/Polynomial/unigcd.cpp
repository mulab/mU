/** \file 
\brief 一元多项式GCD.
*/
#include "common.h"

namespace mU {

namespace{

}

/**
\brief 基b下整数n的长度
*/
void IntegerLength_BigBase(mpz_ptr r,mpz_ptr n,mpz_ptr b)
{
	static mpz_t nn;
	mpz_init(nn);
	mpz_set(nn,n);
	mpz_set_ui(r,0);
	while(1)
	{
		if(mpz_size(nn)!=0)
		{
			mpz_add_ui(r,r,1);
			mpz_div(nn,nn,b);
		}
		else
		{
			mpz_clear(nn);
			return ;
		}
	}
}

/** \brief 整系数多项式无穷范数.
\param f 整系数多项式.
\param r 系数向量的无穷范数.
*/
void UniMaxNormZ(mpz_ptr r,const poly_z & f)
{
	mpz_set_ui(r,0);
	mpz_t tempz;
	mpz_init(tempz);
	for(int i=0;i<f.size();i++)
	{
		mpz_abs(tempz,f[i]);
		if(mpz_cmp(r,tempz)<0)mpz_set(r,tempz);
	}
	mpz_clear(tempz);
	return ;
}

/** \brief 整系数多项式1-范数.
\param f 整系数多项式.
\return 系数向量的1-范数.
*/
void UniOneNormZ(mpz_ptr r,const poly_z & f)
{
	mpz_set_ui(r,0);
	static mpz_t temp;
	mpz_init(temp);
	for(int i=0;i<f.size();i++)
	{
		mpz_abs(temp,f[i]);
		mpz_add(r,r,temp);
	}
	mpz_clear(temp);
}

/** \brief Fp域上多项式GCD.
\param f,g 域上多项式.
\param r 最大公因子.
\note Euclid算法.
*/
void UniGcdZp(poly_z & r,const poly_z & f,const poly_z & g,mpz_ptr p)
{
	poly_z u,v;
	poly_z q;
	copy_poly_z(u,f);
	copy_poly_z(v,g);
	while(v.size()> 0){
		UniDivModZp(q,r,u,v,p);
		UniPolynomialMod(r,r,p);
		u.resize(0);
		u=v;v=r;
		r=poly_z();
	}
	static mpz_t lc;
	mpz_init_set(lc,u[u.size()-1]);
	mpz_invert(lc,lc,p);
	r.resize(u.size());
	for(size_t i=0;i<u.size();i++)
	{
		mpz_mul(r[i],u[i],lc);
	}
	UniPolynomialMod(r,r,p);
	mpz_clear(lc);
	u.resize(0);v.resize(0);q.resize(0);
}

/** \brief Fp域上多项式GCD.
\param f,g 域上多项式.
\param r,s,t 其中r为最大公因子,s,t为Bezout系数多项式.
*/
void UniGcdZp_Ext(poly_z & r,poly_z & s,poly_z & t,const poly_z & f,const poly_z & g,mpz_ptr p)
{
	poly_z u,v;
	poly_z q;
	poly_z s1,s2,t1,t2,l1,l2;
	copy_poly_z(u,f);
	copy_poly_z(v,g);
	s1.resize(1);mpz_set_si(s1[0],1);
	s2.resize(0);
	t1.resize(0);
	t2.resize(1);mpz_set_si(t2[0],1);
	while(v.size()> 0){
		UniDivModZp(q,r,u,v,p);
		UniPolynomialMod(r,r,p);
		UniMulZ(l1,q,t1);UniSubZ(l1,s1,l1);
		UniMulZ(l2,q,t2);UniSubZ(l2,s2,l2);
		u.resize(0);u=v;v=r;r=poly_z();
		s1.resize(0);s1=t1;t1=l1;l1=poly_z();
		s2.resize(0);s2=t2;t2=l2;l2=poly_z();
	}
	static mpz_t lc;
	mpz_init_set(lc,u[u.size()-1]);
	mpz_invert(lc,lc,p);
	r.resize(u.size());
	for(size_t i=0;i<u.size();i++)
	{
		mpz_mul(r[i],u[i],lc);
	}
	UniPolynomialMod(r,r,p);
	s.resize(s1.size());
	for(size_t i=0;i<s1.size();i++)
	{
		mpz_mul(s[i],s1[i],lc);
	}
	UniPolynomialMod(s,s,p);
	t.resize(s2.size());
	for(size_t i=0;i<s2.size();i++)
	{
		mpz_mul(t[i],s2[i],lc);
	}
	UniPolynomialMod(t,t,p);
	mpz_clear(lc);
	u.resize(0);v.resize(0);q.resize(0);s1.resize(0);s2.resize(0);t1.resize(0);t2.resize(0);l1.resize(0);l2.resize(0);
}

/** \brief 整系数多项式的容度.
\param f 整系数多项式.
\param r 整数容度,包含正负号,使得本原部分lc为正整数.
*/
void UniContZ(mpz_ptr r,const poly_z & f)
{
	if(f.size()==0)
	{
		mpz_set_ui(r,1);
		return ;
	}
	mpz_set(r,f[f.size()-1]);
	if(f.size()==1)
	{
		return ;
	}
	int sign;
	sign=mpz_sgn(r);
	mpz_gcd(r,f[0],f[1]);
	if(mpz_cmp_ui(r,1)==0)
	{
		if(sign<0)mpz_neg(r,r);
		return ;
	}
	for(uint i=2;i<f.size();i++)
	{
		mpz_gcd(r,r,f[i]);
		if(mpz_cmp_ui(r,1)==0)
		{
			if(sign<0)mpz_neg(r,r);
			return ;
		}
	}
	if(sign<0)mpz_neg(r,r);
	return ;
}

/** \brief 整系数多项式本原部分.
\param f 整系数多项式.
\param r 本原部分.
\return 本原部分,其lc为正整数.
*/
void UniPPZ(poly_z & r,const poly_z & f)
{
	mpz_t a;
	mpz_init(a);
	UniContZ(a,f);
	r.resize(f.size());
	for(size_t i=0;i<f.size();i++)
	{
		mpz_divexact(r[i],f[i],a);
	}
	mpz_clear(a);
}

/** 
\brief 整系数多项式最大公因子.
\param f,g 整系数本原多项式,且\f$\deg f=n\ge\deg g\ge 1\f$.
\param r 最大公因子.
\note 小素数模方法.
\todo 理论文档此处有误.
*/
void UniGcdZ_SmallPrime1(poly_z & r,const poly_z & f,const poly_z & g)
{
	poly_z fp,gp,vp,v1;
	poly_z fstar,gstar;
	mpz_t c,s,t;
	mpz_init(c);mpz_init(s);mpz_init(t);
	mpz_t A,b,B,z_temp,k,p_bound,p,p_low_bound,p1,B2;
	mpf_t float_temp;
	mpz_init(A);mpz_init(b);mpz_init(B);mpz_init(z_temp);mpz_init(k);mpz_init(p_bound);mpz_init(p);mpz_init(p1);mpz_init(p_low_bound);mpf_init(float_temp);mpz_init(B2);
	UniMaxNormZ(A,f);UniMaxNormZ(b,g);
	int n=f.size()-1,m=g.size()-1;
	if(mpz_cmp(A,b)<0)mpz_set(A,b);
	mpz_gcd(b,f[n],g[m]);
	mpz_ui_pow_ui(B,2,n);
	mpz_mul(B,B,A);
	mpz_mul(B,B,b);
	mpf_sqrt_ui(float_temp,n+1);
	mpz_set_f(z_temp,float_temp);
	mpz_mul(B,B,z_temp);
	mpz_mul_ui(B2,B,2);
	mpz_set_si(z_temp,n);
	int tempint;
	tempint=2*n*mpz_sizeinbase(z_temp,2)+2*mpz_sizeinbase(b,2)+4*n*mpz_sizeinbase(A,2);
	//tempint=2*n*Modules::NumberTheory::IntegerLength(Z(n),2)+2*Modules::NumberTheory::IntegerLength(b,2)+4*n*Modules::NumberTheory::IntegerLength(A,2);
	mpz_set_si(k,tempint);
	mpz_mul_ui(p_bound,k,2);
	mpz_mul_ui(p_bound,p_bound,mpz_sizeinbase(k,2));
	mpz_set_ui(p_low_bound,3);
	fstar.resize(n+1);
	for(size_t i=0;i<=n;i++)
	{
		mpz_mul(fstar[i],f[i],b);
	}
	gstar.resize(m+1);
	for(size_t i=0;i<=m;i++)
	{
		mpz_mul(gstar[i],g[i],b);
	}
	while(1)
	{
		while(1)
		{
			random::randominteger(p,p_low_bound,p_bound);
			if(mpz_probab_prime_p(p,10)>0&&mpz_divisible_p(b,p)==0)break;
		}
		UniPolynomialMod(fp,f,p);
		UniPolynomialMod(gp,g,p);
		UniGcdZp(vp,fp,gp,p);
		if(vp.size()==1)
		{
			r.resize(1);
			mpz_set_si(r[0],1);
			fp.resize(0);gp.resize(0);vp.resize(0);v1.resize(0);
			fstar.resize(0);gstar.resize(0);
			mpz_clear(c);mpz_clear(s);mpz_clear(t);
			mpz_clear(A);mpz_clear(b);mpz_clear(B);mpz_clear(z_temp);mpz_clear(k);mpz_clear(p_bound);mpz_clear(p);mpz_clear(p1);mpz_clear(p_low_bound);mpf_clear(float_temp);
			return ;
		}
		mpz_set(p1,p);
		copy_poly_z(v1,vp);
		while(mpz_cmp(p1,B2)<0)
		{
			while(1)
			{
				random::randominteger(p,p_low_bound,p_bound);
				if(mpz_probab_prime_p(p,10)>0&&mpz_divisible_p(b,p)==0)break;
			}
			UniPolynomialMod(fp,f,p);
			UniPolynomialMod(gp,g,p);
			UniGcdZp(vp,fp,gp,p);
			if(vp.size()==1)
			{
				r.resize(1);
				mpz_set_si(r[0],1);
				fp.resize(0);gp.resize(0);vp.resize(0);v1.resize(0);
				fstar.resize(0);gstar.resize(0);
				mpz_clear(c);mpz_clear(s);mpz_clear(t);
				mpz_clear(A);mpz_clear(b);mpz_clear(B);mpz_clear(z_temp);mpz_clear(k);mpz_clear(p_bound);mpz_clear(p);mpz_clear(p1);mpz_clear(p_low_bound);mpf_clear(float_temp);
				return ;
			}
			if(vp.size()<v1.size())
			{
				mpz_set(p1,p);
				copy_poly_z(v1,vp);
				continue;
			}
			if(vp.size()==v1.size())
			{
				mpz_gcdext(c,s,t,p1,p);
				mpz_mul(t,p,t);
				mpz_mul(s,p1,s);
				mpz_mul(p1,p1,p);
				for(size_t i=0;i<vp.size();i++)
				{
					mpz_mul(c,v1[i],t);
					mpz_addmul(c,vp[i],s);
					mpz_set(v1[i],c);
				}
				UniPolynomialMod(v1,v1,p1);
			}
		}
		for(size_t i=0;i<v1.size();i++)mpz_mul(v1[i],v1[i],b);
		UniPolynomialMod(v1,v1,p1);
		if(poly_z_divisible(fstar,v1)&&poly_z_divisible(gstar,v1))
		{
			UniPPZ(r,v1);break;
		}
	}
	fp.resize(0);gp.resize(0);vp.resize(0);v1.resize(0);
	fstar.resize(0);gstar.resize(0);
	mpz_clear(c);mpz_clear(s);mpz_clear(t);
	mpz_clear(A);mpz_clear(b);mpz_clear(B);mpz_clear(z_temp);mpz_clear(k);mpz_clear(p_bound);mpz_clear(p);mpz_clear(p1);mpz_clear(p_low_bound);mpf_clear(float_temp);
	return ;
}

/** 
\brief 整系数多项式最大公因子.
\param f,g 整系数多项式.
\return 最大公因子.
\note 调用大素数算法或小素数算法.
*/
void UniGcdZ(poly_z & r,const poly_z & f,const poly_z & g)
{
	uint option=1;
	if(f.size()==0)
	{
		copy_poly_z(r,g);
		return ;
	}
	if(g.size()==0)
	{
		copy_poly_z(r,f);
		return ;
	}
	mpz_t contf,contg,b;
	mpz_init(contf);mpz_init(contg);mpz_init(b);
	UniContZ(contf,f);UniContZ(contg,g);
	mpz_gcd(b,contf,contg);
	poly_z _f,_g,h;
	UniPPZ(_f,f);UniPPZ(_g,g);
	int n=_f.deg(),m=_g.deg();
	if(n==0||m==0)
	{
		h.resize(1);
		mpz_set_ui(h[0],1);
	}
	else
	{
		if(n<m)UniGcdZ_SmallPrime1(h,_g,_f);
		else UniGcdZ_SmallPrime1(h,_f,_g);
	}
	r.resize(h.size());
	for(size_t i=0;i<h.size();i++)
	{
		mpz_mul(r[i],h[i],b);
	}
	_f.resize(0);_g.resize(0);h.resize(0);
	mpz_clear(contf);mpz_clear(contg);mpz_clear(b);
	return ;
}

}
