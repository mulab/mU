#pragma once
#include "objects.h"

namespace maTHmU {
namespace Modules {
namespace NumberTheory {

#define MoebiusMu_0 0

Z ExactQuotient(VAR m, UINT n);
INT Divisible(VAR n, UINT m);

VOID FactorDivide(list& factors,list& remain,INT mode);
VOID FactorCrack(list& factors,list& remain,INT mode);

list FactorInteger(VAR N, INT mode);
}
}
}
