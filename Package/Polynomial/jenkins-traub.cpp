/**
\file
\brief Jenkins-Traub算法
*/
#include "polyfloat.h"

namespace mU {

namespace{

void hshift(poly_fc r,poly_fc h,poly_fc p,mpfc_t s)
{
	static mpfc_t temp1,temp2;
	poly_fc temp,xs;
	xs.resize(2);
	mpfc_set(xs[1],mpfc_one);
	mpfc_neg(xs[0],s);
	mpfc_init(temp1);mpfc_init(temp2);
	UniEvalFC(temp1,h,s);
	UniEvalFC(temp2,p,s);
	mpfc_div(temp1,temp1,temp2);
	UniMulFC(temp,temp1,p);
	UniSubFC(temp,h,temp);
	UniDivFC(r,temp,xs);
	mpfc_clear(temp1);mpfc_clear(temp2);
	return ;
}

void sshift(mpfc_t r,poly_fc h,poly_fc p,mpfc_t s)
{
	static mpfc_t temp;
	mpfc_init(temp);
	UniEvalFC(r,p,s);
	UniEvalFC(temp,h,s);
	mpfc_div(r,r,temp);
	mpfc_set(temp,h[h.size()-1]);
	mpfc_mul(r,r,temp);
	mpfc_sub(r,s,r);
	mpfc_clear(temp);
}

void jtstep(mpfc_t r,poly_fc p,uint digits)
{
	uint M=5,L=20,R=1000;

	return ;
}

}

void UniFCRootZ_Jenkins_Traub(poly_fc & roots, poly_z & f, uint digits)
{

	return ;
}

}
