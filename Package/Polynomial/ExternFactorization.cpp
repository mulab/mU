/**
\file
\brief 整系数一元多项式因子分解
\todo 特别地，要加上不可约性的检测，快速分解诸如x^10000-2; x^n-1型多项式的分解
*/
#include "common.h"

namespace mU {

namespace{

}

/**
\brief 整系数多项式因子分解.
\param f 整系数一元多项式.
\return f的分解结果.
*/
var UniFacZ(Var f)
{
	var result=Vec();
	poly_z fz;
	std::vector<poly_z> faclist;
	std::vector<uint> exponents;
	var b = Int();
	to_poly_z(fz,f);
	UniFacZ(fz,CInt(b),faclist,exponents);
	Push(result,b);
	var factor;
	for(size_t i=0;i<faclist.size();i++)
	{
		factor=Vec();
		from_poly_z(factor,faclist[i]);
		Push(result,Vec(factor,Int(exponents[i])));
	}
	clear_poly_z_list(faclist);
	return result;
}

var UniFacZp(Var f,Var p)
{
	var result=Vec();
	poly_z fz;
	std::vector<poly_z> faclist;
	std::vector<uint> exponents;
	static mpz_t p_z;
	mpz_init(p_z);
	mpz_set(p_z,CInt(p));
	var b = Int();
	to_poly_z(fz,f);
	UniFacZp(fz,p_z,CInt(b),faclist,exponents);
	Push(result,b);
	var factor;
	for(size_t i=0;i<faclist.size();i++)
	{
		factor=Vec();
		from_poly_z(factor,faclist[i]);
		Push(result,Vec(factor,Int(exponents[i])));
	}
	clear_poly_z_list(faclist);
	mpz_clear(p_z);
	return result;
}

}
