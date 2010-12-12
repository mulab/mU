#pragma once
#include <mU/Interface.h>
#include <ecl/ecl.h>
#ifdef _MSC_VER
#pragma comment(lib,"ecl.lib")
#endif

#undef API
#ifdef _MSC_VER
#if defined(linkecl_EXPORTS) || defined(LINKECL_EXPORTS)
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#else
#define API
#endif

namespace mU {
/*#define symbolp ECL_SYMBOLP
#define numberp ecl_numberp
inline bool integerp(cl_object obj) {
    return cl_integerp(obj) != Cnil;
}
inline bool rationalp(cl_object obj) {
    return cl_rationalp(obj) != Cnil;
}
inline bool floatp(cl_object obj) {
	return cl_floatp(obj) != Cnil;
}
#define equalp ecl_equalp
#define stringp ecl_stringp
#define fixnump FIXNUMP
#define characterp CHARACTERP
#define nullp Null
#define listp LISTP
#define consp CONSP
#define atomp ATOM
*/
API extern cl_object 
read_from_string_clobj, safe_eval_clobj, safe_apply_clobj, safe_funcall_clobj;
#define ecl_nvalues NVALUES
#define ecl_values VALUES
API cl_object ecl_safe_eval(cl_object form);
API cl_object ecl_safe_funcall(cl_object func, cl_object arg);
API cl_object ecl_safe_apply(cl_object func, cl_object args);
API cl_object ecl_safe_read_cstring(const char* s);
API cl_object ecl_safe_eval_cstring(const char* s);
API cl_object to_ecl(sym);
API cl_object to_ecl(const Key&);
API cl_object to_ecl(const Object&);
API cl_object to_ecl(const Tuple&);
API cl_object to_ecl(const var&);
API var from_ecl(cl_object);
}
