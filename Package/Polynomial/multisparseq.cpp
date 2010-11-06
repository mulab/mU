/**
\file
\brief 多元多项式基本函数 in Q[x]
*/
#include "common.h"

namespace mU {

namespace{

int (__cdecl *com_ptr)(const mono_q & f,const mono_q & g);

bool com_for_sort(const mono_q & f,const mono_q & g)
{
	return (com_ptr(f,g)<0);
}

}

void sparse_q::normalize(order_q order)
{
	while(1)
	{
		bool flag=0;
		for(size_t i=0;i<this->monomials.size();i++)
		{
			if(mpq_cmp_ui(this->monomials[i].coe,0,1)==0)
			{
				mpq_clear(this->monomials[i].coe);
				this->monomials.erase(this->monomials.begin()+i);
				flag=1;
				break;
			}
		}
		if(!flag)break;
	}
	com_ptr=order;
	std::sort(this->monomials.begin(),this->monomials.end(),com_for_sort);
	return ;
}

uint sparse_q::size()
{
	return this->monomials.size();
}

uint sparse_q::size() const
{
	return this->monomials.size();
}

void sparse_q::resize(size_t n)
{
	size_t m=this->monomials.size();
	if(n>m)
	{
		this->monomials.resize(n);
		for(size_t i=m;i<n;i++)
		{
			mpq_init(this->monomials[i].coe);
		}
	}
	else if(n<m)
	{
		for(size_t i=n;i<m;i++)
		{
			mpq_clear(this->monomials[i].coe);
		}
		this->monomials.resize(n);
	}
	return ;
}

void sparse_q::print()
{
	std::cout<<" ( ";
	for(size_t i=0;i<this->monomials.size();i++)
	{
		mpq_out_str(0,10,this->monomials[i].coe);
		std::cout<<"<";
		for(size_t j=0;j<this->monomials[i].exponents.size();j++)
		{
			std::cout<<" "<<this->monomials[i].exponents[j]<<" ";
		}
		std::cout<<">";
		if(i!=this->monomials.size()-1)std::cout<<", ";
	}
	std::cout<<" ) \n";
}

void sparse_q::print() const
{
	std::cout<<" ( ";
	for(size_t i=0;i<this->monomials.size();i++)
	{
		mpq_out_str(0,10,this->monomials[i].coe);
		std::cout<<"<";
		for(size_t j=0;j<this->monomials[i].exponents.size();j++)
		{
			std::cout<<" "<<this->monomials[i].exponents[j]<<" ";
		}
		std::cout<<">";
		if(i!=this->monomials.size()-1)std::cout<<", ";
	}
	std::cout<<" ) \n";
}

void sparse_q::combine_ordered()
{
	size_t i,j,current,sr;
	sr=this->size();
	i=0;j=0;current=0;
	uint n=this->monomials[0].exponents.size();
	while(1)
	{
		while(j<sr&&equal_mono_q_type((*this)[j],(*this)[i],n))j++;
		if(j-i>1)
		{
			for(current=i+1;current<j;current++)
			{
				mpq_add((*this)[i].coe,(*this)[i].coe,(*this)[current].coe);
			}
			for(current=i+1;current<j;current++)
			{
				mpq_clear((*this)[i+1].coe);
				this->monomials.erase(this->monomials.begin()+(i+1));
			}
			sr=this->size();
		}
		i++;j=i;sr=this->size();
		if(i==sr)break;
	}
}

uint sparse_q::deg(uint variable)
{
	uint deg_temp=0;
	for(size_t i=0;i<this->size();i++)
	{
		if(deg_temp<this->monomials[i].exponents[variable])deg_temp=this->monomials[i].exponents[variable];
	}
	return deg_temp;
}

uint sparse_q::deg(uint variable) const
{
	uint deg_temp=0;
	for(size_t i=0;i<this->size();i++)
	{
		if(deg_temp<this->monomials[i].exponents[variable])deg_temp=this->monomials[i].exponents[variable];
	}
	return deg_temp;
}

void sparse_q::erase(uint i)
{
	mpq_clear(this->monomials[i].coe);
	this->monomials.erase(this->monomials.begin()+i);
	return ;
}

void copy_sparse_q(sparse_q & r,const sparse_q & s)
{
	r.resize(s.size());
	for(size_t i=0;i<r.size();i++)
	{
		r[i].exponents=s[i].exponents;
		mpq_set(r[i].coe,s[i].coe);
	}
	return ;
}

void clear_sparse_q_list(std::vector<sparse_q> & x)
{
	for(size_t i=0;i<x.size();i++)
	{
		x[i].resize(0);
	}
	x.resize(0);
}

int lex_q(const mono_q & f,const mono_q & g)
{
	for(size_t i=0;i<f.exponents.size();i++)
	{
		if(f.exponents[i]<g.exponents[i])return -1;
		if(f.exponents[i]>g.exponents[i])return 1;
	}
	return 0;
}

bool equal_mono_q_type(const mono_q & f,const mono_q & g,uint n_bound)
{
	for(uint i=0;i<n_bound;i++)
	{
		if(f.exponents[i]!=g.exponents[i])return 0;
	}
	return 1;
}

//make sure f & g should be normalized before being added.
void MultiAddQ(sparse_q & r,const sparse_q & f,const sparse_q & g,order_q order)
{
	size_t sf,sg,i,j;
	sparse_q rr;
	sf=f.size();sg=g.size();
	rr.resize(0);
	i=0;j=0;
	while(1)
	{
		if(i==sf)
		{
			for(size_t k=j;k<sg;k++)
			{
				rr.resize(rr.size()+1);
				rr.right().exponents=g[k].exponents;
				mpq_set(rr.right().coe,g[k].coe);
			}
			break;
		}
		if(j==sg)
		{
			for(size_t k=i;k<sf;k++)
			{
				rr.resize(rr.size()+1);
				rr.right().exponents=f[k].exponents;
				mpq_set(rr.right().coe,f[k].coe);
			}
			break;
		}
		int flag=order(f[i],g[j]);
		if(flag<0)
		{
			rr.resize(rr.size()+1);
			rr.right().exponents=f[i].exponents;
			mpq_set(rr.right().coe,f[i].coe);
			i++;
			continue;
		}
		if(flag>0)
		{
			rr.resize(rr.size()+1);
			rr.right().exponents=g[j].exponents;
			mpq_set(rr.right().coe,g[j].coe);
			j++;
			continue;
		}
		if(flag==0)
		{
			rr.resize(rr.size()+1);
			mpq_add(rr.right().coe,f[i].coe,g[j].coe);
			rr.right().exponents=f[i].exponents;
			i++;j++;
			continue;
		}
	}
	rr.normalize(order);
	r.resize(0);
	r=rr;rr=sparse_q();
	return ;
}

//make sure f & g should be normalized before being substracted.
void MultiSubQ(sparse_q & r,const sparse_q & f,const sparse_q & g,order_q order)
{
	size_t sf,sg,i,j;
	sparse_q rr;
	sf=f.size();sg=g.size();
	rr.resize(0);
	i=0;j=0;
	while(1)
	{
		if(i==sf)
		{
			for(size_t k=j;k<sg;k++)
			{
				rr.resize(rr.size()+1);
				rr.right().exponents=g[k].exponents;
				mpq_neg(rr.right().coe,g[k].coe);
			}
			break;
		}
		if(j==sg)
		{
			for(size_t k=i;k<sf;k++)
			{
				rr.resize(rr.size()+1);
				rr.right().exponents=f[k].exponents;
				mpq_set(rr.right().coe,f[k].coe);
			}
			break;
		}
		int flag=order(f[i],g[j]);
		if(flag<0)
		{
			rr.resize(rr.size()+1);
			rr.right().exponents=f[i].exponents;
			mpq_set(rr.right().coe,f[i].coe);
			i++;
			continue;
		}
		if(flag>0)
		{
			rr.resize(rr.size()+1);
			rr.right().exponents=g[j].exponents;
			mpq_neg(rr.right().coe,g[j].coe);
			j++;
			continue;
		}
		if(flag==0)
		{
			rr.resize(rr.size()+1);
			mpq_sub(rr.right().coe,f[i].coe,g[j].coe);
			rr.right().exponents=f[i].exponents;
			i++;j++;
			continue;
		}
	}
	rr.normalize(order);
	r.resize(0);
	r=rr;rr=sparse_q();
	return ;
}

void mono_q_mul(mono_q & r,const mono_q & f,const mono_q & g)
{
	mpq_mul(r.coe,f.coe,g.coe);
	size_t sf=f.exponents.size();
	r.exponents.resize(sf);
	for(size_t i=0;i<sf;i++)
	{
		r.exponents[i]=f.exponents[i]+g.exponents[i];
	}
	return ;
}

//make sure f & g should be normalized before being multiplied.
void MultiMulQ(sparse_q & r,const sparse_q & f,const sparse_q & g,order_q order)
{
	size_t sf,sg,i,j,current;
	sparse_q rr;
	sf=f.size();sg=g.size();
	rr.resize(sf*sg);
	if(sf==0||sg==0)
	{
		r.resize(0);return ;
	}
	current=0;
	for(i=0;i<sf;i++)
	{
		for(j=0;j<sg;j++)
		{
			mono_q_mul(rr[current],f[i],g[j]);
			current++;
		}
	}
	rr.normalize(order);
	rr.combine_ordered();

	r.resize(0);
	r=rr;rr=sparse_q();
	return ;
}

void MultiMulQ(sparse_q & r,const sparse_q & f,mpq_ptr a,order_q order)
{
	uint sf=f.size();
	copy_sparse_q(r,f);
	for(uint i=0;i<sf;++i)
	{
		mpq_mul(r[i].coe,f[i].coe,a);
	}
	r.normalize();
}

//make sure f & g should be normalized before being multiplied.
void MultiDivQ(sparse_q & r,const sparse_q & f,const sparse_q & g,order_q order)
{
	if(f.size()==0)
	{
		r.resize(0);return ;
	}
	sparse_q rr,rem,t;
	copy_sparse_q(rem,f);
	rr.resize(0);
	uint n=g.size()-1,vars=g[0].exponents.size();
	while(1)
	{
		rr.resize(rr.size()+1);
		mpq_div(rr[rr.size()-1].coe,rem[rem.size()-1].coe,g[n].coe);
		rr[rr.size()-1].exponents.resize(vars);
		for(size_t i=0;i<vars;i++)rr[rr.size()-1].exponents[i]=rem[rem.size()-1].exponents[i]-g[n].exponents[i];
		t.resize(n+1);
		for(size_t i=0;i<=n;i++)mono_q_mul(t[i],rr[rr.size()-1],g[i]);
		MultiSubQ(rem,rem,t,order);
		if(rem.size()==0)break;
	}
	r.resize(0);r=rr;rr=sparse_q();
	rem.resize(0);t.resize(0);
	return ;
}

bool MultiDivisibleQ(const sparse_q & f,const sparse_q & g,order_q order)
{
	if(f.size()==0)
	{
		return 1;
	}
	sparse_q rr,rem,t;
	copy_sparse_q(rem,f);
	uint n=g.size()-1,vars=g[0].exponents.size();
	uint size;
	rr.resize(1);
	rr[0].exponents.resize(vars);
	while(1)
	{
		size=rem.size()-1;
		for(size_t i=0;i<vars;i++)
		{
			if(rem[size].exponents[i]<g[n].exponents[i])
			{
				rr.resize(0);rem.resize(0);t.resize(0);
				return 0;
			}
			rr[0].exponents[i]=rem[size].exponents[i]-g[n].exponents[i];
		}
		mpq_div(rr[0].coe,rem[size].coe,g[n].coe);
		t.resize(n+1);
		for(size_t i=0;i<=n;i++)mono_q_mul(t[i],rr[0],g[i]);
		MultiSubQ(rem,rem,t,order);
		if(rem.size()==0)break;
	}
	rr.resize(0);rem.resize(0);t.resize(0);
	return 1;
}

void MultiSimpleFormQ(mpq_ptr a, sparse_q & r, const sparse_q & f)
{
	uint sf=f.size();
	if(sf==0){r.resize(0);return ;}
	static mpz_t unit,den,den_lcm;
	static mpq_t change;
	mpz_init(unit);mpz_init(den);mpz_init(den_lcm);
	mpq_init(change);
	mpz_set_ui(unit,1);
	if(&r!=&f)copy_sparse_q(r,f);
	mpq_get_den(den_lcm,r[0].coe);
	for(uint i=1;i<sf;i++)
	{
		mpq_get_den(den,r[i].coe);
		mpz_lcm(den_lcm,den_lcm,den);
	}
	mpq_set_den(a,den_lcm);
	mpq_set_num(change,den_lcm);
	mpq_set_den(change,unit);
	for(uint i=0;i<sf;i++)
	{
		mpq_mul(r[i].coe,r[i].coe,change);
	}
	mpq_get_num(den_lcm,r[0].coe);
	for(uint i=1;i<sf;i++)
	{
		mpq_get_num(den,r[i].coe);
		mpz_gcd(den_lcm,den_lcm,den);
		if(mpz_cmp_ui(den_lcm,1)==0)break;
	}
	if(mpq_cmp_ui(r[sf-1].coe,0,1)<0)mpz_neg(den_lcm,den_lcm);
	mpq_set_num(a,den_lcm);
	mpq_set_num(change,unit);
	mpq_set_den(change,den_lcm);
	mpq_canonicalize(change);
	for(uint i=0;i<sf;i++)
	{
		mpq_mul(r[i].coe,r[i].coe,change);
	}
	mpq_canonicalize(a);
	mpq_clear(change);
	mpz_clear(unit);mpz_clear(den);mpz_clear(den_lcm);
	return ;
}

void to_main_variable(std::vector<sparse_q> & f,const sparse_q & g,uint variable)
{
	uint degx,size;
	clear_sparse_q_list(f);
	f.resize(g.deg(variable)+1);
	for(size_t i=0;i<g.size();i++)
	{
		degx=g[i].exponents[variable];
		size=f[degx].size();
		f[degx].resize(size+1);
		mpq_set(f[degx][size].coe,g[i].coe);
		f[degx][size].exponents=g[i].exponents;
		f[degx][size].exponents.erase(f[degx][size].exponents.begin()+variable);
	}
	for(size_t i=0;i<f.size();i++)f[i].normalize();
	return ;
}

void from_main_variable(sparse_q & f,const std::vector<sparse_q> & g,uint variable)
{
	f.resize(0);
	uint size;
	for(uint i=0;i<g.size();i++)
	{
		for(uint j=0;j<g[i].size();j++)
		{
			size=f.size();
			f.resize(size+1);
			mpq_set(f[size].coe,g[i][j].coe);
			f[size].exponents=g[i][j].exponents;
			f[size].exponents.insert(f[size].exponents.begin()+variable,1,i);
		}
	}
	f.normalize();
	return ;
}

}
