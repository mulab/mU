#pragma once
#include "objects.h"

namespace maTHmU {
namespace Modules {
namespace NumberTheory {
//////////////////////////////////////
VOID Init();

VOID Randomize();
VOID SeedRandom(VAR n);
VOID SeedRandom();
Z RandomInteger(VAR n);
Z RandomPrime(VAR n);
Z NextPrime(VAR n);
Z NextPrime(VAR n, VAR k);

INT IntegerLength(VAR n, UINT b);

Z BitAnd(VAR n, VAR m);
Z BitOr(VAR n, VAR m);
Z BitXor(VAR n, VAR m);
Z BitNot(VAR n);
Z BitSet(VAR n, UINT k);
Z BitClear(VAR n, UINT k);
INT BitGet(VAR n, UINT k);

Z BitShiftLeft(VAR n, INT k=1);
Z BitShiftRight(VAR n, INT k=1);

INT OddQ(VAR n);
INT EvenQ(VAR n);
Z Abs(VAR n);

Z Sqrt(VAR n);
INT SquareQ(VAR n);

Z GCD(VAR n1, VAR n2);
Z GCD(LIST n);
list ExtendedGCD(VAR n1, VAR n2);

Z LCM(VAR n1, VAR n2);
Z LCM(LIST n);

INT CoprimeQ(VAR n1, VAR n2);

INT Divisible(VAR n, VAR m);
Z Quotient(VAR m, VAR n);

Z LegendreSymbol(VAR n, VAR m);
Z JacobiSymbol(VAR n, VAR m);
Z KroneckerSymbol(VAR n, VAR m);

Z MoebiusMu(VAR n);
Z EulerPhi(VAR n);
Z PrimeNu(VAR n);

Z Mod(VAR m, VAR n);
Z Power(VAR a, UINT b);
Z PowerMod(VAR a, VAR b, VAR m);
Z InverseMod(VAR a, VAR n);

list ContinuedFraction(VAR d, VAR P, VAR Q, UINT n);
list ContinuedFraction(VAR d, VAR P, VAR Q);
list Convergents(VAR d, VAR P, VAR Q, UINT n);

Z ChineseRemainder(LIST x, LIST m);

list FactorInteger(VAR n);
list Divisors(VAR n);

INT PrimeQ(VAR n);
//////////////////////////////////////
}
}
}
