#pragma once
#include "var.h"

namespace maTHmU {
namespace Interface {
//////////////////////////////////////
#define DECL_FACE(op,name)\
VAR_API op & name(UINT);\
VAR_API op & name(VAR);

#define REG(op,type) Interface::op(type) = op
//////////////////////////////////////
DECL_FACE(N_OP,Zero)
DECL_FACE(N_OP,One)
DECL_FACE(UN_OP,New)
DECL_FACE(UN_OP,Print)
DECL_FACE(UN_OP,Abs)
DECL_FACE(UN_OP,Neg)
DECL_FACE(BIN_OP,Add)
DECL_FACE(BIN_OP,Sub)
DECL_FACE(BIN_OP,Mul)
DECL_FACE(BIN_OP,Div)
DECL_FACE(BIN_OP,Gcd);
DECL_FACE(BIN_OP,Lcm);
DECL_FACE(CMP_OP,Cmp)
DECL_FACE(SC_OP,Sc)
//////////////////////////////////////
}
}
