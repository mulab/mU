#pragma once
#include <mU/Interface.h>
#include <mU0/Kernel.h>
#ifdef _MSC_VER
#pragma comment(lib,"mU0")
#endif

#undef API
#ifdef _MSC_VER
#if defined(mu0link_EXPORTS) || defined(MU0LINK_EXPORTS)
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#else
#define API
#endif

namespace mU {
API mU0::var to_mu0(sym);
API mU0::var to_mu0(const Key&);
API mU0::var to_mu0(const Object&);
API mU0::var to_mu0(const Tuple&);
API mU0::var to_mu0(const var&);
API var from_mu0(mU0::Var);
}
