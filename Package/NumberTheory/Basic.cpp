/** \file
\brief 基础数论函数.
*/
#include "common.h"
#include "objects/integer.h"
#include <gmp.h>
#include <ctime>
#include <algorithm>
#pragma comment(lib, "gmp")

#include "numtheory.h"

namespace Integer = maTHmU::Objects::Integer;
namespace Rational = maTHmU::Objects::Rational;

namespace maTHmU {
namespace
{
	inline mpz_ptr PTR(VOID *x) { return (mpz_ptr)x; }
	inline mpz_ptr PTR(VAR x) { return PTR(x.ptr); }
	inline Z& CAST(VAR x) { return *(Z*)(&x); }
// 	inline var gMod(VAR x, VAR y)
// 	{
// 		if(GetType(x) == Objects::Integer::INTEGER)
// 			return Mod(x,y);
// 		if(GetType(x) == Objects::Polynomial::POLYNOMIAL)
// 			return Objects::Polynomial::Mod(x,y);
// 	}
// 	/** \brief
// 		\param
// 		\return
// 		\note
// 		\todo PolynomialExtendedGCD
// 	*/
// 	inline var ExtendedGCD(VAR x, VAR y)
// 	{
// 		if(GetType(x) == Objects::Integer::INTEGER)
// 			return ExtendedGCD(x,y);
// 		if(GetType(x) == Objects::Polynomial::POLYNOMIAL)
// 			return 0;
// 	}
	inline var operator * (VAR x, VAR y)
	{
		return Integer::Mul(x,y);
	}
	inline var operator +(VAR x, VAR y)
	{
		return Integer::Add(x,y);
	}
	inline var operator -(VAR x, VAR y)
	{
		return Integer::Sub(x,y);
	}
	inline bool operator ==(VAR x, VAR y)
	{
		return Integer::Cmp(x,y)==0;
	}

	inline bool operator <=(VAR x, VAR y)
	{
		return Integer::Cmp(x,y)<=0;
	}
	inline bool operator >=(VAR x, VAR y)
	{
		return Integer::Cmp(x,y)>=0;
	}
	inline bool operator <(VAR x, VAR y)
	{
		return Integer::Cmp(x,y)<0;
	}
	inline bool operator >(VAR x, VAR y)
	{
		return Integer::Cmp(x,y)>0;
	}
	inline var operator /(VAR x, VAR y)
	{
		return Modules::NumberTheory::Quotient(x,y);
	}

	inline bool less(VAR n,VAR m) {
		return Integer::Cmp(Z::cast(n),Z::cast(m))<0;
	}
}
namespace Modules {
/** \brief 数论模块

包含各种数论函数.
*/
namespace NumberTheory {

gmp_randstate_t rstate; /**< 随机状态变量. */

/** \brief 随机数初始化.
	\note 需在调用::RandomInteger之前执行.
*/
VOID Randomize() {
	gmp_randinit_default(rstate);
	gmp_randseed_ui(rstate,time(NULL));
}

/** \brief 使用\f$n\f$作为随机数种子重置随机数生成器.
	\param n 整数.
*/
VOID SeedRandom(VAR n) {
	gmp_randseed(rstate,PTR(n));
}

/** \brief 使用系统时间作为随机数种子重置随机数生成器.
*/
VOID SeedRandom() {
	gmp_randseed_ui(rstate,time(NULL));
}

/** \brief 产生随机整数.
	\param n 正整数.
	\return 0到n之间一致分布的随机数.
	\param {a,b} 正整数对.
	\return a到b之间一致分布的随机数.
	\note 需先执行::Randomize.
	\todo 判断界.
*/
Z RandomInteger(VAR n) {
	var r=Z(0);
	if (list::type(n)) {
		var diff = list::cast(n)[1]-list::cast(n)[0];
		mpz_urandomm(PTR(r),rstate,PTR(diff+Z(1)));
		r=r+list::cast(n)[0];
	}
	else {
		mpz_urandomm(PTR(r),rstate,PTR(n+Z(1)));
	}
	return Z::cast(r);
}

/** \brief 产生随机素数.
\param n 大于1的整数.
\return 0到n之间一致分布的随机素数.
\param {a,b} 包含素数的正整数对.
\return a到b之间一致分布的随机素数.
\note 需先执行::Randomize.
\todo 判断界.
*/
Z RandomPrime(VAR n) {
	var m;
	var a,b;
	if (list::type(n)) {
		a=list::cast(n)[0];
		b=list::cast(n)[1];
	}
	else {
		a=Z(0);
		b=n;
	}
	while (1) {
		m=RandomInteger(n);
		if (EvenQ(m) && !(m==Z(2)) ) m=m-Z(1);
		while (m<=b && a<=m) {
			if (PrimeQ(m)) return Z::cast(m);
			else m=m+Z(2);
		}
	}
}

/** \brief 求大于\f$n\f$的第一个素数.
	\param n 整数.
	\return 大于\f$n\f$的第一个素数.
	\todo 非整数判断.
*/
Z NextPrime(VAR n) {
	if (n<=Z(1) && Z(-2)<=n) return Z(2);
	if (n==Z(-3)) return Z(-2);
	var m;
	if (EvenQ(n)) m=n+Z(1); else m=n+Z(2);
	while (1) {
		if (PrimeQ(m)) return Z::cast(m);
		else m=m+Z(2);
	}
}
/** \brief 求大于\f$n\f$的第\f$k\f$个素数.
\param n 整数.
\param k 整数.
\return 大于\f$n\f$的第\f$k\f$个素数. 当\f$k\f$为负数时给出小于\f$n\f$的第\f$k\f$个素数.
\todo 非整数判断.
*/
Z NextPrime(VAR n, VAR k) {
	var count=k;
	var temp1=n;
	var temp2;
	if (count<Z(0)) {
		count=Abs(k);
		temp1=Z(0)-temp1;
	}
	else if (count==Z(0)) count=Z(1);
	while (count>=Z(1)) {
		temp2=NextPrime(temp1);
		temp1=temp2;
		count=count-Z(1);
	}
	if (k<Z(0)) return Z::cast(Z(0)-temp2);
	else return Z::cast(temp2);
}

/** \brief 求整数\f$n\f$在\f$b\f$进制表示下的长度.
	\param n 整数.
	\param b 2 - 62.
	\return 整数在\f$b\f$进制表示下的长度.
*/
INT IntegerLength(VAR n, UINT b) {
	return mpz_sizeinbase(PTR(n),b);
}

/** \brief 位与运算.
	\param n, m 整数.
	\return \f$n\f$, \f$m\f$的位与运算.
*/
Z BitAnd(VAR n, VAR m) {
	Z r;
	mpz_and(PTR(r),PTR(n),PTR(m));
	return r;
}

/** \brief 位或运算.
	\param n, m 整数.
	\return \f$n\f$, \f$m\f$的位或运算.
*/
Z BitOr(VAR n, VAR m) {
	Z r;
	mpz_ior(PTR(r),PTR(n),PTR(m));
	return r;
}

/** \brief 位异或运算.
	\param n, m 整数.
	\return \f$n\f$, \f$m\f$的位异或运算.
*/
Z BitXor(VAR n, VAR m) {
	Z r;
	mpz_xor(PTR(r),PTR(n),PTR(m));
	return r;
}

/** \brief 位非运算.
	\param n 整数.
	\return \f$n\f$的位非运算.
*/
Z BitNot(VAR n) {
	Z r;
	mpz_com(PTR(r),PTR(n));
	return r;
}

/** \brief 设置\f$n\f$的\f$2^k\f$位为1.
	\param n 整数.
	\param k 非负整数.
	\return 设置\f$n\f$的\f$2^k\f$位为1.
*/
Z BitSet(VAR n, UINT k) {
	var r=n;
	mpz_setbit(PTR(r),k);
	return Z::cast(r);
}

/** \brief 设置\f$n\f$的\f$2^k\f$位为0.
\param n 整数.
\param k 非负整数.
\return 设置\f$n\f$的\f$2^k\f$位为0.
*/
Z BitClear(VAR n, UINT k) {
	var r=n;
	mpz_clrbit(PTR(r),k);
	return Z::cast(r);
}


/** \brief 求\f$n\f$的\f$2^k\f$位.
\param n, k 正整数.
\return \f$n\f$的\f$2^k\f$位.
*/
INT BitGet(VAR n, UINT k) {
	return mpz_tstbit(PTR(n),k);
}

/** \brief 将\f$n\f$左移\f$k\f$位.
	\param n, k 整数. 默认\f$k=1\f$.
	\return 将\f$n\f$左移\f$k\f$位. 当\f$k\f$为负数时为右移.
*/
Z BitShiftLeft(VAR n, INT k) {
	Z r;
	if (k<0) mpz_tdiv_q_2exp(PTR(r),PTR(n),-k);
	else mpz_mul_2exp(PTR(r),PTR(n),k);
	return r;
}

/** \brief 将\f$n\f$右移\f$k\f$位.
	\param n, k 整数. 默认\f$k=1\f$.
	\return 将\f$n\f$右移\f$k\f$位. 当\f$k\f$为负数时为左移.
*/
Z BitShiftRight(VAR n, INT k) {
	Z r;
	if (k<0) mpz_mul_2exp(PTR(r),PTR(n),-k);
	else mpz_tdiv_q_2exp(PTR(r),PTR(n),k);
	return r;
}

/** \brief 判断一个整数是否为奇数.
	\param n 整数.
	\retval 1 是.
	\retval 0 否.
*/
INT OddQ(VAR n) {
	return mpz_odd_p(PTR(n));
}

/** \brief 判断一个整数是否为偶数.
	\param n 整数.
	\retval 1 是.
	\retval 0 否.
*/
INT EvenQ(VAR n) {
	return mpz_even_p(PTR(n));
}

/** \brief 求整数的绝对值.
	\param n 整数
	\return n的绝对值.
*/
Z Abs(VAR n) {
	Z r;
	mpz_abs(PTR(r),PTR(n));
	return r;
}

/** \brief 整数开平方.
	\param n 整数.
	\return 平方根的整数部分\f$\lfloor\sqrt{n}\rfloor\f$.
	\note 使用Karatsuba平方根算法.
	\todo 补充文档.
*/
Z Sqrt(VAR n) {
	Z r;
	mpz_sqrt(PTR(r),PTR(n));
	return r;
}

/** \brief 判断一个整数是否为完全平方数.
	\param n 输入整数.
	\retval 1 是.
	\retval 0 否.
	\note 使用\f$\mathbb{Z}/k\mathbb{Z}\f$上平方数方法.
	以256，45，17，13，7为模, 比率99.25%, 使用汇编写的mpn_mod_34lsub1加速.
*/
INT SquareQ(VAR n) {
	return mpz_perfect_square_p(PTR(n));
}

/** \brief 判断\f$m\f$是否能整除\f$n\f$.
	\param n 整数.
	\param m 非零整数.
	\retval 1 是.
	\retval 0 否.
	\todo 判断m非零.
*/
INT Divisible(VAR n, VAR m) {
	if (mpz_fits_uint_p(PTR(m))) {
		return mpz_divisible_ui_p(PTR(n),mpz_get_ui(PTR(m)));
	}
	else {
		return mpz_divisible_p(PTR(n),PTR(m));
	}
}

/** \brief 判断\f$m\f$是否能整除\f$n\f$.
\param n 整数.
\param m 非零整数.
\retval 1 是.
\retval 0 否.
\todo 判断m非零.
*/
INT Divisible(VAR n, UINT m) {
	return mpz_divisible_ui_p(PTR(n),m);
}

/** \brief 求\f$m\f$除以\f$n\f$的商.
	\param m 整数.
	\param n 非零整数.
	\return 商\f$\lfloor m/n\rfloor\f$.
	\todo 增加非零判断, 增加非整数处理.
	\todo 需要文档补充.
*/
Z Quotient(VAR m, VAR n) {
	Z r;
	if (mpz_fits_uint_p(PTR(n))) {
		mpz_div_ui(PTR(r),PTR(m),mpz_get_ui(PTR(n)));
	}
	else {
		mpz_div(PTR(r),PTR(m),PTR(n));
	}
	return r;
}

/** \brief 求\f$m\f$除以\f$n\f$的商(已知\f$n\f$整除\f$m\f$).
	\param m 整数.
	\param n 非零整数.
	\return \f$m/n\f$.
*/
Z ExactQuotient(VAR m, UINT n) {
	Z r;
	mpz_divexact_ui(PTR(r),PTR(m),n);
	return r;
}
/** \brief 求\f$m\f$除以\f$n\f$的余数.
	\param m 整数.
	\param n 非零整数.
	\return 余数\f$m \bmod n\f$.
	\note
	\todo 增加非零判断, 增加非整数处理.
	\todo 需要文档补充.
*/
Z Mod(VAR m, VAR n) {
	Z r;
	if (mpz_fits_uint_p(PTR(n))) {
		mpz_mod_ui(PTR(r),PTR(m),mpz_get_ui(PTR(n)));
	}
	else {
		mpz_mod(PTR(r),PTR(m),PTR(n));
	}
	return r;
}
/** \brief 求整数的整数次幂.
	\param a 整数.
	\param b 非负整数.
	\return \f$a^b\f$.
	\note 使用自左向右的二进方法.
	\todo a=0
*/
Z Power(VAR a, UINT b) {
	Z r;
	if (mpz_fits_uint_p(PTR(a))) {
		mpz_ui_pow_ui(PTR(r),mpz_get_ui(PTR(a)),b);
	}
	else {
		mpz_pow_ui(PTR(r),PTR(a),b);
	}
	return r;
}

/** \brief 求\f$a^b \bmod m\f$.
	\param a,b 整数.
	\param m 非零整数.
	\return \f$a^b \bmod m\f$.
	\note 模幂mpz_powm采用\f$2^k\f$进窗口方法，\f$k\f$根据指数选择.
	\note 对小模使用了Montgomery约化过程(只针对奇数模),
	并且对POWM_THRESHOLD以上模使用带余除法.
	\todo a=0.
*/

Z PowerMod(VAR a, VAR b, VAR m) {
	Z r;
	if (mpz_fits_uint_p(PTR(b))) {
		mpz_powm_ui(PTR(r),PTR(a),mpz_get_ui(PTR(b)),PTR(m));
	}
	else {
		mpz_powm(PTR(r),PTR(a),PTR(b),PTR(m));
	}
	return r;
}

/** \brief 求\f$a\f$模\f$n\f$的逆.
	\param a, n 整数.
	\return \f$a\f$模\f$n\f$的逆.
	\todo 互素判定.
*/
Z InverseMod(VAR a, VAR n) {
	Z r;
	mpz_invert(PTR(r),PTR(a),PTR(n));
	return r;
}

/** \brief 求两个整数的最大公因子.
\param n1, n2 整数.
\return 最大公因子.
\note mpz_gcd对较小数据使用Binary GCD,
对超过GCD_ACCEL_THRESHOLD的数据使用Jebelean-Weber-Sorenson加速算法.
*/
Z GCD(VAR n1, VAR n2) {
	Z r;
	if (mpz_fits_uint_p(PTR(n2))) {
		mpz_gcd_ui(PTR(r),PTR(n1),mpz_get_ui(PTR(n2)));
	}
	else {
		if (mpz_fits_uint_p(PTR(n1))) {
			mpz_gcd_ui(PTR(r),PTR(n2),mpz_get_ui(PTR(n1)));
		}
		else {
		mpz_gcd(PTR(r),PTR(n1),PTR(n2));
		}
	}
	return r;
}

/** \brief 求\f$n_i\f$的最大公因子.
	\param n \f$\{n_1,n_2,\ldots\}\f$.
	\return \f$n_i\f$的最大公因子.
*/
Z GCD(LIST n) {
	var t=n[0];
	for (INT i=1;i<=n.size()-1;i++)
	{
		t=GCD(t,n[i]);
		if (Z::cast(t)==1) break;
	}
	return Z::cast(t);
}

/** \brief 求两个整数的最小公倍数.
\param n1, n2 整数.
\return 最小公倍数.
*/
Z LCM(VAR n1, VAR n2) {
	Z r;
	if (mpz_fits_uint_p(PTR(n2))) {
		mpz_lcm_ui(PTR(r),PTR(n1),mpz_get_ui(PTR(n2)));
	}
	else {
		if (mpz_fits_uint_p(PTR(n1))) {
			mpz_lcm_ui(PTR(r),PTR(n2),mpz_get_ui(PTR(n1)));
		}
		else {
			mpz_lcm(PTR(r),PTR(n1),PTR(n2));
		}
	}
	return r;
}

/** \brief 求\f$n_i\f$的最小公倍数.
\param n \f$\{n_1,n_2,\ldots\}\f$.
\return \f$n_i\f$的最小公倍数.
*/
Z LCM(LIST n) {
	var t=n[0];
	for (INT i=1;i<=n.size()-1;i++)
	{
		t=LCM(t,n[i]);
	}
	return Z::cast(t);
}

/** \brief 求两个整数的扩展最大公因子.
\param n1, n2 整数.
\return 扩展最大公因子\f$\{g,\{r_1,r_2\}\}\f$, 满足\f$g=r_1n_1+r_2n_2\f$.
\note 在GCDEXT_THRESHOLD以上使用Lehmer加速算法.
\todo 扩展到任意个.
*/
list ExtendedGCD(VAR n1, VAR n2) {
	Z g=GCD(n1,n2);
	Z r1,r2;
	mpz_gcdext(PTR(g),PTR(r1),PTR(r2),PTR(n1),PTR(n2));
	return list(g,list(r1,r2));
}

/** \brief 判断两个整数是否互素.
\param n1,n2 整数.
\retval 1 是.
\retval 0 否.
*/
INT CoprimeQ(VAR n1, VAR n2) {
	return (GCD(n1, n2)==1);
}
/** \brief 求二次无理数的连分数展式前\f$n\f$项.
	\param d 非负整数且非完全平方.
	\param P 整数.
	\param Q 非零整数.
	\param n 正整数.
	\return \f$(\sqrt{d}+P)/Q\f$的连分数展式的前\f$n\f$项\f$\{a_0,a_1,\ldots,a_{n-1}\}\f$.
	\note 使用全整数运算的递推关系.
*/
list ContinuedFraction(VAR d, VAR P, VAR Q, UINT n) {
	list a;
	var t, r, D, s;
	if (Divisible(d-P*P,Q)) {
		D=d;
		r=P;
		t=Q;
	}
	else {
		D=d*Q*Q;
		r=P*Integer::Abs(Q);
		t=Q*Integer::Abs(Q);
	}
	s=(D-r*r)/t;
	var p,q;
	for (INT i=0;i<=n-1;i++)
	{
		a.push((Sqrt(D)+r)/t);
		p=a[i]*t-r;
		q=a[i]*(r-p)+s;
		s=t;
		t=q;
		r=p;
		if (Z::cast(t)==0) break;
	}
	return a;
}

/** \brief 求二次无理数的连分数展式.
	\param d 非负整数且非完全平方.
	\param P 整数.
	\param Q 非零整数.
	\return \f$\{\{a_0,a_1,\ldots,a_m\},\{a_{m+1},\ldots,a_{m+l}\}\}\f$.
	\f$(\sqrt{d}+P)/Q\f$的连分数展式. 表中的第一项为非循环部分, 第二项为循环部分.
	\note 先使用Floyd算法求出非循环部分, 再求出最小循环周期.
*/
list ContinuedFraction(VAR d, VAR P, VAR Q) {
	list p,q,a;
	var D;
	if (Divisible(d-P*P,Q)) {
		D=d;
		p.push(P);
		q.push(Q);
	}
	else {
		D=d*Q*Q;
		p.push(P*Integer::Abs(Q));
		q.push(Q*Integer::Abs(Q));
	}
	INT i=0;
	INT j=0;
	INT l=0;
	while (1) {
		a.push((Sqrt(D)+p[i])/q[i]);
		p.push(a[i]*q[i]-p[i]);
		if (i==0) {
			q.push(a[i]*(p[i]-p[i+1])+(D-p[0]*p[0])/q[0]);
		}
		else {
			q.push(a[i]*(p[i]-p[i+1])+q[i-1]);
		}

		if (Z::cast(q[i+1])==0) return list(a,list());
		if (i%2==0 && p[i+1]==p[i/2] && q[i+1]==q[i/2]) {
			l=i/2+1;
			j=i/2-1;
			while (j>=0 && p[j]==p[j+l] && q[j]==q[j+l]) {
				j--;
			}
			j++;
			for (INT k=1;k<=l;k++) {
				if (l%k==0 && p[j]==p[j+k] && q[j]==q[j+k]) {
					l=k;
					break;
				}
			}
			break;
		}
		i++;
	}
	return list(a.mid(0,j),a.mid(j,j+l));
}

/** \brief 求二次无理数的渐进分数.
	\param d 非负整数且非完全平方.
	\param P 整数.
	\param Q 非零整数.
	\param n 正整数.
	\return \f$(\sqrt{d}+P)/Q\f$的前\f$n\f$个渐进分数组成的表.
	\note 使用递推关系.
*/
list Convergents(VAR d, VAR P, VAR Q, UINT n) {
	list h,k;
	h.push(Z(0));
	h.push(Z(1));
	k.push(Z(1));
	k.push(Z(0));
	var h0=Z(0),h1=Z(1),h2,k0=Z(1),k1=Z(0),k2;
	list a=ContinuedFraction(d,P,Q,n);
	list r;
	for (INT i=0;i<=n-1;i++) {
		h2=a[i]*h1+h0;
		k2=a[i]*k1+k0;
		h0=h1;
		h1=h2;
		k0=k1;
		k1=k2;
		r.push(Rational::NewZ(h2,k2));
	}
	return r;
}

/** \brief 求解线性同余方程组\f$x\equiv x_i\pmod{m_i}\f$.
\param x 表\f$\{x_1,\ldots,x_n\}\f$.
\param m 表\f$\{m_1,\ldots,m_n\}\f$.
\return 线性同余方程组最小正整数解.
\note 使用改进的中国剩余定理算法.
\todo 测试效率.
*/
Z ChineseRemainder(LIST x, LIST m) {
	UINT n=m.size();
	list y(n),p(n),u(n);
	y[0]=Mod(x[0],m[0]);
	var t=Z(1);
	for (INT i=1;i<=n-1;i++) {
		t=t*m[i-1];
		p[i]=Mod(t,m[i]);
		u[i]=list::cast(ExtendedGCD(p[i],m[i])[1])[0];
		var s=y[i-1];
		for (INT j=i-2;j>=0;j--) {
			s=s*m[j]+y[j];
		}
		y[i]=Mod((x[i]-s)*u[i],m[i]);
	}
	var r=y[n-1];
	for (INT i=n-2;i>=0;i--) {
		r=r*m[i]+y[i];
	}
	return Z::cast(r);
}

/** \brief 求Legendre符号\f$\legendre{n}{m}\f$.
\param n 整数.
\param m 正奇素数.
\return Legendre符号\f$\legendre{n}{m}\f$.
\note 使用二进方法.
\todo 增加正奇素数判断.
*/
Z LegendreSymbol(VAR n, VAR m) {
	return Z(mpz_legendre(PTR(n),PTR(m)));
}
/** \brief 求Jacobi符号\f$\legendre{n}{m}\f$.
\param n 整数.
\param m 正奇数.
\return Jacobi符号\f$\legendre{n}{m}\f$.
\note 使用二进方法.
\todo 增加正奇数判断.
*/
Z JacobiSymbol(VAR n, VAR m) {
	return Z(mpz_legendre(PTR(n),PTR(m)));
}

/** \brief 求Kronecker符号\f$\legendre{n}{m}\f$.
\param n,m 整数.
\return Kronecker符号\f$\legendre{n}{m}\f$.
\note 使用二进方法.
*/
Z KroneckerSymbol(VAR n, VAR m) {
	return Z(mpz_kronecker(PTR(n),PTR(m)));
}

/** \brief 求Euler函数\f$\varphi(n)\f$.
	\param n 整数.
	\return Euler函数\f$\varphi(n)\f$.
*/
Z EulerPhi(VAR n) {
	if (n==Z(0)) return Z(0);
	if (Abs(n)==Z(1)) return Z(1);
	list s=FactorInteger(Abs(n));
	var m=Z(1);
	for (UINT i=0;i<s.size();i++)
	{
		m=m*Power(list::cast(s[i])[0],mpz_get_ui(PTR(list::cast(s[i])[1]))-1)*(list::cast(s[i])[0]-Z(1));
	}
	return Z::cast(m);
}

/** \brief 求Moebius函数\f$\mU(n)\f$.
\param n 整数.
\return Moebius函数\f$\mU(n)\f$.
*/
Z MoebiusMu(VAR n) {
	list r;
	var m=Abs(n);
	if (m==Z(0)) { return Z(0); }
	if (m==Z(1)) { return Z(1); }
	/* 直接判断是否有重素因子 */
	try {
		r=FactorInteger(m,1);
	}
	catch (int e) {
		if (e==MoebiusMu_0)
			return Z(0);
		else throw(e);
	}
	if (r.size()%2==0) return Z(1);
	else return Z(-1);
}

/** \cond */
inline var first(VAR x) {return list::cast(x)[0];}
inline var second(VAR x) {return list::cast(x)[1];}
/** \endcond */

/** \brief 求\f$n\f$的所有因子.
	\param n 非零整数.
	\return \f$n\f$的所有因子.
	\todo 判断零.
*/
list Divisors(VAR n) {
	if (Abs(n)==Z(1)) return list().push(Z(1));
	list f=FactorInteger(Abs(n));
	list p=list::map(first,f);	/* 因子 */
	list e=list::map(second,f);	/* 次数 */
	list r;
	UINT t,l;
	r.push(Z(1));
	for (UINT i=0;i<p.size();i++) {
		t=0;
		l=r.size();
		for (UINT k=0;k<Z::cast(e[i]);k++)
		{
			for (UINT j=t;j<l;j++)
			{
				r.push(r[j]*p[i]);
			}
			t=l;
			l=r.size();
		}
	}
	std::sort(r.begin(),r.end(),less);
	return r;
}

/** \brief 求\f$n\f$的不同素因子个数\f$\nu(n)\f$.
\param n 非零整数.
\return \f$n\f$的不同素因子个数\f$\nu(n)\f$.
\todo 判断零.
*/
Z PrimeNu(VAR n) {
	if (Abs(n)==Z(1)) return Z(0);
	return Z(FactorInteger(Abs(n)).size());
}


}
}
}
