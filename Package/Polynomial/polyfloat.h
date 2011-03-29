#pragma once
#include "common.h"

namespace mU {

//floatbasic.cpp
void ComplexInitialize();
struct mpfc
{
	mpf_t Re,Im;
};
typedef mpfc mpfc_t[1];
typedef mpfc *mpfc_ptr;
extern mpf_t mpfc_mpf_temp[4];
extern mpfc_t mpfc_one,mpfc_zero;

void mpfc_init(mpfc_ptr x);
void mpfc_setdigits(mpfc_ptr x,uint digits);
uint mpfc_getdigits(mpfc_ptr x);
void mpfc_clear(mpfc_ptr x);
void mpfc_out_str(FILE * stream,int base, size_t n_digits,mpfc_ptr x);
void mpfc_neg(mpfc_ptr x,mpfc_ptr y);
void mpfc_set(mpfc_ptr x,mpfc_ptr y);
void mpfc_set_z(mpfc_ptr x,mpz_ptr y);
void mpfc_set_ui(mpfc_ptr x, uint y);
void mpfc_set_c(mpfc_ptr x,uint a,uint b);
bool mpfc_iszero(mpfc_ptr x);
void mpfc_add(mpfc_ptr r,mpfc_ptr x,mpfc_ptr y);
void mpfc_sub(mpfc_ptr r,mpfc_ptr x,mpfc_ptr y);
void mpfc_mul(mpfc_ptr r,mpfc_ptr x,mpfc_ptr y);
void mpfc_mul_ui(mpfc_ptr r,mpfc_ptr x,uint a);
void mpfc_div(mpfc_ptr r,mpfc_ptr x,mpfc_ptr y);
void mpfc_abs(mpf_t r,mpfc_ptr x);

class poly_fc
{
public:
	inline uint size() {return this->rep.size();}
	inline mpfc_ptr & operator [](uint i) {return (mpfc_ptr &)this->rep[i];}
	void resize(uint n);
	void normalize();
	void print();
private:
	std::vector<mpfc_ptr> rep;
};
void poly_z_to_poly_fc(poly_fc & r, poly_z & f);
void UniEvalFC(mpfc_ptr r,poly_fc & f,mpfc_ptr x);
void UniDFormFC(poly_fc & r,poly_fc & f);
void UniAddFC(poly_fc & r,poly_fc & f,poly_fc & g);
void UniSubFC(poly_fc & r,poly_fc & f,poly_fc & g);
void UniMulFC(poly_fc & r,mpfc_ptr a,poly_fc & f);	
void UniMulFC(poly_fc & r,poly_fc & f,poly_fc & g);
void UniDivFC(poly_fc & q,poly_fc & f,poly_fc & g);

class poly_f
{
public:
	inline uint size() {return this->rep.size();}
	inline mpf_ptr & operator [](uint i) {return (mpf_ptr &)this->rep[i];}
	void resize(uint n);
	void normalize();
	void print();
private:
	std::vector<mpf_ptr> rep;
};
void UniEvalF(mpf_ptr r, poly_f & f, mpf_ptr x);
void UniDFormF(poly_f & r, poly_f & f);
void UniRootF_Newton();
void UniFCRootZ_Jenkins_Traub(poly_fc & roots, poly_z & f, uint digits);

}
