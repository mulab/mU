#include "objects/interface.h"
#include <vector>

namespace maTHmU {
namespace Interface {
//////////////////////////////////////
#define DEF_FACE(op,name)\
namespace { std::vector<op> name##Factory(0x100); }\
op & name(UINT x) { return name##Factory[x >> 24]; }\
op & name(VAR x) { return name(GetType(x)); }

//////////////////////////////////////
DEF_FACE(N_OP,Zero)
DEF_FACE(N_OP,One)
DEF_FACE(UN_OP,New)
DEF_FACE(UN_OP,Print)
DEF_FACE(UN_OP,Abs)
DEF_FACE(UN_OP,Neg)
DEF_FACE(BIN_OP,Add)
DEF_FACE(BIN_OP,Sub)
DEF_FACE(BIN_OP,Mul)
DEF_FACE(BIN_OP,Div)
DEF_FACE(BIN_OP,Gcd)
DEF_FACE(BIN_OP,Lcm)
DEF_FACE(CMP_OP,Cmp)
DEF_FACE(SC_OP,Sc)
//////////////////////////////////////
}
}
