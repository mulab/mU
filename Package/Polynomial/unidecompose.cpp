/** \file
\brief 复合函数分解
*/
#include "common.h"

namespace mU{

namespace
{

void uni_functional_decomposition_q(poly_q & g,poly_q & h,const poly_q & f,uint r)// f monic, try f=g(h(x)), where \deg g=r.
{
	poly_q fw,hw,quo,rem;
	mpq_t a,b;
	mpq_init(a);mpq_init(b);
	uint n=f.size()-1;
	uint s=n/r;
	UniReversalQ(fw,f,n);
	mpq_set_ui(a,1,r);
	UniNthRootQ_Euler(hw,fw,a,s);
	UniReversalQ(h,hw,s);
	copy_poly_q(fw,f);
	g.resize(r+1);
	uint current=0;
	while(fw.size()>=h.size())
	{
		UniDivModQ(quo,rem,fw,h);
		if(rem.size()>1)
		{
			mpq_clear(a);mpq_clear(b);
			fw.resize(0);hw.resize(0);quo.resize(0);rem.resize(0);
			g.resize(0);h.resize(0);
			return ;
		}
		if(rem.size()==0)
		{
			mpq_set_ui(g[current++],0,1);
		}
		else
		{
			mpq_set(g[current++],rem[0]);
		}
		fw=quo;quo=poly_q();
	}
	if(quo.size()>1)
	{
		mpq_clear(a);mpq_clear(b);
		fw.resize(0);hw.resize(0);quo.resize(0);rem.resize(0);
		g.resize(0);h.resize(0);
		return ;
	}
	if(fw.size()==0)
	{
		mpq_set_ui(g[current++],0,1);
	}
	else
	{
		mpq_set(g[current++],fw[0]);
	}
	mpq_clear(a);mpq_clear(b);
	fw.resize(0);hw.resize(0);quo.resize(0);rem.resize(0);
	return ;
}

void uni_functional_decomposition_recursion_q(std::vector<poly_q> & partlist,const poly_q & f)//partlist should be empty before entering
{
	uint n=f.size()-1;
	uint n2=n/2;
	for(uint i=2;i<=n2;i++)
	{
		if(n%i==0)
		{
			poly_q g,h;
			uni_functional_decomposition_q(g,h,f,i);
			if(g.size()!=0&&h.size()!=0)
			{
				partlist.push_back(g);g=poly_q();
				uni_functional_decomposition_recursion_q(partlist,h);
				h.resize(0);
				return ;
			}
		}
	}
	partlist.resize(partlist.size()+1);
	copy_poly_q(partlist[partlist.size()-1],f);
	return ;
}

void nomarlize_partlist(std::vector<poly_q> & partlist)
{
	uint i=0,j=1,k,power;
	uint sp=partlist.size();
	while(1)
	{
		while(j<sp&&is_x_power_q(partlist[j]))++j;
		if(j-i>1)
		{
			uint case_of_first=0,size_of_first=partlist[i].size();
			if(mpq_cmp_ui(partlist[i][0],0,1)!=0)case_of_first+=1;
			if(mpq_cmp_ui(partlist[i][size_of_first-1],0,1)!=0)case_of_first+=2;
			for(uint l=1;l<partlist[i].size()-1;++l)
			{
				if(mpq_cmp_ui(partlist[i][l],0,1)!=0)case_of_first=0;
			}
			if(case_of_first)
			{
				power=size_of_first-1;
			}
			else
			{
				power=1;
			}
			for(k=i+1;k<j;++k)power*=partlist[k].size()-1;
			x_power_q(partlist[i+1],power);
			for(k=i+2;k<j;++k)
			{
				partlist[i+2].resize(0);
				partlist.erase(partlist.begin()+(i+2));
			}
			if(case_of_first==2)
			{
				mpq_set(partlist[i+1][partlist[i+1].size()-1],partlist[i][partlist[i].size()-1]);
				partlist[i].resize(0);
				partlist.erase(partlist.begin()+i);
				--i;
			}
			if(case_of_first==3)
			{
				mpq_set(partlist[i][1],partlist[i][partlist[i].size()-1]);
				partlist[i].resize(2);
			}
			j=i+2;
		}
		i=j;++j;sp=partlist.size();
		if(i==sp)break;
	}
	return ;
}

}

/**
\brief 有理系数多项式开方（求幂）算法.
\param g 有理系数多项式.
\param a 有理数,欲求的幂次.
\param r 
\note Euler算法.
*/
void UniNthRootQ_Euler(poly_q & r,const poly_q & g,mpq_ptr a,uint s)
{
	uint n=g.size()-1;
	if(s-1<n)n=s-1;
	static mpq_t q_temp1,q_temp2;
	mpq_init(q_temp1);mpq_init(q_temp2);
	r.resize(n+1);mpq_set_ui(r[0],1,1);
	for(uint i=1;i<=n;i++)
	{
		mpq_set_ui(r[i],0,1);
		mpq_set_si(q_temp1,-i,1);
		for(uint j=1;j<=i;j++)
		{
			mpq_add(q_temp1,q_temp1,a);
			mpq_add(q_temp1,q_temp1,r[0]);
			mpq_mul(q_temp2,q_temp1,g[j]);
			mpq_mul(q_temp2,q_temp2,r[i-j]);
			mpq_add(r[i],r[i],q_temp2);
		}
		mpq_set_ui(q_temp1,i,1);
		mpq_div(r[i],r[i],q_temp1);
	}
	r.normalize();
	mpq_clear(q_temp1);mpq_clear(q_temp2);
	return ;
}

/**
\brief 有理系数多项式的倒转.
\param r f的倒转
\param f 有理系数多项式.
\param n 长度>=s.deg
\note 专用于复合分解算法中.
*/
void UniReversalQ(poly_q & r,const poly_q & s,uint n)
{
	r.resize(n+1);
	uint bound=std::min<uint>(n,s.size()-1);
	for(uint i=0;i<=bound;i++)
	{
		mpq_set(r[n-i],s[i]);
	}
	for(uint i=bound+1;i<=n;i++)mpq_set_ui(r[n-i],0,1);
	r.normalize();
	return ;
}

/**
\brief 有理系数多项式的完全复合分解.
\param f 有理系数多项式.
\param partlist 形如\f$\{f_1,f_2,\ldots,f_k\}\f$的list,其中\f$f=f_1\circ f_2\circ\cdots\circ f_k\f$.
*/
void UniFuncionalDecompositionQ(std::vector<poly_q> & partlist,const poly_q & f)
{
	clear_poly_q_list(partlist);
	mpq_t a,change;
	uint n=f.size()-1;
	poly_q _f;
	mpq_init(a);mpq_init(change);
	mpq_set(a,f[n]);
	_f.resize(n+1);
	for(uint i=0;i<=n;++i)
	{
		mpq_div(_f[i],f[i],a);
	}
	uni_functional_decomposition_recursion_q(partlist,_f);
	for(uint i=0;i<partlist[0].size();++i)
	{
		mpq_mul(partlist[0][i],partlist[0][i],a);
	}
	for(uint i=partlist.size()-1;i>0;i--)
	{
		poly_q_simpleform(a,partlist[i],partlist[i]);
		mpq_set_ui(change,1,1);
		for(uint j=0;j<partlist[i].size();++j)mpq_mul(partlist[i][j],partlist[i][j],a);
		for(uint j=1;j<partlist[i-1].size();++j)
		{
			mpq_mul(change,change,a);
			mpq_div(partlist[i-1][j],partlist[i-1][j],change);
		}
	}
	nomarlize_partlist(partlist);
	_f.resize(0);
	mpq_clear(a);mpq_clear(change);
	return ;
}

/**
\brief 整系数多项式的完全复合分解.
\param f 整系数多项式.
\param partlist 形如\f$\{f_1,f_2,\ldots,f_k\}\f$的list,其中\f$f=f_1\circ f_2\circ\cdots\circ f_k\f$.
*/
void UniFuncionalDecompositionZ(mpq_ptr a,std::vector<poly_z> & partlist,const poly_z & f)
{
	poly_q fq;
	std::vector<poly_q> partlistq;
	poly_z_to_poly_q(fq,f);
	UniFuncionalDecompositionQ(partlistq,fq);
	partlist.resize(partlistq.size());
	uint psize=partlistq.size();
	for(uint i=1;i<psize;++i)
	{
		poly_q_to_poly_z(partlist[i],partlistq[i]);
	}
	poly_q_simpleform(a,partlist[0],partlistq[0]);
	clear_poly_q_list(partlistq);
	fq.resize(0);
	return ;
}

}
