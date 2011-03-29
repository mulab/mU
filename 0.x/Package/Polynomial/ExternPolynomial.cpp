#include <mU/Function.h>
#include "common.h"
#include "polyfloat.h"
//#pragma comment(lib,"var")

namespace mU {
//////////////////////////////////////

mpz_t z_unit,z_zero;
mpq_t q_unit,q_zero;
var tag_modulus,tag_cyclotomic,tag_root;

namespace {

Wrap(Test)
{
	var f_var=At(x,0);
	var v_var=Variables(f_var);
	v_var=At(v_var,0);
	f_var=Coefficients(Expand(f_var),v_var);
	poly_z f;
	poly_fc roots;
	to_poly_z(f,f_var);
	var n_var=At(x,1);
	uint digits=mpz_get_ui(CInt(n_var));

	UniFCRootZ_Jenkins_Traub(roots,f,digits);
	roots.print();

	f.resize(0);
	roots.resize(0);
	return 0;
}

Wrap(RootIntervals)
{
	//rootintervals
	var f_var=At(x,0);
	var v_var=Variables(f_var);
	var result_var=Vec();
	v_var=At(v_var,0);
	f_var=Coefficients(Expand(f_var),v_var);
	poly_z f;
	to_poly_z(f,f_var);
	std::vector<interval_q> intervallist;
	UniRealRootSeparationZ_CF(intervallist,f);
	Resize(result_var,intervallist.size());
	for(uint i=0;i<intervallist.size();++i)
	{
		At(result_var,i)=Vec(Rat(),Rat());
		mpq_set(CRat(At(At(result_var,i),0)),intervallist[i].first);
		mpq_set(CRat(At(At(result_var,i),1)),intervallist[i].second);
	}
	clear_inverval_q_list(intervallist);
	return result_var;
}

Wrap(GroebnerBasis)
{
	var polylist_var=At(x,0);
	var v_var;
	if(Size(x)>1)
	{
		v_var=At(x,1);
	}
	else
	{
		v_var=Variables(polylist_var);
	}
	var result_var=Vec();
	var temp;
	std::vector<sparse_q> polylist,gb;
	uint vars=Size(v_var);
	std::vector<uint> indices(vars,0);
	polylist.resize(Size(polylist_var));
	for(uint i=0;i<Size(polylist_var);++i)
	{
		temp=CoefficientList(Expand(At(polylist_var,i)),v_var);
		to_sparse_q(polylist[i],temp,vars,indices);
	}
	MultiGroebnerBasisQ(gb,polylist,vars);
	Resize(result_var,gb.size());
	for(uint i=0;i<gb.size();++i)
	{
		from_sparse_q(temp,gb[i],vars,0);
		At(result_var,i)=FromCoefficientList(temp,v_var);
	}
	clear_sparse_q_list(polylist);
	clear_sparse_q_list(gb);
	return result_var;
}

Wrap(ZRoot)
{
	var f=At(x,0);
	var v=Variables(f);
	v=At(v,0);
	f=Coefficients(Expand(f),v);
	poly_z fpoly;
	var result=Vec();
	to_poly_z(fpoly,f);
	std::vector<mpz_ptr> rootlist;
	UniZRootZ(rootlist,fpoly);
	Resize(result,rootlist.size());
	for(uint i=0;i<rootlist.size();++i)
	{
		At(result,i)=Int();
		mpz_set(CInt(At(result,i)),rootlist[i]);
	}
	resize_z_list(rootlist,0);
	fpoly.resize(0);
	return result;
}

Wrap(Solve)
{
	var f=At(x,0);
	var v=Variables(f);
	v=At(v,0);
	f=Coefficients(Expand(f),v);
	poly_z fpoly;
	var result;
	to_poly_z(fpoly,f);
	UniSolve_Factorization_Stage(result,fpoly);
	uint size=Size(result);
	for(uint i=0;i<size;++i)
	{
		At(result,i)=Vec(Ex(tag_Rule,Vec(v,At(result,i))));
	}
	fpoly.resize(0);
	return result;
}

Wrap(PolynomialGCD)
{
	var result;
	var f_var=At(x,0),g_var=At(x,1);
	var v_var=Variables(Vec(f_var,g_var));
	uint totalvar=Size(v_var);
	if(totalvar==1)
	{
		f_var=Coefficients(Expand(f_var),At(v_var,0));
		g_var=Coefficients(Expand(g_var),At(v_var,0));
	}
	else
	{
		f_var=CoefficientList(Expand(f_var),v_var);
		g_var=CoefficientList(Expand(g_var),v_var);
	}
	if(Size(x)==3)
	{
		var rule=At(x,2);
		if(Head(rule)==tag_Rule&&At(Body(rule),0)==tag_modulus)
		{
			var p=At(Body(rule),1);
			if(mpz_cmp_ui(CInt(p),0)!=0)
			{
				if(totalvar==1)
				{
					poly_z f,g,h;
					to_poly_z(f,f_var);to_poly_z(g,g_var);
					UniGcdZp(h,f,g,CInt(p));
					from_poly_z(result,h);
					result=FromCoefficients(result,At(v_var,0));
					f.resize(0);g.resize(0);h.resize(0);
					return result;
				}
				else
				{
					sparse_z f,g,h;
					std::vector<uint> indices(totalvar);
					to_sparse_z(f,f_var,totalvar,indices);to_sparse_z(g,g_var,totalvar,indices);
					MultiGcdZp(h,f,g,CInt(p));
					from_sparse_z(result,h,totalvar,0);
					result=FromCoefficientList(result,v_var);
					result=Expand(result);
					f.resize(0);g.resize(0);h.resize(0);
					return result;
				}
			}
		}
	}
	if(totalvar==1)
	{
		poly_z f,g,h;
		to_poly_z(f,f_var);to_poly_z(g,g_var);
		UniGcdZ(h,f,g);
		from_poly_z(result,h);
		result=FromCoefficients(result,At(v_var,0));
		f.resize(0);g.resize(0);h.resize(0);
		return result;
	}
	else
	{
		sparse_z f,g,h;
		std::vector<uint> indices(totalvar);
		to_sparse_z(f,f_var,totalvar,indices);to_sparse_z(g,g_var,totalvar,indices);
		MultiGcdZ(h,f,g);
		from_sparse_z(result,h,totalvar,0);
		result=FromCoefficientList(result,v_var);
		result=Expand(result);
		f.resize(0);g.resize(0);h.resize(0);
		return result;
	}
	return Int(0L);
}

Wrap(Decompose)
{
	var f=At(x,0);
	var v=Variables(f);
	v=At(v,0);
	f=Coefficients(Expand(f),v);
	poly_q fpoly;
	std::vector<poly_q> partlist;
	to_poly_q(fpoly,f);
	UniFuncionalDecompositionQ(partlist,fpoly);
	var result=Vec();
	var g;
	Resize(result,partlist.size());
	for(uint i=0;i<partlist.size();i++)
	{
		from_poly_q(g,partlist[i]);
		g=FromCoefficients(g,v);
		At(result,i)=g;
	}
	clear_poly_q_list(partlist);
	fpoly.resize(0);
	return result;
}

Wrap(Cyclotomic)
{
	var n_var=At(x,0);
	var v_var=At(x,1);
	uint n=mpz_get_ui(CInt(n_var));
	poly_z r;
	UniCyclotomicZ(r,n);
	var r_var=Vec();
	from_poly_z(r_var,r);
	r.resize(0);
	r_var=FromCoefficients(r_var,v_var);
	return r_var;
}

Wrap(CyclotomicQ)
{
	var f_var=At(x,0);
	var v_var=Variables(f_var);
	v_var=At(v_var,0);
	f_var=Coefficients(Expand(f_var),v_var);
	poly_z f,g;
	to_poly_z(f,f_var);
	uint n;
	n=UniShiftedCyclotomicZQ(g,f);
	var n_var=Int();
	mpz_set_ui(CInt(n_var),n);
	if(n!=0)
	{
		var g_var,gg_var;
		g_var=Vec();
		from_poly_z(g_var,g);
		gg_var=FromCoefficients(g_var,v_var);
		f.resize(0);g.resize(0);
		return Ex(tag_cyclotomic,Vec(n_var,gg_var));
	}
	else
	{
		f.resize(0);g.resize(0);
		return n_var;
	}
}

Wrap(FactorList)
{
	var f=At(x,0);
	var v = Variables(f);

	if(Size(v)==0)
	{
		var r=Vec();
		Push(r,f);
		return r;
	}
	//多元情形
	if(Size(v)>1)return 0;

	//非整系数情形
	var fc;
	fc = Coefficients(Expand(f),At(v,0));
	for(size_t i = 0; i < Size(fc); ++i)
	{
		if(!IntQ(At(fc,i)))return 0;
	}
	if(Size(x)==2)
	{
		var rule=At(x,1);
		if(Head(rule)==tag_Rule&&At(Body(rule),0)==tag_modulus)
		{
			var p=At(Body(rule),1);
			if(mpz_cmp_ui(CInt(p),0)!=0)
			{
				var r=UniFacZp(fc,p);
				size_t n = Size(r);
				for(size_t i = 1; i < n; ++i) 
				{
					At(At(r,i),0) = FromCoefficients(At(At(r,i),0),At(v,0));
				}
				return r;
			}
		}
	}
	var r = UniFacZ(fc);
	size_t n = Size(r);
	for(size_t i = 1; i < n; ++i) 
	{
		At(At(r,i),0) = FromCoefficients(At(At(r,i),0),At(v,0));
	}
	return r;
}

Wrap(Factor)
{
	var r,result;
	r=wrap_FactorList(x);
	//error
	if(!r)return 0;

	result=Vec();
	uint size=Size(r);
	for(uint i=1;i<size;++i)
	{
		Push(result,Ex(TAG(Power),Vec(At(At(r,i),0),At(At(r,i),1))));
	}
	result=Ex(TAG(Times),result);
	result=Ex(TAG(Times),Vec(At(r,0),result));
	return result;
}

//todo 不同的字典序
Wrap(CharacterSet)
{
	var v_var;
	var polylist_var;
	uint vars;
	polylist_var=At(x,0);
	if(Size(x)==1)
	{
		v_var=Variables(polylist_var);
	}
	if(Size(x)==2)
	{
		v_var=At(x,1);
	}
	vars=Size(v_var);
	var f_var;
	std::vector<sparse_z> polylist,cs;
	std::vector<uint> indices(vars,0);
	polylist.resize(Size(polylist_var));
	for(uint i=0;i<Size(polylist_var);i++)
	{
		f_var=CoefficientList(Expand(At(polylist_var,i)),v_var);
		to_sparse_z(polylist[i],f_var,vars,indices);
		polylist[i].normalize();
	}
	CharacterSetZ(cs,polylist,vars);
	var result=Vec();
	Resize(result,cs.size());
	for(uint i=0;i<cs.size();i++)
	{
		from_sparse_z(f_var,cs[i],vars,0);
		At(result,i)=FromCoefficientList(f_var,v_var);
	}
	clear_sparse_z_list(polylist);clear_sparse_z_list(cs);
	return result;
}

}

void to_poly_q(poly_q & f,Var g)
{
	size_t n=Size(g);
	f.resize(n);
	static var unit=Int(1L);
	for(size_t i=0;i<n;i++)
	{
		if(IntQ(At(g,i)))
		{
			mpq_set_num(f[i],CInt(At(g,i)));
			mpq_set_den(f[i],CInt(unit));
		}
		else
		{
			mpq_set(f[i],CRat(At(g,i)));
		}
	}
	return ;
}

void from_poly_q(var & f, const poly_q & g)
{
	size_t n=g.size();
	f=Vec();
	Resize(f,n);
	for(size_t i=0;i<n;i++)
	{
		At(f,i)=Rat();
		mpq_set(CRat(At(f,i)),g[i]);
	}
	return ;
}

void to_poly_z(poly_z &f, Var g)
{
	size_t n = Size(g);
	f.resize(n);
	for(size_t i = 0; i < n; ++i)
	{
		mpz_set(f[i], CInt(At(g,i)));
	}
}

void from_poly_z(var & f, const poly_z& g)
{
	size_t n = g.size();
	f=Vec();
	Resize(f,n);
	for(size_t i = 0; i < n; ++i)
	{
		At(f,i) = Int();
		mpz_set(CInt(At(f,i)), g[i]);
	}
}

//make sure r should be empty before converting, indices should be resized to n at first
//don't forget to normalize r after this proceed
void to_sparse_z(sparse_z & r,Var s,uint n,std::vector<uint> & indices)
{
	if(n==0)
	{
		r.resize(r.size()+1);
		mpz_set(r[r.size()-1].coe,CInt(s));
		r[r.size()-1].exponents=indices;
		std::reverse(r[r.size()-1].exponents.begin(),r[r.size()-1].exponents.end());
		return ;
	}
	for(uint i=0;i<Size(s);i++)
	{
		indices[n-1]=i;
		to_sparse_z(r,At(s,i),n-1,indices);
	}
	if(n==indices.size())
	{
		r.normalize();
	}
	return ;
}

void from_sparse_z(var & s,const sparse_z & r,uint n,uint index)
{
	if(index==n)
	{
		s=Int();
		if(r.size()==0)
		{
			mpz_set_ui(CInt(s),0);
			return ;
		}
		mpz_set(CInt(s),r[0].coe);
		return ;
	}
	uint degx=r.deg(index);
	uint size=r.size();
	uint i_x;
	std::vector<sparse_z> ri;
	ri.resize(degx+1);
	s=Vec();
	Resize(s,degx+1);
	for(uint i=0;i<size;i++)
	{
		i_x=r[i].exponents[index];
		ri[i_x].resize(ri[i_x].size()+1);
		mpz_set(ri[i_x][ri[i_x].size()-1].coe,r[i].coe);
		ri[i_x][ri[i_x].size()-1].exponents=r[i].exponents;
	}
	for(uint i=0;i<=degx;i++)
	{
		from_sparse_z(At(s,i),ri[i],n,index+1);
	}
	clear_sparse_z_list(ri);
	return ;
}

void to_sparse_q(sparse_q & r,Var s,uint n,std::vector<uint> & indices)
{
	if(n==0)
	{
		r.resize(r.size()+1);
		if(IntQ(s))
		{
			mpq_set_num(r[r.size()-1].coe,CInt(s));
			mpq_set_den(r[r.size()-1].coe,z_unit);
		}
		else 
		{
			mpq_set(r[r.size()-1].coe,CRat(s));
		}
		r[r.size()-1].exponents=indices;
		std::reverse(r[r.size()-1].exponents.begin(),r[r.size()-1].exponents.end());
		return ;
	}
	for(uint i=0;i<Size(s);i++)
	{
		indices[n-1]=i;
		to_sparse_q(r,At(s,i),n-1,indices);
	}
	if(n==indices.size())
	{
		r.normalize();
	}
	return ;
}

void from_sparse_q(var & s,const sparse_q & r,uint n,uint index)
{
	if(index==n)
	{
		s=Rat();
		if(r.size()==0)
		{
			mpq_set_ui(CRat(s),0,1);
			return ;
		}
		mpq_set(CRat(s),r[0].coe);
		return ;
	}
	uint degx=r.deg(index);
	uint size=r.size();
	uint i_x;
	std::vector<sparse_q> ri;
	ri.resize(degx+1);
	s=Vec();
	Resize(s,degx+1);
	for(uint i=0;i<size;i++)
	{
		i_x=r[i].exponents[index];
		ri[i_x].resize(ri[i_x].size()+1);
		mpq_set(ri[i_x][ri[i_x].size()-1].coe,r[i].coe);
		ri[i_x][ri[i_x].size()-1].exponents=r[i].exponents;
	}
	for(uint i=0;i<=degx;i++)
	{
		from_sparse_q(At(s,i),ri[i],n,index+1);
	}
	clear_sparse_q_list(ri);
	return ;
}
void PolynomialInitialize()
{
	mpz_init(z_unit);mpz_set_ui(z_unit,1);
	mpz_init(z_zero);mpz_set_ui(z_zero,0);
	mpq_init(q_unit);mpq_set_ui(q_unit,1,1);
	mpq_init(q_zero);mpq_set_ui(q_zero,0,1);
	tag_cyclotomic=Sym(L"Cyclotomic",System);
	tag_modulus=Sym(L"Modulus",System);
	tag_root=Sym(L"Root",System);

	random::randomize();
	Begin(System);
	Externals[Sym(L"Factor")] = WRAP(Factor);
	Externals[Sym(L"FactorList")] = WRAP(FactorList);
	Externals[Sym(L"CharacterSet")]= WRAP(CharacterSet);
	Externals[Sym(L"Cyclotomic")]= WRAP(Cyclotomic);
	Externals[Sym(L"CyclotomicQ")]= WRAP(CyclotomicQ);
	Externals[Sym(L"test")]= WRAP(Test);
	Externals[Sym(L"Decompose")]= WRAP(Decompose);
	Externals[Sym(L"PolynomialGCD")]= WRAP(PolynomialGCD);
	Externals[Sym(L"Solve")]= WRAP(Solve);
	Externals[Sym(L"ZRoot")]= WRAP(ZRoot);
	Externals[Sym(L"GroebnerBasis")]= WRAP(GroebnerBasis);
	Externals[Sym(L"RootIntervals")]= WRAP(RootIntervals);
	End();
	UniSolveInitialize();
	ComplexInitialize();
}
//////////////////////////////////////
}
DLLMAIN(mU::PolynomialInitialize)
