/**
\file
\brief 多元多项式Groebner基
*/
#include "common.h"

namespace mU {

namespace{

order_q com_ptr;
uint n;
typedef std::pair<uint,uint> tuple2;
typedef std::set<tuple2> tuple2set;
var q_uint=Rat(1L,1L);

bool groebner_less(const sparse_q & g, const sparse_q & h)
{
	return com_ptr(g[g.size()-1],h[h.size()-1])<0;
}

void groebner_reduce(sparse_q & r, const sparse_q & f, const std::vector<sparse_q> polylist)
{
	sparse_q p,p_temp;
	mono_q t;
	uint size=polylist.size();
	uint rp,i;
	bool flag;
	std::vector<uint> rps(size);
	for(i=0;i<size;++i)rps[i]=polylist[i].size()-1;
	mpq_init(t.coe);
	t.exponents.resize(n);
	mpq_set_ui(t.coe,1,1);
	copy_sparse_q(p,f);
	r.resize(0);
	while(p.size()>0)
	{
		rp=p.size()-1;
		for(i=0;i<size;++i)
		{
			flag=1;
			for(uint j=0;j<n;++j)
			{
				if(p[rp].exponents[j]<polylist[i][rps[i]].exponents[j])
				{
					flag=0;break;
				}
				t.exponents[j]=p[rp].exponents[j]-polylist[i][rps[i]].exponents[j];
			}
			if(flag)break;
		}
		if(flag)
		{
			mpq_div(t.coe,p[rp].coe,polylist[i][rps[i]].coe);
			p_temp.resize(polylist[i].size());
			for(uint j=0;j<=rps[i];++j)mono_q_mul(p_temp[j],t,polylist[i][j]);
			MultiSubQ(p,p,p_temp);
		}
		else
		{
			r.resize(r.size()+1);
			mpq_set(r[r.size()-1].coe,p[rp].coe);
			r[r.size()-1].exponents=p[rp].exponents;
			p.resize(rp);
		}
	}
	mpq_clear(t.coe);
	p.resize(0);p_temp.resize(0);
	r.normalize(com_ptr);
	return ;
}

void groebner_reduce(std::vector<sparse_q> & polylist, uint index)
{
	sparse_q p,p_temp;
	mono_q t;
	uint size=polylist.size();
	uint rp,i;
	bool flag;
	std::vector<uint> rps(size);
	for(i=0;i<size;++i)rps[i]=polylist[i].size()-1;
	mpq_init(t.coe);
	t.exponents.resize(n);
	mpq_set_ui(t.coe,1,1);
	copy_sparse_q(p,polylist[index]);
	polylist[index].resize(0);
	while(p.size()>0)
	{
		rp=p.size()-1;
		for(i=0;i<size;++i)
		{
			flag=0;
			if(i==index)continue;
			flag=1;
			for(uint j=0;j<n;++j)
			{
				if(p[rp].exponents[j]<polylist[i][rps[i]].exponents[j])
				{
					flag=0;break;
				}
				t.exponents[j]=p[rp].exponents[j]-polylist[i][rps[i]].exponents[j];
			}
			if(flag)break;
		}
		if(flag)
		{
			mpq_div(t.coe,p[rp].coe,polylist[i][rps[i]].coe);
			p_temp.resize(polylist[i].size());
			for(uint j=0;j<=rps[i];++j)mono_q_mul(p_temp[j],t,polylist[i][j]);
			MultiSubQ(p,p,p_temp);
		}
		else
		{
			polylist[index].resize(polylist[index].size()+1);
			mpq_set(polylist[index][polylist[index].size()-1].coe,p[rp].coe);
			polylist[index][polylist[index].size()-1].exponents=p[rp].exponents;
			p.resize(rp);
		}
	}
	mpq_clear(t.coe);
	p.resize(0);p_temp.resize(0);
	polylist[index].normalize(com_ptr);
	return ;
}

void mono_q_lcm(mono_q & r, const mono_q & f, const mono_q & g)
{
	r.exponents.resize(n);
	for(uint i=0;i<n;++i)r.exponents[i]=std::max<uint>(f.exponents[i],g.exponents[i]);
}

void mono_q_div(mono_q & r, const mono_q & f, const mono_q & g)
{
	r.exponents.resize(n);
	for(uint i=0;i<n;++i)r.exponents[i]=f.exponents[i]-g.exponents[i];
	return ;
}

bool mono_q_divisible(const mono_q & f, const mono_q & g)
{
	for(uint i=0;i<n;++i)if(f.exponents[i]<g.exponents[i])return 0;
	return 1;
}

bool crit1(const mono_q & f, const mono_q & g)
{
	for(uint i=0;i<n;++i)
	{
		if(std::min<uint>(f.exponents[i],g.exponents[i])>0)return 1;
	}
	return 0;
}

bool for_crit2(const mono_q & h, const mono_q & f, const mono_q & g)
{
	for(uint i=0;i<n;++i)
	{
		if(h.exponents[i]>std::max<uint>(f.exponents[i],g.exponents[i]))return 0;
	}
	return 1;
}

void crit2(tuple2set & NC, tuple2set & C, uint s, sparse_q & lms)
{
	tuple2set::iterator index1,index2;
	uint l=0,i,j;
	while(l<s)
	{
		index1=std::find(NC.begin(),NC.end(),tuple2(l,s));
		if(index1!=NC.end())
		{
			i=0;
			while(i<s)
			{
				index1=std::find(NC.begin(),NC.end(),tuple2(i,l));
				index2=std::find(C.begin(),C.end(),tuple2(i,l));
				if(index1==NC.end()&&index2==C.end()){++i;continue;}
				index1=std::find(NC.begin(),NC.end(),tuple2(i,s));
				if(index1==NC.end()){++i;continue;}
				if(for_crit2(lms[l],lms[i],lms[s]))NC.erase(index1);
				++i;
			}
		}
		++l;
	}
	i=0;
	while(i<s)
	{
		index1=std::find(NC.begin(),NC.end(),tuple2(i,s));
		if(index1!=NC.end())
		{
			j=i+1;
			while(j<s)
			{
				index1=std::find(NC.begin(),NC.end(),tuple2(j,s));
				if(index1==NC.end()){++j;continue;}
				index1=std::find(NC.begin(),NC.end(),tuple2(i,j));
				if(index1==NC.end()){++j;continue;}
				if(for_crit2(lms[s],lms[i],lms[j]))NC.erase(index1);
				++j;
			}
		}
		++i;
	}
	return ;
}

void syl(sparse_q & r, const sparse_q & f, const sparse_q & g, const mono_q & lmf, const mono_q & lmg)
{
	sparse_q ff,gg;
	mono_q x,y;
	uint size;
	mpq_init(x.coe);mpq_init(y.coe);
	mono_q_lcm(x,lmf,lmg);
	mono_q_div(y,x,lmf);
	mpq_div(y.coe,CRat(q_uint),lmf.coe);
	size=f.size();
	ff.resize(size);
	for(uint i=0;i<size;++i)mono_q_mul(ff[i],y,f[i]);
	mono_q_div(y,x,lmg);
	mpq_div(y.coe,CRat(q_uint),lmg.coe);
	size=g.size();
	gg.resize(size);
	for(uint i=0;i<size;++i)mono_q_mul(gg[i],y,g[i]);
	MultiSubQ(r,ff,gg);
	mpq_clear(x.coe);mpq_clear(y.coe);
	ff.resize(0);gg.resize(0);
	return ;
}

void groebner_buchberger(std::vector<sparse_q> & gb, sparse_q & lms, const std::vector<sparse_q> & polylist)
{
	uint size=polylist.size();
	tuple2set C,NC;
	sparse_q r;
	clear_sparse_q_list(gb);
	gb.resize(size);
	lms.resize(size);
	for(uint i=0;i<size;++i)
	{
		copy_sparse_q(gb[i],polylist[i]);
		lms[i].exponents=gb[i][gb[i].size()-1].exponents;
		mpq_set(lms[i].coe,gb[i][gb[i].size()-1].coe);
	}
	NC.insert(tuple2(0,1));
	uint i=1,s=size-1;
	while(i<s)
	{
		for(uint j=0;j<=i;++j)NC.insert(tuple2(j,i+1));
		crit2(NC,C,i+1,lms);
		++i;
	}
	uint first,second;
	while(NC.size()>0)
	{
		first=(*NC.begin()).first;
		second=(*NC.begin()).second;
		NC.erase(NC.begin());
		C.insert(tuple2(first,second));
		if(crit1(lms[first],lms[second]))
		{
			syl(r,gb[first],gb[second],lms[first],lms[second]);
			groebner_reduce(r,r,gb);
			if(r.size()>0)
			{
				gb.push_back(r);r=sparse_q();
				++s;
				lms.resize(s+1);
				lms[s].exponents=gb[s][gb[s].size()-1].exponents;
				mpq_set(lms[s].coe,gb[s][gb[s].size()-1].coe);
				for(uint i=0;i<s;++i)NC.insert(tuple2(i,s));
				crit2(NC,C,s,lms);
			}
		}
	}
	r.resize(0);
	return ;
}

void minimal_groebner(std::vector<sparse_q> & gb, sparse_q & lms)
{
	for(uint i=0;i<gb.size();++i)
	{
		for(uint j=0;j<gb.size();++j)
		{
			if(i==j)continue;
			if(mono_q_divisible(lms[i],lms[j]))
			{
				gb[i].resize(0);
				gb.erase(gb.begin()+i);
				lms.erase(i);
				--i;
				break;
			}
		}
	}
	return ;
}

void reduced_groebner(std::vector<sparse_q> & gb)
{
	uint s=gb.size();
	for(uint i=0;i<s;++i)
	{
		groebner_reduce(gb,i);
	}
	return ;
}

}

void MultiGroebnerBasisQ(std::vector<sparse_q> & gb, const std::vector<sparse_q> & polylist, uint totalvar, order_q order)
{
	n=totalvar;
	com_ptr=order;
	sparse_q lms;
	mpq_t a;
	mpq_init(a);
	groebner_buchberger(gb,lms,polylist);
	minimal_groebner(gb,lms);
	reduced_groebner(gb);
	std::sort(gb.begin(),gb.end(),groebner_less);
	for(uint i=0;i<gb.size();++i)MultiSimpleFormQ(a,gb[i],gb[i]);
	lms.resize(0);
	mpq_clear(a);
	return ;
}

//Timing[Do[GroebnerBasis[{x^2+y^2+z^2-1,x+y+z,x^2-2 x+y^2-2 y+z^2+2 z}],{i,1,100}]]
//GroebnerBasis[{x^2+y^2+z^2-1,x+y+z,x^2-2 x+y^2-2 y+z^2+2 z}]
//GroebnerBasis[{x+y-a,x-y-b},{y,a,b,x}]

}
