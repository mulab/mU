/**
\file
\brief 有关复数
*/
#include "polyfloat.h"

namespace mU {

namespace{

uint FC_DEFAULT_PREC=100;
inline uint DigitisToBits(uint prec){return (uint)(3.3219280948873623478703194294894 * prec);}

}

mpf_t mpfc_mpf_temp[4];//don't use this in other places except in the file complexbasic.cpp
mpfc_t mpfc_one,mpfc_zero;

void ComplexInitialize()
{
	FC_DEFAULT_PREC=100;
	mpf_init(mpfc_mpf_temp[0]);
	mpf_init(mpfc_mpf_temp[1]);
	mpf_init(mpfc_mpf_temp[2]);
	mpf_init(mpfc_mpf_temp[3]);
	mpfc_init(mpfc_one);mpfc_set_ui(mpfc_one,1);
	mpfc_init(mpfc_zero);mpfc_set_ui(mpfc_zero,0);
	return ;
}

void mpfc_init(mpfc_ptr x)
{
	mpf_init2(x->Im,(uint)(3.3219280948873623478703194294894 * FC_DEFAULT_PREC));
	mpf_init2(x->Re,(uint)(3.3219280948873623478703194294894 * FC_DEFAULT_PREC));
}

void mpfc_setdigits(mpfc_ptr x, uint digits)
{
	mpf_set_prec(x->Im,(uint)(3.3219280948873623478703194294894 * digits));
	mpf_set_prec(x->Re,(uint)(3.3219280948873623478703194294894 * digits));
}

uint mpfc_getdigits(mpfc_ptr x)
{
	return (uint)(mpf_get_prec(x->Im)/3.3219280948873623478703194294894);
}

void mpfc_clear(mpfc_ptr x)
{
	mpf_clear(x->Im);
	mpf_clear(x->Re);
}

void mpfc_out_str(FILE * stream,int base, size_t n_digits,mpfc_ptr x)
{
	std::cout<<"(";
	mpf_out_str(stream,base,n_digits,x->Re);
	std::cout<<",";
	mpf_out_str(stream,base,n_digits,x->Im);
	std::cout<<")";
}

void mpfc_neg(mpfc_ptr x,mpfc_ptr y)
{
	mpf_neg(x->Re,y->Re);
	mpf_neg(x->Im,y->Im);
}

void mpfc_set(mpfc_ptr x,mpfc_ptr y)
{
	mpf_set(x->Re,y->Re);
	mpf_set(x->Im,y->Im);
}

void mpfc_set_z(mpfc_ptr x,mpz_ptr y)
{
	mpf_set_z(x->Re,y);
	mpf_set_ui(x->Im,0);
}

void mpfc_set_ui(mpfc_ptr x, uint y)
{
	mpf_set_ui(x->Re,y);
	mpf_set_ui(x->Im,0);
}

void mpfc_set_c(mpfc_ptr x,uint a,uint b)
{
	mpf_set_si(x->Re,a);
	mpf_set_si(x->Im,b);
}

bool mpfc_iszero(mpfc_ptr x)
{
	return (mpf_cmp_ui(x->Im,0)==0)&&(mpf_cmp_ui(x->Re,0)==0);
}

void mpfc_add(mpfc_ptr r,mpfc_ptr x,mpfc_ptr y)
{
	mpf_add(r->Re,x->Re,y->Re);
	mpf_add(r->Im,x->Im,y->Im);
}

void mpfc_sub(mpfc_ptr r,mpfc_ptr x,mpfc_ptr y)
{
	mpf_sub(r->Re,x->Re,y->Re);
	mpf_sub(r->Im,x->Im,y->Im);
}

void mpfc_mul(mpfc_ptr r,mpfc_ptr x,mpfc_ptr y)
{
	mpf_mul(mpfc_mpf_temp[0],x->Re,y->Re);
	mpf_mul(mpfc_mpf_temp[1],x->Im,y->Im);
	mpf_sub(r->Re,mpfc_mpf_temp[0],mpfc_mpf_temp[1]);
	mpf_mul(mpfc_mpf_temp[0],x->Re,y->Im);
	mpf_mul(mpfc_mpf_temp[1],x->Im,y->Re);
	mpf_add(r->Im,mpfc_mpf_temp[0],mpfc_mpf_temp[1]);
}

void mpfc_mul_ui(mpfc_ptr r,mpfc_ptr x,uint a)
{
	mpf_mul_ui(r->Re,x->Re,a);
	mpf_mul_ui(r->Im,x->Im,a);
}

void mpfc_div(mpfc_ptr r,mpfc_ptr x,mpfc_ptr y)
{
	static mpfc_t z;
	mpf_mul(mpfc_mpf_temp[0],x->Re,y->Re);
	mpf_mul(mpfc_mpf_temp[1],x->Im,y->Im);
	mpf_add(mpfc_mpf_temp[2],mpfc_mpf_temp[0],mpfc_mpf_temp[1]);
	mpf_mul(mpfc_mpf_temp[0],x->Re,y->Im);
	mpf_mul(mpfc_mpf_temp[1],x->Im,y->Re);
	mpf_sub(mpfc_mpf_temp[3],mpfc_mpf_temp[1],mpfc_mpf_temp[0]);
	mpf_mul(mpfc_mpf_temp[0],x->Re,x->Re);
	mpf_mul(mpfc_mpf_temp[1],x->Im,x->Im);
	mpf_add(mpfc_mpf_temp[0],mpfc_mpf_temp[0],mpfc_mpf_temp[1]);
	mpf_div(r->Re,mpfc_mpf_temp[2],mpfc_mpf_temp[0]);
	mpf_div(r->Im,mpfc_mpf_temp[3],mpfc_mpf_temp[0]);
}

void mpfc_abs(mpf_t r,mpfc_ptr x)
{
	mpf_mul(mpfc_mpf_temp[0],x->Re,x->Re);
	mpf_mul(mpfc_mpf_temp[1],x->Im,x->Im);
	mpf_add(r,mpfc_mpf_temp[0],mpfc_mpf_temp[1]);
	mpf_sqrt(r,r);
}

void poly_fc::resize(uint n)
{
	uint m=this->rep.size();
	if(n>m)
	{
		this->rep.resize(n);
		for(uint i=m;i<n;i++)
		{
			this->rep[i]=new mpfc;
			mpfc_init(this->rep[i]);
		}
	}
	else if(n<m)
	{
		for(uint i=n;i<m;i++)
		{
			mpfc_clear(this->rep[i]);
			delete this->rep[i];
		}
		this->rep.resize(n);
	}
	return ;
}

void poly_fc::normalize()
{
	uint i=this->rep.size();
	while(i>0&&mpfc_iszero(this->rep[i-1]))i--;
	this->resize(i);
	return ;
}

void poly_fc::print()
{
	if(this->size()==0)
	{
		std::cout<<"0\n";
		return ;
	}
	std::cout<<"(";
	for(size_t i=0;i<this->size();i++)
	{
		mpfc_out_str(0,10,FC_DEFAULT_PREC,this->rep[i]);
		std::cout<<" ";
	}
	std::cout<<")\n";
	return ;
}

void poly_z_to_poly_fc(poly_fc & r, poly_z & f)
{
	r.resize(f.size());
	for(uint i=0;i<r.size();++i)
	{
		mpfc_set_z(r[i],f[i]);
	}
	return ;
}

void UniEvalFC(mpfc_ptr r,poly_fc & f,mpfc_ptr x)
{
	uint i=f.size()-1;
	mpfc_set(r,f[i]);
	if(i==0)return ;
	--i;
	while(1)
	{
		mpfc_mul(r,r,x);
		mpfc_add(r,r,f[i]);
		if(i==0)break;
		--i;
	}
	return ;
}

void UniDFormFC(poly_fc & r,poly_fc & f)
{
	if(f.size()<=1)
	{
		r.resize(0);
		return ;
	}
	r.resize(f.size()-1);
	for(size_t i=0;i<r.size();i++)
	{
		mpfc_mul_ui(r[i],f[i+1],i+1);
	}
	r.normalize();
}

void UniAddFC(poly_fc & r,poly_fc & f,poly_fc & g)
{
	int sf=f.size(),sg=g.size(),ms=std::min<int>(sf,sg);
	r.resize(std::max<int>(sf,sg));
	for(int i=0;i<ms;i++)mpfc_add(r[i],f[i],g[i]);
	if(sf<sg)
	{
		for(int i=ms;i<sg;i++)mpfc_set(r[i],g[i]);
	}
	else
	{
		for(int i=ms;i<sf;i++)mpfc_set(r[i],f[i]);
	}
	if(sf==sg)r.normalize();
	return ;
}

void UniSubFC(poly_fc & r,poly_fc & f,poly_fc & g)
{
	int sf=f.size(),sg=g.size(),ms=std::min<int>(sf,sg);
	r.resize(std::max<int>(sf,sg));
	for(int i=0;i<ms;i++)mpfc_sub(r[i],f[i],g[i]);
	if(sf<sg)
	{
		for(int i=ms;i<sg;i++)mpfc_neg(r[i],g[i]);
	}
	else
	{
		for(int i=ms;i<sf;i++)mpfc_set(r[i],f[i]);
	}
	if(sf==sg)r.normalize();
	return ;
}

void UniMulFC(poly_fc & r,mpfc_ptr a,poly_fc & f)
{
	r.resize(f.size());
	uint sf=f.size();
	for(uint i=0;i<sf;++i)
	{
		mpfc_mul(r[i],a,f[i]);
	}
	return ;
}

void UniMulFC(poly_fc & r,poly_fc & f,poly_fc & g)
{
	uint sa=f.size(),sb=g.size();
	if(sa==0||sb==0)
	{
		r.resize(0);
		return ;
	}
	uint sx = sa+sb-1;
	r.resize(sx);
	int i, j, jmin, jmax;
	static mpfc_t t, accum;
	mpfc_init(t); mpfc_init(accum);
	for (i = 0; i < sx; i++) {
		jmin = std::max<int>(0, i-sb+1);
		jmax = std::min<int>(sa-1, i);
		mpfc_set_ui(accum,0);
		for (j = jmin; j <= jmax; j++) {
			mpfc_mul(t, f[j], g[i-j]);
			mpfc_add(accum, accum, t);
		}
		mpfc_set(r[i], accum);
	}
	mpfc_clear(t); mpfc_clear(accum);
}

void UniDivFC(poly_fc & q,poly_fc & f,poly_fc & g)
{
	poly_fc r;
	int k,n;
	r.resize(f.size());
	for(int i=0;i<f.size();i++)mpfc_set(r[i],f[i]);
	n=g.size()-1;
	k=r.size()-g.size();
	if(k<0)
	{
		q.resize(0);return;
	}
	q.resize(k+1);
	mpfc_t t;
	mpfc_init(t);
	do 
	{
		mpfc_div(q[k],r[n+k],g[n]);
		if(!mpfc_iszero(q[k]))
		{
			for(int i=0;i<n;i++)
			{
				uint j=n+k-1-i;
				mpfc_mul(t,q[k],g[j-k]);
				mpfc_sub(r[j],r[j],t);
			}
		}
	} while (k--);
	mpfc_clear(t);
	r.resize(0);
}

void poly_f::resize(uint n)
{
	uint m=this->rep.size();
	if(n>m)
	{
		this->rep.resize(n);
		for(uint i=m;i<n;i++)
		{
			this->rep[i]=new __mpf_struct;
			mpf_init2(this->rep[i],DigitisToBits(FC_DEFAULT_PREC));
		}
	}
	else if(n<m)
	{
		for(uint i=n;i<m;i++)
		{
			mpf_clear(this->rep[i]);
			delete this->rep[i];
		}
		this->rep.resize(n);
	}
	return ;
}

void poly_f::normalize()
{
	uint i=this->rep.size();
	while(i>0&&mpf_cmp_ui(this->rep[i-1],0)==0)i--;
	this->resize(i);
	return ;
}

void poly_f::print()
{
	if(this->size()==0)
	{
		std::cout<<"0\n";
		return ;
	}
	std::cout<<"(";
	for(size_t i=0;i<this->size();i++)
	{
		mpf_out_str(0,10,FC_DEFAULT_PREC,this->rep[i]);
		std::cout<<" ";
	}
	std::cout<<")\n";
	return ;
}

void UniEvalF(mpf_ptr r, poly_f & f, mpf_ptr x)
{
	uint i=f.size()-1;
	mpf_set(r,f[i]);
	if(i==0)return ;
	--i;
	while(1)
	{
		mpf_mul(r,r,x);
		mpf_add(r,r,f[i]);
		if(i==0)break;
		--i;
	}
	return ;
}

void UniDFormF(poly_f & r, poly_f & f)
{
	if(f.size()<=1)
	{
		r.resize(0);
		return ;
	}
	r.resize(f.size()-1);
	for(size_t i=0;i<r.size();i++)
	{
		mpf_mul_ui(r[i],f[i+1],i+1);
	}
	r.normalize();
}

void UniRootF_Newton()
{
	poly_f f,fd;
	mpf_t x,y,den,num;
	mpf_t prec;
	mpf_init2(den,DigitisToBits(FC_DEFAULT_PREC));
	mpf_init2(num,DigitisToBits(FC_DEFAULT_PREC));
	mpf_init2(y,DigitisToBits(FC_DEFAULT_PREC));
	mpf_init2(x,DigitisToBits(FC_DEFAULT_PREC));
	mpf_init2(prec,1);
	f.resize(3);
	mpf_set_str(prec,"1e-50",10);
	mpf_set_si(f[0],-2);
	mpf_set_si(f[1],0);
	mpf_set_si(f[2],1);
	mpf_set_str(x,"1",10);

	UniDFormF(fd,f);

	while(1)
	{
		UniEvalF(num,f,x);
		UniEvalF(den,fd,x);
		mpf_div(y,num,den);
		mpf_abs(num,y);
		if(mpf_cmp(num,prec)<0)break;
		mpf_sub(x,x,y);
	}
	mpf_sub(y,x,y);
	mpf_out_str(0,10,FC_DEFAULT_PREC,y);std::cout<<"\n";
	mpf_clear(prec);
	mpf_clear(den);
	mpf_clear(num);
	mpf_clear(y);
	mpf_clear(x);
	f.resize(0);
	fd.resize(0);
}

}
