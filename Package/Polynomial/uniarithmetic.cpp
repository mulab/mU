/** \file
\brief 一元多项式基本算术运算
\todo 有待修改补充,基本运算函数的目标和源是否可为同一地址的标注
*/
#include "common.h"
#include <algorithm>
#include <iostream>

namespace mU {

// basic arithmetics
void poly_z::resize(size_t n)
	{
		size_t m = this->size();
		if(n > m)
		{
			((std::vector<mpz_ptr>*)this)->resize(n);
			for(size_t i = m; i < n; ++i)
			{
				(*this)[i] = new __mpz_struct;
				mpz_init((*this)[i]);
			}
		}
		else if(n < m)
		{
			for(size_t i = n; i < m; ++i)
			{
				mpz_clear((*this)[i]);
				delete (*this)[i];
			}
			((std::vector<mpz_ptr>*)this)->resize(n);
		}
	}
int poly_z::deg() { return this->size() - 1; }
void poly_z::normalize()
	{
		uint n;
		const mpz_ptr* p;

		n = this->size();
		if (n == 0) return;
		p = &((*this)[0]) + n;
		while (n > 0 && mpz_size(*--p) == 0) {
			n--;
		}
		resize(n);
	}
mpz_ptr* poly_z::elts() { return &((*this)[0]); }
mpz_ptr const* poly_z::elts() const { return &((*this)[0]); }

void poly_z::print()
{
	if(this->size()==0)
	{
		std::cout<<"0\n";
		return ;
	}
	std::cout<<"(";
	for(size_t i=0;i<this->size();i++)
	{
		mpz_out_str(0,10,(*this)[i]);
		std::cout<<" ";
	}
	std::cout<<")\n";
	return ;
}

void poly_z::print() const
{
	if(this->size()==0)
	{
		std::cout<<"0\n";
		return ;
	}
	std::cout<<"(";
	for(size_t i=0;i<this->size();i++)
	{
		mpz_out_str(0,10,(*this)[i]);
		std::cout<<" ";
	}
	std::cout<<")\n";
	return ;
}

bool is_x_power_z(const poly_z & f)
{
	uint degf=f.size()-1;
	for(uint i=0;i<degf;++i)
	{
		if(mpz_cmp_ui(f[i],0)!=0)return 0;
	}
	return 1;
}

void copy_poly_z(poly_z & r,const poly_z & f)
{
	r.resize(f.size());
	for(size_t i=0;i<r.size();i++)
	{
		mpz_set(r[i],f[i]);
	}
}

void poly_z_dform(poly_z & r,const poly_z & f)
{
	if(f.size()<=1)
	{
		r.resize(0);
		return ;
	}
	r.resize(f.size()-1);
	static mpz_t temp;
	mpz_init(temp);
	for(size_t i=0;i<r.size();i++)
	{
		mpz_mul_si(temp,f[i+1],(uint)(i+1));
		mpz_set(r[i],temp);
	}
	r.normalize();
	mpz_clear(temp);
}

inline void PlainMul(mpz_ptr *xp, const mpz_ptr *ap, uint sa, const mpz_ptr *bp, uint sb)
{
	if (sa == 0 || sb == 0) return;

	uint sx = sa+sb-1;

	int i, j, jmin, jmax;
	static mpz_t t, accum;

	mpz_init(t); mpz_init(accum);
	for (i = 0; i < sx; i++) {
		jmin = std::max<int>(0, i-sb+1);
		jmax = std::min<int>(sa-1, i);
		mpz_set_ui(accum,0);
		for (j = jmin; j <= jmax; j++) {
			mpz_mul(t, ap[j], bp[i-j]);
			mpz_add(accum, accum, t);
		}
		mpz_set(xp[i], accum);
	}
	mpz_clear(t); mpz_clear(accum);
}

void UniNegZ(poly_z & r,const poly_z & f)
{
	r.resize(f.size());
	uint size=r.size();
	for(uint i=0;i<size;++i)
	{
		mpz_neg(r[i],f[i]);
	}
	return ;
}

void UniMulZ(poly_z & r,const poly_z & f,const poly_z & g)
{
	if(f.size()==0||g.size()==0)
	{
		r.resize(0);
		return ;
	}
	r.resize(f.size()+g.size()-1);
	PlainMul(r.elts(),f.elts(),f.size(),g.elts(),g.size());
}

void UniAddZ(poly_z & r,const poly_z & f,const poly_z & g)
{
	mpz_ptr * rp;
	const mpz_ptr * fp,* gp;
	int sf=f.size(),sg=g.size(),ms=std::min<int>(sf,sg);
	r.resize(std::max<int>(sf,sg));
	rp=r.elts();
	fp=f.elts();
	gp=g.elts();
	for(int i=0;i<ms;i++)mpz_add(rp[i],fp[i],gp[i]);
	if(sf<sg)
	{
		for(int i=ms;i<sg;i++)mpz_set(rp[i],gp[i]);
	}
	else
	{
		for(int i=ms;i<sf;i++)mpz_set(rp[i],fp[i]);
	}
	if(sf==sg)r.normalize();
	return ;
}

void UniSubZ(poly_z & r,const poly_z & f,const poly_z & g)
{
	mpz_ptr * rp;
	const mpz_ptr * fp,* gp;
	int sf=f.size(),sg=g.size(),ms=std::min<int>(sf,sg);
	r.resize(std::max<int>(sf,sg));
	rp=r.elts();
	fp=f.elts();
	gp=g.elts();
	for(int i=0;i<ms;i++)mpz_sub(rp[i],fp[i],gp[i]);
	if(sf<sg)
	{
		for(int i=ms;i<sg;i++)mpz_neg(rp[i],gp[i]);
	}
	else
	{
		for(int i=ms;i<sf;i++)mpz_set(rp[i],fp[i]);
	}
	if(sf==sg)r.normalize();
	return ;
}

void UniDivExactZ(poly_z & q,const poly_z & f,const poly_z & g)
{
	poly_z r;
	int k,n;
	r.resize(f.size());
	for(int i=0;i<f.size();i++)mpz_set(r[i],f[i]);
	n=g.size()-1;
	k=r.deg()-n;
	if(k<0)
	{
		q.resize(0);
		r.resize(0);
		return;
	}
	q.resize(k+1);
	do 
	{
		mpz_divexact(q[k],r[n+k],g[n]);
		if(mpz_cmp_ui(q[k],0)!=0)
		{
			for(int i=0;i<n;i++)
			{
				uint j=n+k-1-i;
				mpz_submul(r[j],q[k],g[j-k]);
			}
		}
	} while (k--);
	q.normalize();
	r.resize(0);
	return ;
}

void UniPseudoModZ(poly_z & r,const poly_z & f,const poly_z & g)
{
	int k,n;
	if(&r!=&f)
	{
		r.resize(f.size());
		for(int i=0;i<f.size();i++)mpz_set(r[i],f[i]);
	}
	n=g.size()-1;
	k=r.deg()-n;
	if(k<0)
	{
		return;
	}
	static mpz_t change,q;
	mpz_init(change);mpz_init(q);
	mpz_set(change,g[n]);
	mpz_pow_ui(change,change,k+1);
	uint size=r.size();
	for(uint i=0;i<size;++i)
	{
		mpz_mul(r[i],r[i],change);
	}
	do 
	{
		mpz_divexact(q,r[n+k],g[n]);
		if(mpz_cmp_ui(q,0)!=0)
		{
			for(int i=0;i<n;i++)
			{
				uint j=n+k-1-i;
				mpz_submul(r[j],q,g[j-k]);
			}
		}
	} while (k--);
	r.resize(n);
	r.normalize();
	mpz_abs(change,r[0]);
	for(uint i=1;i<r.size();++i)
	{
		mpz_gcd(change,change,r[i]);
		if(mpz_cmp_ui(change,1)==0)break;
	}
	if(mpz_cmp_ui(change,1)!=0)
	{
		for(uint i=0;i<r.size();++i)mpz_divexact(r[i],r[i],change);
	}
	mpz_clear(change);
	mpz_clear(q);
}

void UniEvalZ(mpz_ptr r,const poly_z & f,mpz_ptr x)
{
	uint i=f.size()-1;
	mpz_set(r,f[i]);
	if(i==0)return ;
	--i;
	while(1)
	{
		mpz_mul(r,r,x);
		mpz_add(r,r,f[i]);
		if(i==0)break;
		--i;
	}
	return ;
}

void UniEvalZp(mpz_ptr r,const poly_z & f,mpz_ptr x,mpz_ptr p)
{
	uint i=f.size()-1;
	mpz_set(r,f[i]);
	if(i==0)return ;
	--i;
	while(1)
	{
		mpz_mul(r,r,x);
		mpz_add(r,r,f[i]);
		mpz_mod(r,r,p);
		if(i==0)break;
		--i;
	}
	return ;
}

void UniPolynomialMod(poly_z & r,const poly_z & f,mpz_ptr m)
{
	if(f.size()==0)
	{
		r.resize(0);
		return ;
	}
	mpz_ptr * rp=r.elts();
	const mpz_ptr * fp=f.elts();
	int sf=f.size();
	r.resize(sf);
	static mpz_t t,m2;
	mpz_init(m2);
	mpz_div_ui(m2,m,2);
	mpz_init(t);
	for(int i=0;i<sf;i++)
	{
		mpz_mod(t,fp[i],m);
		if(mpz_cmp(t,m2)>0)
		{
			mpz_sub(r[i],t,m);
		}
		else
		{
			mpz_set(r[i],t);
		}
	}
	mpz_clear(t);
	mpz_clear(m2);
	r.normalize();
	return ;
}

void UniDivModZp(poly_z & q,poly_z & r,const poly_z & f,const poly_z & g,mpz_ptr p)
{
	int k,n;
	if(&r!=&f)
	{
		r.resize(f.size());
		for(int i=0;i<f.size();i++)mpz_set(r[i],f[i]);
	}
	n=g.size()-1;
	k=r.deg()-n;
	if(k<0)
	{
		q.resize(0);return;
	}
	q.resize(k+1);
	static mpz_t inverse;
	mpz_init(inverse);
	mpz_invert(inverse,g[n],p);
	do 
	{
		mpz_mul(q[k],r[n+k],inverse);
		mpz_mod(q[k],q[k],p);
		if(mpz_cmp_ui(q[k],0)!=0)
		{
			for(int i=0;i<n;i++)
			{
				uint j=n+k-1-i;
				mpz_submul(r[j],q[k],g[j-k]);
			}
		}
	} while (k--);
	UniPolynomialMod(q,q,p);
	r.resize(n);
	UniPolynomialMod(r,r,p);
	mpz_clear(inverse);
}

void UniDivZp(poly_z & q,const poly_z & f,const poly_z & g,mpz_ptr p)
{
	poly_z r;
	int k,n;

	r.resize(f.size());
	for(int i=0;i<f.size();i++)mpz_set(r[i],f[i]);

	n=g.size()-1;
	k=r.deg()-n;
	if(k<0)
	{
		q.resize(0);return;
	}
	q.resize(k+1);
	static mpz_t inverse;
	mpz_init(inverse);
	mpz_invert(inverse,g[n],p);
	do 
	{
		mpz_mul(q[k],r[n+k],inverse);
		mpz_mod(q[k],q[k],p);
		if(mpz_cmp_ui(q[k],0)!=0)
		{
			for(int i=0;i<n;i++)
			{
				uint j=n+k-1-i;
				mpz_submul(r[j],q[k],g[j-k]);
			}
		}
	} while (k--);
	UniPolynomialMod(q,q,p);
	r.resize(0);
	mpz_clear(inverse);
}

void UniModZp(poly_z & r,const poly_z & f,const poly_z & g,mpz_ptr p)
{
	int k,n;
	if(&r!=&f)
	{
		r.resize(f.size());
		for(int i=0;i<f.size();i++)mpz_set(r[i],f[i]);
	}
	n=g.size()-1;
	k=r.deg()-n;
	if(k<0)
	{
		return;
	}
	static mpz_t inverse,q;
	mpz_init(inverse);mpz_init(q);
	mpz_invert(inverse,g[n],p);
	do 
	{
		mpz_mul(q,r[n+k],inverse);
		mpz_mod(q,q,p);
		if(mpz_cmp_ui(q,0)!=0)
		{
			for(int i=0;i<n;i++)
			{
				uint j=n+k-1-i;
				mpz_submul(r[j],q,g[j-k]);
			}
		}
	} while (k--);
	r.resize(n);
	UniPolynomialMod(r,r,p);
	mpz_clear(inverse);
	mpz_clear(q);
}

bool poly_z_divisible(const poly_z & f,const poly_z & g)
{
	int k,n;
	poly_z r;
	copy_poly_z(r,f);

	n=g.size()-1;
	k=r.deg()-n;
	if(k<0)
	{
		bool result=(r.size()==0);
		r.resize(0);
		return result;
	}
	static mpz_t quo,rem;
	mpz_init(quo);mpz_init(rem);
	do 
	{
		mpz_fdiv_qr(quo,rem,r[n+k],g[n]);
		if(mpz_size(rem)!=0)
		{
			mpz_clear(quo);mpz_clear(rem);
			r.resize(0);
			return false;
		}
		if(mpz_cmp_ui(quo,0)!=0)
		{
			for(int i=0;i<n;i++)
			{
				uint j=n+k-1-i;
				mpz_submul(r[j],quo,g[j-k]);
			}
		}
	} while (k--);
	r.resize(n);
	r.normalize();
	mpz_clear(quo);mpz_clear(rem);
	bool result=(r.size()==0);
	r.resize(0);
	return result;
}

void resize_z_list(std::vector<mpz_ptr> & r,uint n)
{
	uint m=r.size();
	if(n>m)
	{
		r.resize(n);
		for(uint i=m;i<n;i++)
		{
			r[i]=new __mpz_struct;
			mpz_init(r[i]);
		}
	}
	else if(n<m)
	{
		for(uint i=n;i<m;i++)
		{
			mpz_clear(r[i]);
			delete r[i];
		}
		r.resize(n);
	}
	return ;
}

void clear_poly_z_list(std::vector<poly_z> & r)
{
	for(size_t i=0;i<r.size();i++)
	{
		r[i].resize(0);
	}
	r.resize(0);
}

void poly_q::resize(uint n)
{
	uint m=this->rep.size();
	if(n>m)
	{
		this->rep.resize(n);
		for(uint i=m;i<n;i++)
		{
			this->rep[i]=new __mpq_struct;
			mpq_init(this->rep[i]);
		}
	}
	else if(n<m)
	{
		for(uint i=n;i<m;i++)
		{
			mpq_clear(this->rep[i]);
			delete this->rep[i];
		}
		this->rep.resize(n);
	}
	return ;
}

void poly_q::normalize()
{
	uint i=this->rep.size();
	while(i>0&&mpq_cmp_ui(this->rep[i-1],0,1)==0)i--;
	this->resize(i);
	return ;
}

void poly_q::print()
{
	if(this->size()==0)
	{
		std::cout<<"0\n";
		return ;
	}
	std::cout<<"(";
	for(size_t i=0;i<this->size();i++)
	{
		mpq_out_str(0,10,this->rep[i]);
		std::cout<<" ";
	}
	std::cout<<")\n";
	return ;
}

void poly_q::print() const 
{
	if(this->size()==0)
	{
		std::cout<<"0\n";
		return ;
	}
	std::cout<<"(";
	for(size_t i=0;i<this->size();i++)
	{
		mpq_out_str(0,10,this->rep[i]);
		std::cout<<" ";
	}
	std::cout<<")\n";
	return ;
}

bool is_x_power_q(const poly_q & f)
{
	uint degf=f.size()-1;
	for(uint i=0;i<degf;++i)
	{
		if(mpq_cmp_ui(f[i],0,1)!=0)return 0;
	}
	return 1;
}

void x_power_q(poly_q & r,uint n)
{
	r.resize(n+1);
	for(uint i=0;i<n;++i)mpq_set_ui(r[i],0,1);
	mpq_set_ui(r[n],1,1);
}

void poly_z_to_poly_q(poly_q & r,const poly_z & f)
{
	static mpz_t unit;
	mpz_init(unit);
	mpz_set_ui(unit,1);
	r.resize(f.size());
	uint sf=f.size();
	for(uint i=0;i<sf;i++)
	{
		mpq_set_num(r[i],f[i]);
		mpq_set_den(r[i],unit);
	}
	mpz_clear(unit);
	return ;
}

void poly_q_to_poly_z(poly_z & r,const poly_q & f)
{
	r.resize(f.size());
	uint sf=f.size();
	for(uint i=0;i<sf;i++)
	{
		mpq_get_num(r[i],f[i]);
	}
	return ;
}

void poly_q_dform(poly_q & r,const poly_q & f)
{
	if(f.size()<=1)
	{
		r.resize(0);
		return ;
	}
	r.resize(f.size()-1);
	static mpq_t temp;
	mpq_init(temp);
	for(size_t i=0;i<r.size();i++)
	{
		mpq_set_ui(temp,(uint)(i+1),1);
		mpq_mul(r[i],temp,f[i+1]);
	}
	mpq_clear(temp);
	r.normalize();
}

void poly_q_simpleform(mpq_ptr a,poly_z & r,const poly_q & f)
{
	poly_q rq;
	poly_q_simpleform(a,rq,f);
	poly_q_to_poly_z(r,rq);
	rq.resize(0);
	return ;
}

void poly_q_simpleform(mpq_ptr a,poly_q & r,const poly_q & f)
{
	uint sf=f.size();
	if(sf==0){r.resize(0);return ;}
	static mpz_t unit,den,den_lcm;
	static mpq_t change;
	mpz_init(unit);mpz_init(den);mpz_init(den_lcm);
	mpq_init(change);
	mpz_set_ui(unit,1);
	if(&r!=&f)copy_poly_q(r,f);
	mpq_get_den(den_lcm,r[0]);
	for(uint i=1;i<sf;i++)
	{
		mpq_get_den(den,r[i]);
		mpz_lcm(den_lcm,den_lcm,den);
	}
	mpq_set_den(a,den_lcm);
	mpq_set_num(change,den_lcm);
	mpq_set_den(change,unit);
	for(uint i=0;i<sf;i++)
	{
		mpq_mul(r[i],r[i],change);
	}
	mpq_get_num(den_lcm,r[0]);
	for(uint i=1;i<sf;i++)
	{
		mpq_get_num(den,r[i]);
		mpz_gcd(den_lcm,den_lcm,den);
		if(mpz_cmp_ui(den_lcm,1)==0)break;
	}
	if(mpq_cmp_ui(r[sf-1],0,1)<0)mpz_neg(den_lcm,den_lcm);
	mpq_set_num(a,den_lcm);
	mpq_set_num(change,unit);
	mpq_set_den(change,den_lcm);
	mpq_canonicalize(change);
	for(uint i=0;i<sf;i++)
	{
		mpq_mul(r[i],r[i],change);
	}
	mpq_canonicalize(a);
	mpq_clear(change);
	mpz_clear(unit);mpz_clear(den);mpz_clear(den_lcm);
	return ;
}

void copy_poly_q(poly_q & r,const poly_q & f)
{
	r.resize(f.size());
	for(size_t i=0;i<r.size();i++)
	{
		mpq_set(r[i],f[i]);
	}
}

void clear_poly_q_list(std::vector<poly_q> & r)
{
	for(uint i=0;i<r.size();++i)r[i].resize(0);
	r.resize(0);
}

void UniNegQ(poly_q & r,const poly_q & f)
{
	r.resize(f.size());
	uint size=r.size();
	for(uint i=0;i<size;++i)
	{
		mpq_neg(r[i],f[i]);
	}
	return ;
}

void UniEvalQ(mpq_ptr r,const poly_q & f,mpq_ptr x)
{
	uint i=f.size()-1;
	mpq_set(r,f[i]);
	if(i==0)return ;
	--i;
	while(1)
	{
		mpq_mul(r,r,x);
		mpq_add(r,r,f[i]);
		if(i==0)break;
		--i;
	}
	return ;
}

void UniMulQ(poly_q & r, const poly_q & f, const poly_q & g)
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
	static mpq_t t, accum;
	mpq_init(t); mpq_init(accum);
	for (i = 0; i < sx; i++) {
		jmin = std::max<int>(0, i-sb+1);
		jmax = std::min<int>(sa-1, i);
		mpq_set_ui(accum,0,1);
		for (j = jmin; j <= jmax; j++) {
			mpq_mul(t, f[j], g[i-j]);
			mpq_add(accum, accum, t);
		}
		mpq_set(r[i], accum);
	}
	mpq_clear(t); mpq_clear(accum);
}

void UniDivModQ(poly_q & q,poly_q & r,const poly_q & f,const poly_q & g)
{
	int k,n;
	if(&r!=&f)
	{
		r.resize(f.size());
		for(int i=0;i<f.size();i++)mpq_set(r[i],f[i]);
	}
	n=g.size()-1;
	k=r.size()-g.size();
	if(k<0)
	{
		q.resize(0);return;
	}
	q.resize(k+1);
	mpq_t t;
	mpq_init(t);
	do 
	{
		mpq_div(q[k],r[n+k],g[n]);
		if(mpq_cmp_ui(q[k],0,1)!=0)
		{
			for(int i=0;i<n;i++)
			{
				uint j=n+k-1-i;
				mpq_mul(t,q[k],g[j-k]);
				mpq_sub(r[j],r[j],t);
			}
		}
	} while (k--);
	mpq_clear(t);
	r.resize(n);
	r.normalize();
}

void UniDivQ(poly_q & q,const poly_q & f,const poly_q & g)
{
	poly_q r;
	int k,n;
	r.resize(f.size());
	for(int i=0;i<f.size();i++)mpq_set(r[i],f[i]);
	n=g.size()-1;
	k=r.size()-g.size();
	if(k<0)
	{
		q.resize(0);return;
	}
	q.resize(k+1);
	mpq_t t;
	mpq_init(t);
	do 
	{
		mpq_div(q[k],r[n+k],g[n]);
		if(mpq_cmp_ui(q[k],0,1)!=0)
		{
			for(int i=0;i<n;i++)
			{
				uint j=n+k-1-i;
				mpq_mul(t,q[k],g[j-k]);
				mpq_sub(r[j],r[j],t);
			}
		}
	} while (k--);
	mpq_clear(t);
	r.resize(0);
}

void resize_q_list(std::vector<mpq_ptr> & r,uint n)
{
	uint m=r.size();
	if(n>m)
	{
		r.resize(n);
		for(uint i=m;i<n;i++)
		{
			r[i]=new __mpq_struct;
			mpq_init(r[i]);
		}
	}
	else if(n<m)
	{
		for(uint i=n;i<m;i++)
		{
			mpq_clear(r[i]);
			delete r[i];
		}
		r.resize(n);
	}
	return ;
}

}
