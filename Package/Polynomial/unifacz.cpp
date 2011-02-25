/**
\file
\brief 整系数一元多项式因子分解，利用poly_z
*/
#include "common.h"

namespace mU {

typedef std::vector<int> int_vec;

namespace{

struct hensel_node
{
	bool end_node;
	poly_z data;
	poly_z assist;
	hensel_node * l;
	hensel_node * r;
};
typedef hensel_node * hensel_node_p;

hensel_node_p uni_generate_factor_tree_z(const std::vector<poly_z> & faclist,mpz_ptr p,uint a,uint b)
{
	hensel_node_p current;
	uint n=b-a;
	if(n==1)
	{
		current=new hensel_node;
		current->end_node=true;
		copy_poly_z(current->data,faclist[a]);
		return current;
	}
	uint n2=n/2;
	uint c=a+n2;
	hensel_node_p l,r;
	l=uni_generate_factor_tree_z(faclist,p,a,c);
	r=uni_generate_factor_tree_z(faclist,p,c,b);
	current=new hensel_node;
	current->end_node=false;
	current->l=l;
	current->r=r;
	poly_z temp,s,t;
	UniMulZ(current->data,l->data,r->data);
	UniGcdZp_Ext(temp,s,t,l->data,r->data,p);
	l->assist=s;s=poly_z();
	r->assist=t;t=poly_z();	
	temp.resize(0);s.resize(0);t.resize(0);
	return current;
}

//make sure the faclist mUst be empty initially
void uni_get_factor_list_z(std::vector<poly_z> & faclist,const hensel_node_p factree)
{
	if(factree->end_node)
	{
		faclist.resize(faclist.size()+1);
		copy_poly_z(faclist[faclist.size()-1],factree->data);
	}
	else
	{
		uni_get_factor_list_z(faclist,factree->l);
		uni_get_factor_list_z(faclist,factree->r);
	}
}

void uni_destroy_factor_tree_z(hensel_node_p factree)
{
	if(factree->end_node)
	{
		factree->data.resize(0);
		factree->assist.resize(0);
		delete factree;
	}
	else
	{
		factree->data.resize(0);
		factree->assist.resize(0);
		uni_destroy_factor_tree_z(factree->l);
		uni_destroy_factor_tree_z(factree->r);
		delete factree;
	}
}

void uni_hensel_step_z(const poly_z & f,mpz_ptr m,poly_z & g,poly_z & h, poly_z & s, poly_z & t)
{
	static mpz_t m2;
	mpz_init(m2);
	mpz_mul(m2,m,m);
	poly_z e,q,r,b,c,d,temp1,temp2;
	UniMulZ(temp1,g,h);
	UniSubZ(e,f,temp1);
	UniPolynomialMod(e,e,m2);
	UniMulZ(temp1,s,e);
	UniDivModZp(q,r,temp1,h,m2);
	UniAddZ(h,h,r);
	UniPolynomialMod(h,h,m2);

	UniMulZ(temp1,t,e);
	UniDivModZp(q,r,temp1,g,m2);
	UniAddZ(g,g,r);
	UniPolynomialMod(g,g,m2);

	UniMulZ(temp1,t,h);
	UniMulZ(temp2,s,g);
	UniAddZ(b,temp1,temp2);
	mpz_sub_ui(b[0],b[0],1);
	UniPolynomialMod(b,b,m2);
	UniMulZ(temp1,s,b);
	UniDivModZp(c,d,temp1,h,m2);
	UniSubZ(s,s,d);
	UniPolynomialMod(s,s,m2);

	UniMulZ(temp1,t,b);
	UniDivModZp(c,d,temp1,g,m2);
	UniSubZ(t,t,d);
	UniPolynomialMod(t,t,m2);

	mpz_clear(m2);
	e.resize(0);q.resize(0);r.resize(0);b.resize(0);c.resize(0);d.resize(0);temp1.resize(0);temp2.resize(0);
	return ;
}

void uni_lift_tree_step_z(hensel_node_p factree,mpz_ptr m)
{
	if(factree->end_node)return ;
	uni_hensel_step_z(factree->data,m,factree->l->data,factree->r->data,factree->l->assist,factree->r->assist);
	uni_lift_tree_step_z(factree->l,m);
	uni_lift_tree_step_z(factree->r,m);
}

// make sure that before lifting ,the root should be f itself, not monic f.
void uni_lift_tree_z(hensel_node_p factree,mpz_ptr a,mpz_ptr b,mpz_ptr m,mpz_ptr l) 
{
	static mpz_t d,m2j,m2jj,_a,temp1,temp2;
	mpz_init(d);
	mpz_set_ui(d,mpz_sizeinbase(l,2));
	mpz_init(m2j);
	mpz_set(m2j,m);
	mpz_init(m2jj);
	mpz_set(m2jj,m);
	mpz_init(_a);
	mpz_set(_a,a);
	mpz_init(temp1);mpz_init(temp2);
	poly_z f;
	copy_poly_z(f,factree->data);
	int d_int=mpz_get_ui(d);
	for(int j=1;j<=d_int;j++)
	{
		mpz_mul(m2j,m2jj,m2jj);
		mpz_mul_ui(temp1,_a,2);
		mpz_mul(temp2,_a,_a);
		mpz_mul(temp2,temp2,b);
		mpz_sub(_a,temp1,temp2);
		mpz_mod(_a,_a,m2j);
		for(size_t i=0;i<f.size();i++)
		{
			mpz_mul(factree->data[i],f[i],_a);
		}
		UniPolynomialMod(factree->data,factree->data,m2j);
		uni_lift_tree_step_z(factree,m2jj);
		mpz_set(m2jj,m2j);
	}
	f.resize(0);
	mpz_clear(d);
	mpz_clear(m2j);
	mpz_clear(m2jj);
	mpz_clear(_a);
	mpz_clear(temp1);mpz_clear(temp2);
	return ;
}

void delete_list(std::vector<poly_z> & x,const int_vec & indices)
{
	for(int i=0;i<indices.size();i++)
	{
		x[indices[i]-i].resize(0);
		x.erase(x.begin()+(indices[i]-i));
	}
	return ;
}

bool d_1_test(const std::vector<poly_z> & glist,const int_vec & indices,mpz_ptr m,mpz_ptr B,mpz_ptr b)
{
	static mpz_t m2;
	mpz_init(m2);
	mpz_div_ui(m2,m,2);
	const poly_z * f=&glist[indices[0]];
	mpz_t pretest;
	mpz_init_set_ui(pretest,0);
	for(size_t i=0;i<indices.size();i++)
	{
		f=&glist[indices[i]];
		mpz_add(pretest,pretest,(*f)[f->size()-2]);
	}
	mpz_mul(pretest,pretest,b);
	mpz_mod(pretest,pretest,m);
	if(mpz_cmp(pretest,m2)>0)
	{
		mpz_sub(pretest,pretest,m);
	}
	bool result=(mpz_cmpabs(pretest,B)<=0);
	mpz_clear(pretest);
	mpz_clear(m2);
	return result;
}

bool card_search_test(int_vec indices, int s, int r, std::vector<poly_z> & glist, std::vector<poly_z> & result, poly_z & fstar, poly_z & bpoly, mpz_ptr m, mpz_ptr B)
{
	if(indices.size()==s)
	{
		if(!d_1_test(glist,indices,m,B,bpoly[0]))return 0;
		poly_z trial;
		poly_z the_other,temp;
		UniMulZ(trial,bpoly,glist[indices[0]]);
		UniPolynomialMod(trial,trial,m);
		for(size_t i=1;i<indices.size();i++)
		{
			UniMulZ(temp,trial,glist[indices[i]]);
			trial.resize(0);trial=temp;temp=poly_z();
			UniPolynomialMod(trial,trial,m);
		}
		UniMulZ(temp,bpoly,fstar);
		UniDivModZp(the_other,temp,temp,trial,m);
		mpz_t temp1,temp2;
		mpz_init(temp1);mpz_init(temp2);
		UniOneNormZ(temp1,trial);
		UniOneNormZ(temp2,the_other);
		mpz_mul(temp1,temp1,temp2);
		if(mpz_cmp(temp1,B)<=0)
		{
			UniPPZ(fstar,the_other);
			UniPPZ(temp,trial);
			result.push_back(temp);temp=poly_z();
			bpoly.resize(1);mpz_set(bpoly[0],fstar[fstar.size()-1]);
			delete_list(glist,indices);
			the_other.resize(0);
			temp.resize(0);
			trial.resize(0);
			mpz_clear(temp1);mpz_clear(temp2);
			return 1;
		}
		else
		{
			the_other.resize(0);
			temp.resize(0);
			trial.resize(0);
			mpz_clear(temp1);mpz_clear(temp2);
			return 0;
		}
	}
	int size=indices.size();
	int low=size==0? 0:indices[size-1]+1;
	indices.push_back(0);
	for(int i=low;i<r;i++)
	{
		indices[size]=i;
		if(card_search_test(indices,s,r,glist,result,fstar,bpoly,m,B))
		{
			return 1;
		}
	}
	return 0;
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
//
/**
\brief 无平方分解.
\param f 整系数一元多项式.
\param sqflist 无平方因子序列
*/
void UniSqrFreeDecomZ(const poly_z & f,std::vector<poly_z> & sqflist)
{
	poly_z fd,u,v1,w1,v1d,h,temp;
	clear_poly_z_list(sqflist);
	poly_z_dform(fd,f);
	UniGcdZ(u,f,fd);
	UniDivExactZ(v1,f,u);
	UniDivExactZ(w1,fd,u);
	poly_z_dform(v1d,v1);
	while(1)
	{
		UniSubZ(temp,w1,v1d);
		UniGcdZ(h,v1,temp);
		sqflist.resize(sqflist.size()+1);
		copy_poly_z(sqflist[sqflist.size()-1],h);
		UniDivExactZ(temp,v1,h);
		copy_poly_z(v1,temp);
		UniSubZ(temp,w1,v1d);
		UniDivExactZ(w1,temp,h);
		poly_z_dform(v1d,v1);
		if(v1.size()==1&&mpz_cmp_si(v1[0],1)==0)break;
		//	h=UniGcdZ(v1,w1-v1d);
		//	result=list::cast(List::Join(result,list(1,h)));
		//	v1=v1/h;
		//	w1=(w1-v1d)/h;
		//	v1d=v1.dform();
		//	if(v1==onepoly)break;
	}
	fd.resize(0);u.resize(0);v1.resize(0);w1.resize(0);v1d.resize(0);h.resize(0);temp.resize(0);
	return;
}

/**
\brief 整系数多项式因子分解.
\param f 整系数一元多项式.
\return f的分解结果.
*/
void UniFacZ(const poly_z & f,mpz_ptr b,std::vector<poly_z> & faclist,std::vector<uint> & exponents)
{
	clear_poly_z_list(faclist);
	exponents.resize(0);
	if(f.size()==0)
	{
		mpz_set_ui(b,0);
		return ;
	}
	if(f.size()==1)
	{
		mpz_set(b,f[f.size()-1]);
		return ;
	}
	UniContZ(b,f);
	poly_z _f;
	_f.resize(f.size());
	for(int i=0;i<_f.size();i++)
	{
		mpz_div(_f[i],f[i],b);
	}
	std::vector<poly_z> sqflist(0),sqffaclist;
	UniSqrFreeDecomZ(_f,sqflist);
	for(int i=0;i<sqflist.size();i++)
	{
		if(sqflist[i].size()>1)
		{
			UniFacZ_Hensel_FactorCombination(sqflist[i],sqffaclist);
			for(int j=0;j<sqffaclist.size();j++)
			{
				faclist.push_back(sqffaclist[j]);
				sqffaclist[j]=poly_z();
				exponents.push_back(i+1);
			}
		}
	}
	clear_poly_z_list(sqflist);
	clear_poly_z_list(sqffaclist);
	return ;
}

/**
\brief Hensel提升因子组合算法.
\param f 待分解整系数无平方因子本原n次多项式,n>=1,lc(f)>=0.
\param faclist 各不相同的不可约因子的集合.
*/
void UniFacZ_Hensel_FactorCombination(const poly_z & f,std::vector<poly_z> & faclist)
{
	//clock_t t1,t2;

	clear_poly_z_list(faclist);
	
	//t1=clock();

	int n=f.size()-1;
	if(n==1)
	{
		faclist.resize(1);
		copy_poly_z(faclist[0],f);
		return ;
	}
	poly_z fstar,fp,fd,temp,bpoly;
	mpz_t A,b,B,p,p_temp;
	mpf_t float_temp;
	mpz_init(A);mpz_init(b);mpz_init(B);mpz_init(p);mpz_init(p_temp);
	mpf_init(float_temp);
	UniMaxNormZ(A,f);
	mpz_set(b,f[n]);
	int r;
	mpz_ui_pow_ui(B,2,n);
	mpz_mul(B,B,A);
	mpz_mul(B,B,b);
	mpf_sqrt_ui(float_temp,n+1);
	mpz_set_f(p_temp,float_temp);
	mpz_mul(B,B,p_temp);
	std::vector<poly_z> glist,glist_temp;
	int trial=0,r_temp=0;

	mpz_set_ui(p,2);
	mpz_t inverse;
	mpz_init(inverse);
	while(1)
	{
		mpz_nextprime(p,p);
		if(mpz_divisible_p(b,p)==0)
		{
			UniPolynomialMod(fp,f,p);
			poly_z_dform(fd,fp);
			UniPolynomialMod(fd,fd,p);
			UniGcdZp(temp,fp,fd,p);
			if(temp.size()<2)
			{
				mpz_invert(inverse,b,p);
				fp.resize(f.size());
				for(size_t i=0;i<f.size();i++)
				{
					mpz_mul(fp[i],f[i],inverse);
					mpz_mod(fp[i],fp[i],p);
				}
				BerlekampZp_SmallPrime(fp,p,glist);
				r=glist.size();
				if(r_temp==0||r<r_temp)
				{
					r_temp=r;
					clear_poly_z_list(glist_temp);
					glist_temp=glist;
					glist.resize(0);
					mpz_set(p_temp,p);
				}
				else trial++;
			}
		}
		if(trial>=2)
		{
			r=r_temp;
			clear_poly_z_list(glist);
			glist=glist_temp;
			glist_temp.resize(0);
			mpz_set(p,p_temp);
			break;
		}
	}
	mpz_clear(inverse);

	mpz_t l,a;
	mpz_init(l);mpz_init(a);
	mpz_mul_ui(l,B,2);
	mpz_add_ui(l,l,1);
	IntegerLength_BigBase(l,l,p);
	mpz_invert(a,b,p);
	
	//t2=clock();
	//std::cout<<t2-t1<<"\n";
	//t1=clock();

	hensel_node_p factree=uni_generate_factor_tree_z(glist,p,0,r);
	copy_poly_z(factree->data,f);
	uni_lift_tree_z(factree,a,b,p,l);
	clear_poly_z_list(glist);
	uni_get_factor_list_z(glist,factree);
	uni_destroy_factor_tree_z(factree);

	//t2=clock();
	//std::cout<<t2-t1<<"\n";
	//t1=clock();

	int s=1;
	mpz_t pl;
	mpz_init(pl);
	mpz_pow_ui(pl,p,mpz_get_ui(l));
	int_vec indices;
	bpoly.resize(1);mpz_set(bpoly[0],b);
	copy_poly_z(fstar,f);
	while(2*s<=r)
	{
		indices.resize(0);
		if(!card_search_test(indices,s,r,glist,faclist,fstar,bpoly,pl,B))
		{
			s++;
		}
		else
		{
			r=glist.size();
		}
	}
	faclist.push_back(fstar);fstar=poly_z();

	std::sort(faclist.begin(),faclist.end(),factor_compare);

	clear_poly_z_list(glist);
	clear_poly_z_list(glist_temp);
	fstar.resize(0);fp.resize(0);fd.resize(0);temp.resize(0);bpoly.resize(0);
	mpz_clear(A);mpz_clear(b);mpz_clear(B);mpz_clear(p);mpz_clear(p_temp);
	mpf_clear(float_temp);
	mpz_clear(l);mpz_clear(a);
	mpz_clear(pl);

	//t2=clock();
	//std::cout<<t2-t1<<"\n";
}

}
