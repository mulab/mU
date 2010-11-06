/**
\file
\brief 多元多项式GCD in Zp[X]
*/
#include "common.h"

namespace mU {

namespace{

void cont_in_last_zp(poly_z & r,const sparse_z & f,mpz_ptr p)
{
	uint n=f[0].exponents.size();
	std::vector<poly_z> fc;
	sparse_z sparse_temp;
	uint i=0,j=0,sf=f.size(),k;
	while(1)
	{
		while(j<sf&&equal_mono_z_type(f[i],f[j],n-1))j++;
		sparse_temp.resize(j-i);
		for(k=i;k<j;k++)
		{
			mpz_set(sparse_temp[k-i].coe,f[k].coe);
			sparse_temp[k-i].exponents.resize(1);
			sparse_temp[k-i].exponents[0]=f[k].exponents[n-1];
		}
		fc.resize(fc.size()+1);
		sparse_z_to_poly_z(fc[fc.size()-1],sparse_temp);
		i=j;
		if(i==sf)break;
	}
	sparse_temp.resize(0);
	copy_poly_z(r,fc[0]);
	sf=fc.size();
	if(sf==1||(r.size()==1&&mpz_cmp_ui(r[0],1)==0))
	{
		return ;
	}
	for(i=1;i<sf;i++)
	{
		UniGcdZp(r,r,fc[i],p);
		if(r.size()==1)if(mpz_cmp_ui(r[0],1)==0)return ;
	}
	return ;
}

void pp_in_last_zp(sparse_z & r,const sparse_z & f,mpz_ptr p)
{
	if(f.size()==0)
	{
		r.resize(0);
		return ;
	}
	uint n=f[0].exponents.size();
	poly_z cont;
	sparse_z sparse_cont;
	cont_in_last_zp(cont,f,p);
	poly_z_to_sparse_z(sparse_cont,cont,n,n-1);
	MultiDivExactZp(r,f,sparse_cont,p);
	cont.resize(0);
	sparse_cont.resize(0);
	return ;
}

void lc_in_last_zp(poly_z & r,const sparse_z & f,mpz_ptr p)
{
	uint n=f[0].exponents.size();
	sparse_z sparse_temp;
	uint sf=f.size();
	int i=sf-1,j=sf-1,k;

		while(j>-1&&equal_mono_z_type(f[i],f[j],n-1))--j;
		sparse_temp.resize(i-j);
		for(k=i;k>j;--k)
		{
			mpz_set(sparse_temp[i-k].coe,f[k].coe);
			sparse_temp[i-k].exponents.resize(1);
			sparse_temp[i-k].exponents[0]=f[k].exponents[n-1];
		}
		sparse_z_to_poly_z(r,sparse_temp);

	sparse_temp.resize(0);
	return ;
}

void eval_in_last_zp(sparse_z & r,const sparse_z & f,mpz_ptr x,mpz_ptr p)
{
	std::vector<sparse_z> fc;
	uint n=f[0].exponents.size();
	to_main_variable(fc,f,n-1);
	uint sf=fc.size();
	uint i=sf-1;
	copy_sparse_z(r,fc[i]);
	if(i==0)
	{
		clear_sparse_z_list(fc);
		return ;
	}
	--i;
	while(1)
	{
		MultiMulZ(r,r,x);
		MultiAddZ(r,r,fc[i]);
		MultiPolynomialMod(r,r,p);
		if(i==0)
		{
			clear_sparse_z_list(fc);
			return ;
		}
		--i;
	}
	clear_sparse_z_list(fc);
	return ;
}

}

//f and g should normalized first
void MultiGcdZp_Dense_Interpolation(sparse_z & r,const sparse_z & f, const sparse_z & g,uint n, mpz_ptr p)
{
	if(n==1)// problem of univariate polynomials
	{
		poly_z ff,gg,rr;
		sparse_z_to_poly_z(ff,f);
		sparse_z_to_poly_z(gg,g);
		UniGcdZp(rr,ff,gg,p);
		poly_z_to_sparse_z(r,rr,1,0);
		ff.resize(0);gg.resize(0);rr.resize(0);
		return ;
	}
	poly_z contf,contg,poly_temp,a,b;
	sparse_z _f,_g,sparse_temp;
	cont_in_last_zp(contf,f,p);
	cont_in_last_zp(contg,g,p);
	UniGcdZp(a,contf,contg,p);
	poly_z_to_sparse_z(sparse_temp,contf,n,n-1);
	MultiDivExactZp(_f,f,sparse_temp,p);
	poly_z_to_sparse_z(sparse_temp,contg,n,n-1);
	MultiDivExactZp(_g,g,sparse_temp,p);
	lc_in_last_zp(contf,_f,p);
	lc_in_last_zp(contg,_g,p);
	UniGcdZp(b,contf,contg,p);
	sparse_z q,h,fv,gv,hv;
	poly_z q_one_v;
	sparse_z h_temp;
	uint m,m1,size;
	mpz_t v,bv,inverse;
	q.resize(1);mpz_set_ui(q[0].coe,1);q[0].exponents.resize(n,0);
	q_one_v.resize(1);mpz_set_ui(q_one_v[0],1);
	h.resize(1);mpz_set_ui(h[0].coe,1);h[0].exponents.resize(n,0);
	m=std::min<uint>(_f.deg(0),_g.deg(0))+1;
	mpz_init(v);mpz_init(bv);mpz_init(inverse);
	mpz_set_si(v,-1);
	while(1)
	{
		mpz_add_ui(v,v,1);
		if(mpz_cmp(v,p)==0)
		{
			r.resize(0);
			break;
		}
		UniEvalZp(bv,b,v,p);
		if(mpz_cmp_ui(bv,0)==0)continue;
		eval_in_last_zp(fv,_f,v,p);
		eval_in_last_zp(gv,_g,v,p);
		MultiGcdZp_Dense_Interpolation(hv,fv,gv,n-1,p);
		if(hv.size()==0)
		{
			r.resize(0);
			break;
		}
		m1=hv.deg(0);
		size=hv.size();
		if(m1>m)continue;
		//正则化
		mpz_set(inverse,hv[size-1].coe);
		mpz_invert(inverse,inverse,p);
		for(uint i=0;i<size;++i)
		{
			mpz_mul(hv[i].coe,hv[i].coe,inverse);
			mpz_mul(hv[i].coe,hv[i].coe,bv);
		}
		MultiPolynomialMod(hv,hv,p);
		if(m1<m)
		{
			//q=x_n-v
			q.resize(2);
			mpz_neg(q[0].coe,v);q[0].exponents.resize(n);
			for(uint i=0;i<n;++i)q[0].exponents[i]=0;
			mpz_set_ui(q[1].coe,1);q[1].exponents.resize(n);
			for(uint i=0;i<n;++i)q[1].exponents[i]=0;
			q[1].exponents[n-1]=1;
			q_one_v.resize(2);
			mpz_neg(q_one_v[0],v);mpz_set_ui(q_one_v[1],1);
			//h=hv
			h.resize(hv.size());
			for(uint i=0;i<hv.size();++i)
			{
				mpz_set(h[i].coe,hv[i].coe);
				h[i].exponents=hv[i].exponents;
				h[i].exponents.push_back(0);
			}
			m=m1;
			continue;
		}
		//modification of h
		eval_in_last_zp(h_temp,h,v,p);
		size=hv.size();
		for(uint i=0;i<size;++i)hv[i].exponents.push_back(0);
		size=h_temp.size();
		for(uint i=0;i<size;++i)h_temp[i].exponents.push_back(0);
		MultiSubZ(h_temp,hv,h_temp);
		MultiPolynomialMod(sparse_temp,h_temp,p);
		MultiMulZ(h_temp,sparse_temp,q);
		MultiPolynomialMod(h_temp,h_temp,p);
		UniEvalZp(inverse,q_one_v,v,p);
		mpz_invert(inverse,inverse,p);
		MultiMulZ(h_temp,h_temp,inverse);
		MultiPolynomialMod(h_temp,h_temp,p);
		MultiAddZ(h,h,h_temp);
		MultiPolynomialMod(h,h,p);
		//modification of q
		sparse_temp.resize(2);
		mpz_neg(sparse_temp[0].coe,v);sparse_temp[0].exponents.resize(n);
		for(uint i=0;i<n;++i)sparse_temp[0].exponents[i]=0;
		mpz_set_ui(sparse_temp[1].coe,1);sparse_temp[1].exponents.resize(n);
		for(uint i=0;i<n;++i)sparse_temp[1].exponents[i]=0;
		sparse_temp[1].exponents[n-1]=1;
		MultiMulZ(h_temp,q,sparse_temp);
		MultiPolynomialMod(q,h_temp,p);
		contf.resize(2);
		mpz_neg(contf[0],v);mpz_set_ui(contf[1],1);
		UniMulZ(contg,contf,q_one_v);
		UniPolynomialMod(q_one_v,contg,p);
		//criteria
		lc_in_last_zp(poly_temp,h,p);
		UniSubZ(poly_temp,poly_temp,b);
		UniPolynomialMod(poly_temp,poly_temp,p);
		if(m==0)
		{
			poly_z_to_sparse_z(r,a,n,n-1);
			return ;
		}
		if(poly_temp.size()==0)
		{
			pp_in_last_zp(h_temp,h,p);
			if(MultiDivisibleZp(_f,h_temp,p)&&MultiDivisibleZp(_g,h_temp,p))
			{
				poly_z_to_sparse_z(sparse_temp,a,n,n-1);
				MultiMulZ(r,sparse_temp,h_temp);
				MultiPolynomialMod(r,r,p);
				return ;
			}
		}
	}
	contf.resize(0);contg.resize(0);poly_temp.resize(0);a.resize(0);b.resize(0);
	_f.resize(0);_g.resize(0);sparse_temp.resize(0);
	q.resize(0);h.resize(0);fv.resize(0);gv.resize(0);hv.resize(0);
	q_one_v.resize(0);
	h_temp.resize(0);
	mpz_clear(v);mpz_clear(bv);mpz_clear(inverse);
	return ;
}

void MultiGcdZp(sparse_z & r,const sparse_z & f, const sparse_z & g,mpz_ptr p)
{
	if(f.size()==0)
	{
		copy_sparse_z(r,g);
		return ;
	}
	if(g.size()==0)
	{
		copy_sparse_z(r,f);
		return ;
	}
	uint n=f[0].exponents.size();
	MultiGcdZp_Dense_Interpolation(r,f,g,n,p);
}

}
