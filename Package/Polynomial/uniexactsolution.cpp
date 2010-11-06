/** \file
\brief 一元方程精确解
*/
#include "common.h"

namespace mU{

namespace
{

bool mpz_ptr_less(mpz_ptr a,mpz_ptr b)
{
	return mpz_cmp(a,b)<0;
}

void uni_z_root_z_separation(std::vector<mpz_ptr> & rootlist, mpz_ptr a, mpz_ptr b, const poly_z & f, uint change_a, uint change_b, std::vector<poly_z> & sturm_seq)
{
	//exit
	if(mpz_cmp(a,b)==0)return ;
	if(change_a<=change_b)
	{
		return ;
	}
	mpz_add_ui(a,a,1);
	if(mpz_cmp(a,b)==0)return ;
	mpz_sub_ui(a,a,1);
	//recursion
	mpz_t c;
	std::vector<mpz_ptr> ztemp;
	resize_z_list(ztemp,2);
	mpz_init(c);
	
	mpz_add(c,a,b);
	mpz_div_2exp(c,c,1);
	UniEvalZ(ztemp[0],f,c);
	if(mpz_size(ztemp[0])==0)
	{
		resize_z_list(rootlist,rootlist.size()+1);
		mpz_set(rootlist[rootlist.size()-1],c);

		mpz_sub_ui(ztemp[1],c,1);
		while(1)
		{
			UniEvalZ(ztemp[0],f,ztemp[1]);
			mpz_sub_ui(ztemp[1],ztemp[1],1);
			if(mpz_size(ztemp[0])==0)
			{
				resize_z_list(rootlist,rootlist.size()+1);
				mpz_add_ui(rootlist[rootlist.size()-1],ztemp[1],1);
			}
			else
			{
				break;
			}
			if(mpz_cmp(ztemp[1],a)==0)break;
		}
		if(mpz_cmp(ztemp[1],a)>0)
		{
			mpz_add_ui(ztemp[1],ztemp[1],1);
			uni_z_root_z_separation(rootlist,a,ztemp[1],f,change_a,UniSturmChangeSignZ(sturm_seq,ztemp[1]),sturm_seq);
		}

		mpz_add_ui(ztemp[1],c,1);
		while(1)
		{
			UniEvalZ(ztemp[0],f,ztemp[1]);
			mpz_add_ui(ztemp[1],ztemp[1],1);
			if(mpz_size(ztemp[0])==0)
			{
				resize_z_list(rootlist,rootlist.size()+1);
				mpz_sub_ui(rootlist[rootlist.size()-1],ztemp[1],1);
			}
			else
			{
				break;
			}
			if(mpz_cmp(ztemp[1],b)==0)break;
		}
		if(mpz_cmp(ztemp[1],b)<0)
		{
			mpz_sub_ui(ztemp[1],ztemp[1],1);
			uni_z_root_z_separation(rootlist,ztemp[1],b,f,UniSturmChangeSignZ(sturm_seq,ztemp[1]),change_b,sturm_seq);
		}
	}
	else
	{
		uint change_c=UniSturmChangeSignZ(sturm_seq,c);
		uni_z_root_z_separation(rootlist,a,c,f,change_a,change_c,sturm_seq);
		uni_z_root_z_separation(rootlist,c,b,f,change_c,change_b,sturm_seq);
	}
	mpz_clear(c);
	resize_z_list(ztemp,0);
	return ;
}

}

void UniZRootZ_ByFactor(std::vector<mpz_ptr> & rootlist,const poly_z & f)
{
	static mpz_t b,a;
	std::vector<poly_z> faclist;
	std::vector<uint> deglist;
	mpz_init(b);mpz_init(a);
	UniFacZ(f,b,faclist,deglist);
	uint totalroots=0;
	for(uint i=0;i<faclist.size();i++)
	{
		if(faclist[i].size()==2)
		{
			mpz_set(a,faclist[i][1]);
			mpz_neg(b,faclist[i][0]);
			if(mpz_divisible_p(b,a)!=0)
			{
				totalroots++;
				resize_z_list(rootlist,totalroots);
				mpz_divexact(rootlist[totalroots-1],b,a);
			}
		}
	}
	mpz_clear(b);mpz_clear(a);
	clear_poly_z_list(faclist);
	std::sort(rootlist.begin(),rootlist.end(),mpz_ptr_less);
	return ;
}

/**
\brief 整系数多项式的整数根(不含重数).
\param f 整系数多项式.
\return 整根的list.
\note 利用Zp中的根.
*/
void UniZRootZ_ByZp(std::vector<mpz_ptr> & rootlist,const poly_z & f)
{
	static mpz_t A,B,p,z_temp,nA;
	mpz_init(A);mpz_init(B);mpz_init(p);mpz_init(z_temp);mpz_init(nA);
	UniMaxNormZ(A,f);
	uint n=f.size()-1;
	mpz_mul(B,A,A);
	mpz_add(B,B,A);
	mpz_mul_ui(B,B,2*n);
	mpz_add_ui(p,B,1);
	mpz_nextprime(p,p);
	poly_z fp,vi,ui;
	std::vector<mpz_ptr> ulist;
	uint totalroot=0;
	UniPolynomialMod(fp,f,p);
	UniZpRootZp(ulist,fp,p);
	mpz_mul_ui(nA,A,n);
	ui.resize(2);
	mpz_set_ui(ui[1],1);
	for(uint i=0;i<ulist.size();i++)
	{
		if(mpz_cmpabs(ulist[i],A)<=0)
		{
			mpz_neg(ui[0],ulist[i]);
			UniDivZp(vi,fp,ui,p);
			UniMaxNormZ(z_temp,vi);
			if(mpz_cmp(z_temp,nA)<=0)
			{
				totalroot++;
				resize_z_list(rootlist,totalroot);
				mpz_set(rootlist[totalroot-1],ulist[i]);
			}
		}
	}
	mpz_clear(A);mpz_clear(B);mpz_clear(p);mpz_clear(z_temp);mpz_clear(nA);
	fp.resize(0);vi.resize(0);ui.resize(0);
	resize_z_list(ulist,0);
	std::sort(rootlist.begin(),rootlist.end(),mpz_ptr_less);
	return ;
}

/**
\brief 有限域Fp上多项式在Fp中的根(不含重数)
\param f Fp[x]中多项式
\param p 阶数
\return 根的集合
\note 利用因子分解算法求解
*/
void UniZpRootZp(std::vector<mpz_ptr> & rootlist,const poly_z & f,mpz_ptr p)
{
	resize_z_list(rootlist,0);
	poly_z h0,h;
	h0.resize(2);mpz_set_ui(h0[0],0);mpz_set_ui(h0[1],1);
	copy_poly_z(h,h0);

	PowerMod(h,p,f,p);

	mpz_sub_ui(h[1],h[1],1);
	UniGcdZp(h0,h,f,p);
	if(h0.size()==1)
	{
		h0.resize(0);h.resize(0);
		return ;
	}
	std::vector<poly_z> faclist;
	UniEqlDegFacZp(faclist,h0,p,1);
	for(uint i=0;i<faclist.size();i++)
	{
		resize_z_list(rootlist,i+1);
		mpz_neg(rootlist[i],faclist[i][0]);
	}
	clear_poly_z_list(faclist);
	h0.resize(0);h.resize(0);
	std::sort(rootlist.begin(),rootlist.end(),mpz_ptr_less);
	return ;
}

void UniZRootZ_BySeparation(std::vector<mpz_ptr> & rootlist,const poly_z & f)
{
	mpz_t bound,a,b;
	std::vector<poly_z> sturm_seq;
	mpz_init(bound);mpz_init(a);mpz_init(b);
	UniUpperBoundOfRootZ(bound,f);
	mpz_set(b,bound);mpz_neg(a,b);
	resize_z_list(rootlist,0);
	UniSturmSequenceZ(sturm_seq,f);

	uni_z_root_z_separation(rootlist,a,b,f,UniSturmChangeSignZ(sturm_seq,a),UniSturmChangeSignZ(sturm_seq,b),sturm_seq);
	
	std::sort(rootlist.begin(),rootlist.end(),mpz_ptr_less);
	clear_poly_z_list(sturm_seq);
	mpz_clear(bound);mpz_clear(a);mpz_clear(b);
	return ;
}

void UniZRootZ(std::vector<mpz_ptr> & rootlist, const poly_z & f)
{
	UniZRootZ_BySeparation(rootlist,f);
	return ;
}

}
