#pragma once
#include <mU/Kernel.h>
#include <ctime>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>

#ifndef _MSC_VER
#define __cdecl __attribute__((cdecl))
#endif

namespace mU {
//////////////////////////////////////

//ExternPolynomial.cpp
extern mpz_t z_unit,z_zero;
extern mpq_t q_unit,q_zero;
extern var tag_modulus,tag_cyclotomic,tag_root;

//////////////////////////////////////
//functions of univariate polynomials
//polyrandom.cpp
class random
{
public:
	static gmp_randstate_t rstate;
	static void randomize();
	static void randominteger(mpz_ptr r,mpz_ptr a,mpz_ptr b);
	static void randominteger(int & r,int a,int b);
};

//uniarithmetic.cpp
struct poly_z : public std::vector<mpz_ptr>
{
	void resize(size_t n);
	int deg();
	void normalize();
	mpz_ptr* elts();
	mpz_ptr const* elts() const;
	void print();
	void print() const;
};
bool is_x_power_z(const poly_z & f);
void copy_poly_z(poly_z & r,const poly_z & f);
void poly_z_dform(poly_z & r,const poly_z & f);
void UniNegZ(poly_z & r,const poly_z & f);
void UniMulZ(poly_z & r,const poly_z & f, const poly_z & g);
void UniAddZ(poly_z & r,const poly_z & f,const poly_z & g);
void UniSubZ(poly_z & r,const poly_z & f,const poly_z & g);
void UniDivExactZ(poly_z & q,const poly_z & f,const poly_z & g);
void UniPseudoModZ(poly_z & r,const poly_z & f,const poly_z & g);
void UniEvalZ(mpz_ptr r,const poly_z & f,mpz_ptr x);
void UniEvalZp(mpz_ptr r,const poly_z & f,mpz_ptr x,mpz_ptr p);
void UniPolynomialMod(poly_z & r,const poly_z & f,mpz_ptr m);
void UniDivModZp(poly_z & q,poly_z & r,const poly_z & f,const poly_z & g,mpz_ptr p);
void UniDivZp(poly_z & q,const poly_z & f,const poly_z & g,mpz_ptr p);
void UniModZp(poly_z & r,const poly_z & f,const poly_z & g,mpz_ptr p);
bool poly_z_divisible(const poly_z & f,const poly_z & g);
void resize_z_list(std::vector<mpz_ptr> & r,uint n);
void clear_poly_z_list(std::vector<poly_z> & r);
class poly_q
{
public:
	inline uint size() {return this->rep.size();}
	inline uint size() const {return this->rep.size();}
	inline mpq_ptr & operator [](uint i) {return (mpq_ptr &)this->rep[i];}
	inline const mpq_ptr & operator [](uint i) const {return (const mpq_ptr &)this->rep[i];}
	void resize(uint n);
	void normalize();
	void print();
	void print() const;
private:
	std::vector<mpq_ptr> rep;
};
bool is_x_power_q(const poly_q & f);
void x_power_q(poly_q & r,uint n);
void poly_z_to_poly_q(poly_q & r,const poly_z & f);
void poly_q_to_poly_z(poly_z & r,const poly_q & f);
void poly_q_dform(poly_q & r,const poly_q & f);
void poly_q_simpleform(mpq_ptr a,poly_z & r,const poly_q & f);
void poly_q_simpleform(mpq_ptr a,poly_q & r,const poly_q & f);
void copy_poly_q(poly_q & r,const poly_q & f);
void clear_poly_q_list(std::vector<poly_q> & r);
void UniNegQ(poly_q & r,const poly_q & f);
void UniEvalQ(mpq_ptr r,const poly_q & f,mpq_ptr x);
void UniMulQ(poly_q & r, const poly_q & f, const poly_q & g);
void UniDivModQ(poly_q & q,poly_q & r,const poly_q & f,const poly_q & g);
void UniDivQ(poly_q & q,const poly_q & f,const poly_q & g);
void resize_q_list(std::vector<mpq_ptr> & r,uint n);

//unigcd.cpp
void IntegerLength_BigBase(mpz_ptr r,mpz_ptr n,mpz_ptr b);
void UniMaxNormZ(mpz_ptr r,const poly_z & f);
void UniOneNormZ(mpz_ptr r,const poly_z & f);
void UniContZ(mpz_ptr r,const poly_z & f);
void UniPPZ(poly_z & r,const poly_z & f);
void UniGcdZp(poly_z & r,const poly_z & f,const poly_z & g,mpz_ptr p);
void UniGcdZp_Ext(poly_z & r,poly_z & s,poly_z & t,const poly_z & f,const poly_z & g,mpz_ptr p);
void UniGcdZ_SmallPrime1(poly_z & r,const poly_z & f,const poly_z & g);
void UniGcdZ(poly_z & r,const poly_z & f,const poly_z & g);

//unifaczp.cpp
void Power(poly_z & f,uint n,mpz_ptr p);
void PowerMod(poly_z & f,mpz_ptr n,const poly_z & g,mpz_ptr p);
void PowerMod_Pth_Power(poly_z & f,const poly_z & g,mpz_ptr p);
void UniDisDegFacZp(std::vector<poly_z> & disfactorlist, std::vector<uint> & deglist,const poly_z & f,mpz_ptr p);
void UniRandomZp(poly_z & r,uint deg_bound,mpz_ptr p);
void UniEqlDegFacZp(std::vector<poly_z> & eqlfactorlist,const poly_z & f,mpz_ptr p,uint d);
void UniFacZp_fsqfmonic(std::vector<poly_z> & faclist,const poly_z & f,mpz_ptr p);
void UniSqrFreeDecomZp(const poly_z & f,std::vector<poly_z> & sqflist,mpz_ptr p);
void UniFacZp(const poly_z & f,mpz_ptr p,mpz_ptr b,std::vector<poly_z> & faclist,std::vector<uint> & exponents);

//unismallprime.cpp
void BerlekampZp_SmallPrime(const poly_z & f,mpz_ptr p,std::vector<poly_z> & faclist);

//unifacz.cpp
void UniSqrFreeDecomZ(const poly_z & f,std::vector<poly_z> & sqflist);
void UniFacZ_Hensel_FactorCombination(const poly_z & f,std::vector<poly_z> & faclist);
void UniFacZ(const poly_z & f,mpz_ptr b,std::vector<poly_z> & faclist,std::vector<uint> & exponents);

//unicyclotomic.cpp
void UniCyclotomicZ(poly_z & r,uint n);
uint UniCyclotomicZQ_InversePhi(const poly_z & f);
uint UniShiftedCyclotomicZQ(poly_z & r,const poly_z & f);

//uniexactsolution.cpp
void UniZpRootZp(std::vector<mpz_ptr> & rootlist,const poly_z & f,mpz_ptr p);
void UniZRootZ_ByZp(std::vector<mpz_ptr> & rootlist,const poly_z & f);
void UniZRootZ_ByFactor(std::vector<mpz_ptr> & rootlist,const poly_z & f);
void UniZRootZ_BySeparation(std::vector<mpz_ptr> & rootlist,const poly_z & f);
void UniZRootZ(std::vector<mpz_ptr> & rootlist, const poly_z & f);

//unidecompose.cpp
void UniNthRootQ_Euler(poly_q & r,const poly_q & g,mpq_ptr a,uint s);
void UniReversalQ(poly_q & r,const poly_q & s,uint n);
void UniFuncionalDecompositionQ(std::vector<poly_q> & partlist,const poly_q & f);
void UniFuncionalDecompositionZ(mpq_ptr a,std::vector<poly_z> & partlist,const poly_z & f);

//unirootseparation.cpp
class interval_q
{
public:
	mpq_t first,second;
	void init() {mpq_init(first);mpq_init(second);};
	void destroy() {mpq_clear(first);mpq_clear(second);};
	friend bool operator < (const interval_q & f, const interval_q & g)
	{
		int c1=mpq_cmp(f.first,g.first),c2=mpq_cmp(f.second,g.second);
		return c1<0||(c1==0&&c2<0);
	}
	void print()
	{
		std::cout<<"[";
		mpq_out_str(0,10,first);
		std::cout<<",";
		mpq_out_str(0,10,second);
		std::cout<<"]\n";
	}
};
void clear_inverval_q_list(std::vector<interval_q> &x);
void UniUpperBoundOfRootZ(mpz_ptr b, const poly_z & f);
void UniSturmSequenceZ(std::vector<poly_z> & sturm_seq, const poly_z & f);
uint UniSturmChangeSignZ(std::vector<poly_z> & sturm_seq, mpz_ptr x);
void UniRealRootSeparationZ(std::vector<interval_q> & intervallist, const poly_z & f);
void UniRealRootSeparationZ_CF(std::vector<interval_q> & intervallist, const poly_z & f);

//////////////////////////////////////
//functions of multivariate polynomials
//multisparsez.cpp
class mono_z
{
public:
	mpz_t coe;
	std::vector<uint> exponents;
};
typedef int (__cdecl *order_z)(const mono_z &,const mono_z &);
int lex_z(const mono_z & f,const mono_z & g);
bool equal_mono_z_type(const mono_z & f,const mono_z & g,uint n_bound);
//to do: to separate deleting 0 terms and sorting in normalizing
class sparse_z
{
public:
	void normalize(order_z order=lex_z);
	inline uint size();
	inline uint size() const;
	void resize(size_t n);
	inline mono_z & right(){return (mono_z &)this->monomials[this->monomials.size()-1];}
	inline mono_z & right() const {return (mono_z &)this->monomials[this->monomials.size()-1];}
	inline mono_z & operator [](uint i) {return (mono_z &)this->monomials[i];}
	inline const mono_z & operator [](uint i) const {return (const mono_z &)this->monomials[i];}
	void combine_ordered();
	void print();
	void print() const;
	uint deg(uint variable=0);
	uint deg(uint variable=0) const;
private:
	std::vector<mono_z> monomials;
};
void copy_sparse_z(sparse_z & r,const sparse_z & s);
void clear_sparse_z_list(std::vector<sparse_z> & x);
void sparse_z_to_poly_z(poly_z & r,const sparse_z & s);
void poly_z_to_sparse_z(sparse_z & r, const poly_z & s, uint totalvar, uint variable);
void mono_z_mul(mono_z & r,const mono_z & f,const mono_z & g);
void MultiAddZ(sparse_z & r,const sparse_z & f,const sparse_z & g,order_z order=lex_z);
void MultiSubZ(sparse_z & r,const sparse_z & f,const sparse_z & g,order_z order=lex_z);
void MultiMulZ(sparse_z & r,const sparse_z & f,const sparse_z & g,order_z order=lex_z);
void MultiMulZ(sparse_z & r,const sparse_z & f,mpz_ptr a,order_z order=lex_z);
void MultiPowZ(sparse_z & r,const sparse_z & f,uint n,order_z order=lex_z);
void MultiDivExactZ(sparse_z & r,const sparse_z & f,const sparse_z & g,order_z order=lex_z);
bool MultiDivisibleZ(const sparse_z & f,const sparse_z & g,order_z order=lex_z);
void MultiContZ(mpz_ptr r, const sparse_z & f);
void MultiPPZ(sparse_z & r, const sparse_z & f);
void MultiContPPZ(mpz_ptr cont, sparse_z & pp, const sparse_z & f);
void MultiMaxNormZ(mpz_ptr r, const sparse_z & f);
void MultiPolynomialMod(sparse_z & r,const sparse_z & f,mpz_ptr p,order_z order=lex_z);
void MultiDivExactZp(sparse_z & r,const sparse_z & f,const sparse_z & g,mpz_ptr p,order_z order=lex_z);
bool MultiDivisibleZp(const sparse_z & f,const sparse_z & g,mpz_ptr p,order_z order=lex_z);

void to_main_variable(std::vector<sparse_z> & f,const sparse_z & g,uint variable);
void from_main_variable(sparse_z & f,const std::vector<sparse_z> & g,uint variable);
void eval_in_one_variable_z(sparse_z & r,const sparse_z & f,mpz_ptr x,uint variable);

//multisparseq.cpp
class mono_q
{
public:
	mpq_t coe;
	std::vector<uint> exponents;
};
typedef int (__cdecl *order_q)(const mono_q &,const mono_q &);
int lex_q(const mono_q & f,const mono_q & g);
bool equal_mono_q_type(const mono_q & f,const mono_q & g,uint n_bound);
//to do: to separate deleting 0 terms and sorting in normalizing
class sparse_q
{
public:
	void normalize(order_q order=lex_q);
	inline uint size();
	inline uint size() const;
	void resize(size_t n);
	inline mono_q & right(){return (mono_q &)this->monomials[this->monomials.size()-1];}
	inline mono_q & right() const {return (mono_q &)this->monomials[this->monomials.size()-1];}
	inline mono_q & operator [](uint i) {return (mono_q &)this->monomials[i];}
	inline const mono_q & operator [](uint i) const {return (const mono_q &)this->monomials[i];}
	void combine_ordered();
	void print();
	void print() const;
	uint deg(uint variable=0);
	uint deg(uint variable=0) const;
	void erase(uint i);
private:
	std::vector<mono_q> monomials;
};
void copy_sparse_q(sparse_q & r,const sparse_q & s);
void clear_sparse_q_list(std::vector<sparse_q> & x);
void mono_q_mul(mono_q & r,const mono_q & f,const mono_q & g);
void MultiAddQ(sparse_q & r,const sparse_q & f,const sparse_q & g,order_q order=lex_q);
void MultiSubQ(sparse_q & r,const sparse_q & f,const sparse_q & g,order_q order=lex_q);
void MultiMulQ(sparse_q & r,const sparse_q & f,const sparse_q & g,order_q order=lex_q);
void MultiMulQ(sparse_q & r,const sparse_q & f,mpq_ptr a,order_q order=lex_q);
void MultiDivQ(sparse_q & r,const sparse_q & f,const sparse_q & g,order_q order=lex_q);
bool MultiDivisibleQ(const sparse_q & f,const sparse_q & g,order_q order=lex_q);
void MultiSimpleFormQ(mpq_ptr a, sparse_q & r, const sparse_q & f);

void to_main_variable(std::vector<sparse_q> & f,const sparse_q & g,uint variable);
void from_main_variable(sparse_q & f,const std::vector<sparse_q> & g,uint variable);

//multirss.cpp

//multigcdzp.cpp
void MultiGcdZp_Dense_Interpolation(sparse_z & r,const sparse_z & f, const sparse_z & g,uint n, mpz_ptr p);
void MultiGcdZp(sparse_z & r,const sparse_z & f, const sparse_z & g, mpz_ptr p);

//multigcdz.cpp
void MultiGcdZ_Dense_Interpolation(sparse_z & r, const sparse_z & f, const sparse_z & g, uint n);
void MultiGcdZ_sparse2(sparse_z & r, const sparse_z & skel, const sparse_z & l, const sparse_z & f, const sparse_z & g, uint Tk, uint k, uint D);
void MultiGcdZ_Sparse1(sparse_z & r, const sparse_z & l, const sparse_z & f, const sparse_z & g, uint k, uint D);
void MultiGcdZ_Sparse(sparse_z & r, const sparse_z & f, const sparse_z & g, uint n);
void MultiGcdZ(sparse_z & r, const sparse_z & f,const sparse_z & g);

//multiwu.cpp
void CharacterSetZ(std::vector<sparse_z> & cs, const std::vector<sparse_z> & polylist, uint totalvar);
void ReduceByAscendingSet(sparse_z & r,const sparse_z & f,const std::vector<sparse_z> & polylist, uint totalvar);

//multigroebner.cpp
void MultiGroebnerBasisQ(std::vector<sparse_q> & gb, const std::vector<sparse_q> & polylist, uint totalvar, order_q order=lex_q);

//////////////////////////////////////
//connection between functions of polynomials and the kernel
//ExternFactorization.cpp
var UniFacZp(Var f,Var p);
var UniFacZ(Var f);

//ExternPolynomial.cpp
void PolynomialInitialize();
void to_sparse_z(sparse_z & r,Var s,uint n,std::vector<uint> & indices);
void from_sparse_z(var & s,const sparse_z & r,uint n,uint i);
void to_sparse_q(sparse_q & r,Var s,uint n,std::vector<uint> & indices);
void from_sparse_q(var & s,const sparse_q & r,uint n,uint index);
void to_poly_z(poly_z &f, Var g);
void from_poly_z(var & f, const poly_z& g);
void to_poly_q(poly_q & f,Var g);
void from_poly_q(var & f, const poly_q & g);

//ExternUnisolve.cpp
void UniSolveInitialize();
void UniSolve_Decomposition_Stage(var & rootlist, const poly_z & f);
void UniSolve_Factorization_Stage(var & rootlist, const poly_z & f);

//////////////////////////////////////
}
