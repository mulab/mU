/** \file
\brief 多项式模快里用到的生成随机数的类
\todo 随机生成多项式移至此处
*/
#include "common.h"

namespace mU{

namespace{

}

gmp_randstate_t random::rstate;

void random::randomize()

{
	gmp_randinit_default(rstate);
	mpz_t time_z;mpz_init(time_z);
	mpz_set_ui(time_z,time(NULL));
	gmp_randseed(rstate,time_z);
	mpz_clear(time_z);
	return ;
}

void random::randominteger(mpz_ptr r,mpz_ptr a,mpz_ptr b)
{
	mpz_t temp_z;
	mpz_init(temp_z);
	mpz_sub(temp_z,b,a);
	mpz_add_ui(temp_z,temp_z,1);
	mpz_urandomm(r,rstate,temp_z);
	mpz_clear(temp_z);
	mpz_add(r,r,a);
	return ;
}

void random::randominteger(int & r,int a,int b)
{
	mpz_t rr,bound;
	mpz_init(rr);mpz_init(bound);
	mpz_set_si(rr,r);mpz_set_si(bound,b-a+1);
	mpz_urandomm(rr,rstate,bound);
	r=mpz_get_si(rr);
	r=r+a;
	mpz_clear(rr);mpz_clear(bound);
	return ;
}

}
