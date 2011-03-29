#include <mU/Kernel.h>
#include "common.h"
#include <fstream>

namespace mU {
//////////////////////////////////////
namespace {

var ca,cb,cc,cd,ce;
var solution1,solution2,solution3,solution4;
var expr_minus,expr_root_of_uint;

uint gcd_uint(uint a,uint b)
{
	uint u=a,v=b,r;
	while(v!=0)
	{
		r=u%v;
		u=v;v=r;
	}
	return u;
}

void root_of_unit(var & root, uint i, uint n, map_t & current)
{
	uint h=gcd_uint(i,n);
	uint ii=i/h,nn=n/h;
	ii=ii%nn;
	if(2*ii==nn)
	{
		root=Int();mpz_set_si(CInt(root),-1);
		return ;
	}
	if(ii==0)
	{
		root=Int();mpz_set_ui(CInt(root),1);
		return ;
	}
	if(2*ii<nn)
	{
		var a=Int();
		mpz_set_ui(CInt(a),1);
		current[ca]=a;
		a=Rat();
		mpq_set_ui(CRat(a),2*ii,nn);
		current[cb]=a;
		root=Eval(Subs(current,expr_root_of_uint));
		return ;
	}
	else
	{
		var a=Int();
		mpz_set_si(CInt(a),-1);
		current[ca]=a;
		a=Rat();
		mpq_set_ui(CRat(a),2*ii-nn,nn);
		current[cb]=a;
		root=Eval(Subs(current,expr_root_of_uint));
		return ;
	}
}

}

void UniSolveInitialize()
{
	var In=ParseFile(Path() + L"Package/Polynomial/UniSolve.u");
	ca=At(In,0);
	cb=At(In,1);
	cc=At(In,2);
	cd=At(In,3);
	ce=At(In,4);
	solution1=At(In,5);
	solution2=At(In,6);
	solution3=At(In,7);
	solution4=At(In,8);
	expr_minus=At(In,9);
	expr_root_of_uint=At(In,10);
	return ;
}

void UniSolve_Decomposition_Stage(var & rootlist, const poly_z & f)
{
	static mpq_t l;
	var a,b,c,d,e;
	var temp;
	var rootlist1,rootlist2;
	map_t new_e;
	mpq_init(l);
	std::vector<poly_z> partlist;
	rootlist=Vec();
	if(f.size()>5)
	{
		UniFuncionalDecompositionZ(l,partlist,f);
	}
	else
	{
		partlist.resize(1);
		copy_poly_z(partlist[0],f);
	}
	uint deg_deal,oldsize;
	poly_z & deal=partlist[0];
	deg_deal=deal.size()-1;

	//first cyclotomic-testing
	uint n;
	poly_z g;
	var root;
	if(deg_deal>3)n=UniShiftedCyclotomicZQ(g,deal);
	else n=0;
	rootlist=Vec();
	if(n>0)
	{
		for(uint i=0;i<n;++i)
		{
			if(gcd_uint(i,n)==1)
			{
				root_of_unit(root,i,n,new_e);
				if(mpz_cmp_ui(g[0],0)!=0||mpz_cmp_ui(g[1],1)!=0)
				{
					a=Int();
					mpz_set(CInt(a),g[0]);
					new_e[ca]=a;
					new_e[cb]=root;
					root=Eval(Subs(new_e,expr_minus));
					a=Int();
					mpz_set(CInt(a),g[1]);
					new_e[ca]=a;
					new_e[cb]=root;
					root=Eval(Subs(new_e,solution1));
				}
				Push(rootlist,root);
			}
		}
	}
	else
	{
		switch(deg_deal)
		{
		case 1:
			a=Int();b=Int();
			mpz_set(CInt(a),deal[1]);mpz_set(CInt(b),deal[0]);
			new_e[ca]=a;new_e[cb]=b;
			rootlist=Eval(Subs(new_e,solution1));
			break;
		case 2:
			a=Int();b=Int();c=Int();
			mpz_set(CInt(a),deal[2]);mpz_set(CInt(b),deal[1]);mpz_set(CInt(c),deal[0]);
			new_e[ca]=a;new_e[cb]=b;new_e[cc]=c;
			rootlist=Eval(Subs(new_e,solution2));
			break;
		case 3:
			a=Int();b=Int();c=Int();d=Int();
			mpz_set(CInt(a),deal[3]);mpz_set(CInt(b),deal[2]);mpz_set(CInt(c),deal[1]);mpz_set(CInt(d),deal[0]);
			new_e[ca]=a;new_e[cb]=b;new_e[cc]=c;new_e[cd]=d;
			rootlist=Eval(Subs(new_e,solution3));
			break;
		case 4:
			a=Int();b=Int();c=Int();d=Int();e=Int();
			mpz_set(CInt(a),deal[4]);mpz_set(CInt(b),deal[3]);mpz_set(CInt(c),deal[2]);mpz_set(CInt(d),deal[1]);mpz_set(CInt(e),deal[0]);
			new_e[ca]=a;new_e[cb]=b;new_e[cc]=c;new_e[cd]=d;new_e[ce]=e;
			rootlist=Eval(Subs(new_e,solution4));
			break;
		default:
			var fc;
			from_poly_z(fc,f);
			rootlist=Vec();
			for(uint i=1;i<f.size();++i)
			{
				Push(rootlist,Ex(tag_root,Vec(fc,Int(i))));
			}
			mpq_clear(l);
			return ;
			break;
		}
	}
	g.resize(0);
	for(uint i=1;i<partlist.size();++i)
	{
		oldsize=Size(rootlist);
		deal=partlist[i];
		rootlist1=Vec();
		for(uint j=0;j<oldsize;++j)
		{
			deg_deal=deal.size()-1;
			switch(deg_deal)
			{
			case 1:
				a=Int();b=Int();
				mpz_set(CInt(b),deal[0]);
				new_e[ca]=b;
				new_e[cb]=At(rootlist,j);
				temp=Eval(Subs(new_e,expr_minus));
				mpz_set(CInt(a),deal[1]);
				new_e[ca]=a;new_e[cb]=temp;
				rootlist2=Eval(Subs(new_e,solution1));
				break;
			case 2:
				a=Int();b=Int();c=Int();
				mpz_set(CInt(c),deal[0]);
				new_e[ca]=c;
				new_e[cb]=At(rootlist,j);
				temp=Eval(Subs(new_e,expr_minus));
				mpz_set(CInt(a),deal[2]);mpz_set(CInt(b),deal[1]);
				new_e[ca]=a;new_e[cb]=b;new_e[cc]=temp;
				rootlist2=Eval(Subs(new_e,solution2));
				break;
			case 3:
				a=Int();b=Int();c=Int();d=Int();
				mpz_set(CInt(d),deal[0]);
				new_e[ca]=d;
				new_e[cb]=At(rootlist,j);
				temp=Eval(Subs(new_e,expr_minus));
				mpz_set(CInt(a),deal[3]);mpz_set(CInt(b),deal[2]);mpz_set(CInt(c),deal[1]);
				new_e[ca]=a;new_e[cb]=b;new_e[cc]=c;new_e[cd]=temp;
				rootlist2=Eval(Subs(new_e,solution3));
				break;
			case 4:
				a=Int();b=Int();c=Int();d=Int();e=Int();
				mpz_set(CInt(e),deal[0]);
				new_e[ca]=e;
				new_e[cb]=At(rootlist,j);
				temp=Eval(Subs(new_e,expr_minus));
				mpz_set(CInt(a),deal[4]);mpz_set(CInt(b),deal[3]);mpz_set(CInt(c),deal[2]);mpz_set(CInt(d),deal[1]);
				new_e[ca]=a;new_e[cb]=b;new_e[cc]=c;new_e[cd]=d;new_e[ce]=temp;
				rootlist2=Eval(Subs(new_e,solution4));
				break;
			default:
				//x^n
				if(is_x_power_z(deal))
				{
					rootlist2=Vec();
					for(uint i=0;i<deg_deal;++i)
					{
						var root;
						root_of_unit(root,i,deg_deal,new_e);
						temp=Ex(TAG(Power),Vec(At(rootlist,j),Rat(1L,deg_deal)));
						root=Ex(TAG(Times),Vec(root,temp));
						Push(rootlist2,root);
					}
				}
				else
				{
					var fc;
					from_poly_z(fc,f);
					rootlist=Vec();
					for(uint i=1;i<f.size();++i)
					{
						Push(rootlist,Ex(tag_root,Vec(fc,Int(i))));
					}
					mpq_clear(l);
					return ;
				}
				break;
			}
			for(uint k=0;k<Size(rootlist2);++k)Push(rootlist1,At(rootlist2,k));
		}
		rootlist=rootlist1;
	}
	mpq_clear(l);
	return ;
}

void UniSolve_Factorization_Stage(var & rootlist, const poly_z & f)
{
	static mpz_t a;
	mpz_init(a);
	std::vector<poly_z> faclist;
	std::vector<uint> deglist;
	UniFacZ(f,a,faclist,deglist);
	var rootlist1;
	rootlist=Vec();
	uint size=faclist.size();
	for(uint i=0;i<size;++i)
	{
		UniSolve_Decomposition_Stage(rootlist1,faclist[i]);
		uint size1=Size(rootlist1);
		for(uint j=0;j<size1;++j)
		{
			for(uint k=0;k<deglist[i];++k)Push(rootlist,At(rootlist1,j));
		}
	}
	mpz_clear(a);
	clear_poly_z_list(faclist);
	return ;
}

//////////////////////////////////////
}
