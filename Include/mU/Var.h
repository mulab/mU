#pragma once
#include <cctype>
#include <cwctype>
#include <cwchar>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <cmath>
#include <ctime>
#include <locale>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <set>
#include <map>
#include <deque>
#include <queue>
#include <list>
#include <algorithm>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <gmp.h>

namespace mU {
struct var_t;
typedef var_t* Var;
typedef unsigned long uint;
typedef signed long sint;
typedef wchar_t wchar;
using std::string;using std::wstring;using std::istream;using std::wistream;
using std::ostream;using std::wostream;using std::iostream;using std::wiostream;
using std::istringstream;using std::wistringstream;
using std::ostringstream;using std::wostringstream;
using std::stringstream;using std::wstringstream;
using std::ifstream;using std::wifstream;
using std::ofstream;using std::wofstream;
using std::fstream;using std::wfstream;
using std::cin;using std::wcin;using std::cout;using std::wcout;
using std::cerr;using std::wcerr;using std::ios;using std::wios;
}

#ifdef _MSC_VER
#include <hash_set>
#include <hash_map>
#pragma comment(lib,"../../Lib/gmp.lib")
#ifdef MU_EXPORTS
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#pragma comment(lib,"mU")
#endif
#else
#include <ext/hash_set>
#include <ext/hash_map>
namespace __gnu_cxx {
template <>
struct hash<mU::Var>
{
    size_t operator()(const mU::Var &x) const
    {
        return (size_t)x;
    }
};
template <>
struct hash<std::string>
{
    size_t operator()(const std::string &x) const
    {
        return __stl_hash_string(x.c_str());
    }
};
template <>
struct hash<std::wstring>
{
    size_t operator()(const std::wstring &x) const
    {
        return __stl_hash_string((const char*)x.c_str());
    }
};
}
namespace stdext = __gnu_cxx;
#define DLL
#endif
#define API DLL extern

namespace mU {
//////////////////////////////////////
/*!
 * \brief
 * 变量基类
 *
 * \remarks
 * 默认采用句柄型复制控制，可覆盖为值型复制控制。
 */
#define TYPE(x) type_##x
enum type_t
{
	type_obj,type_int,type_rat,type_flt,
	type_str,type_sym,type_vec,type_ex
};
const size_t TYPE_SIZE = 010;
const size_t TYPE_MASK = TYPE_SIZE - 1;

struct var_t
{
	size_t id;
	var_t() : id(0) {}
	var_t(type_t t) : id(t) {}
	Var clone() { id += TYPE_SIZE; return this; }
	Var copy(Var x) { id += TYPE_SIZE; if(x) x->destroy(); return this; }
	void destroy() { id -= TYPE_SIZE; if(id < TYPE_SIZE) delete this; }
	virtual ~var_t() {}
};
inline size_t Type(Var x)
{
	return static_cast<type_t>((x->id) & TYPE_MASK);
}

/*!
 * \brief
 * 句柄
 *
 * \remarks
 * 为指针Var提供自动复制控制，便于标准库容器使用。
 */
class var;
typedef const var& VAR;
class var
{
	Var ptr;
public:
	var() : ptr(0) {}
	var(Var x) : ptr(x ? x->clone() : 0) {}
	var(VAR x) : ptr(x.ptr ? x.ptr->clone() : 0) {}
	Var operator =(Var x) { return ptr = x ? x->copy(ptr) : 0; }
	Var operator =(VAR x) { return ptr = x.ptr ? x.ptr->copy(ptr) : 0; }
	~var() { if(ptr) ptr->destroy(); }
	operator Var() const { return ptr; }
	Var get() const { return ptr; }
	Var operator ->() const { return ptr; }
};
//////////////////////////////////////
/*!
* \brief
* 对象
*
* \remarks
* 为其他对象提供自动内存管理。
*/
struct obj_t : var_t
{
//	static void ruin(Var x) { delete (obj_t*)x; }
	obj_t() : var_t(TYPE(obj)) {}
	virtual ~obj_t() {}
	virtual var tag() { return this; }
	virtual var normal() { return this; }
	virtual void print(wostream &f) { f << this; }
};
inline bool ObjQ(Var x) { return Type(x) == TYPE(obj); }
/*!
* \brief
* 整数
*
* \remarks
* mpz_t
*/
struct int_t : var_t
{
//	static void ruin(Var x) { mpz_clear(INT_REP(x)); delete (int_t*)x; }
	int_t() : var_t(TYPE(int)) { mpz_init(rep); }
	int_t(Var x);
	int_t(uint x) : var_t(TYPE(int)) { mpz_init_set_ui(rep,x); }
	int_t(sint x) : var_t(TYPE(int)) { mpz_init_set_si(rep,x); }
	int_t(double x) : var_t(TYPE(int)) { mpz_init_set_d(rep,x); }
	int_t(const char *x,int y) : var_t(TYPE(int)) { mpz_init_set_str(rep,x,y); }
	virtual ~int_t();
	typedef mpz_t rep_t;
	rep_t rep;
};
inline mpz_t &INT_REP(var_t *x)
{
	return dynamic_cast<int_t*>(x)->rep;
}
inline int_t::int_t(Var x) : var_t(TYPE(int))
{
	mpz_init_set(rep,INT_REP(x));
}
inline int_t::~int_t()
{
	mpz_clear(INT_REP(this));
}
inline bool IntQ(Var x) { return Type(x) == TYPE(int); }
inline var Int() { return new int_t; }
template <typename T>
inline var Int(const T& x) { return new int_t(x); }
inline var Int(const char *x, int y) { return new int_t(x,y); }
inline int_t::rep_t& CInt(Var x) { return INT_REP(x); }

/*!
* \brief
* 有理数
*
* \remarks
* mpq_t
*/
struct rat_t : var_t
{
//	static void ruin(Var x) { mpq_clear(RAT_REP(x)); delete (rat_t*)x; }
	virtual ~rat_t() { mpq_clear(rep); }
	rat_t() : var_t(TYPE(rat)) { mpq_init(rep); }
	typedef mpq_t rep_t;
	rep_t rep;
};
inline rat_t::rep_t &RAT_REP(Var x)
{
	return dynamic_cast<rat_t*>(x)->rep;
}
inline rat_t::rep_t &RAT_REP(const var &x)
{
	return dynamic_cast<rat_t*>(x.get())->rep;
}
inline bool RatQ(Var x) { return Type(x) == TYPE(rat); }
inline var Rat() { return new rat_t; }
inline var Rat(Var x)
{
	var r = Rat();
	mpq_set(RAT_REP(r),RAT_REP(x));
	return r;
}
inline var Rat(uint x, uint y)
{
	var r = Rat();
	mpq_set_ui(RAT_REP(r),x,y);
	return r;
}
inline var Rat(sint x, uint y)
{
	var r = Rat();
	mpq_set_si(RAT_REP(r),x,y);
	return r;
}
inline var Rat(double x)
{
	var r = Rat();
	mpq_set_d(RAT_REP(r),x);
	return r;
}
inline var Rat(const char *x, int y)
{
	var r = Rat();
	mpq_set_str(RAT_REP(r),x,y);
	return r;
}
inline rat_t::rep_t& CRat(Var x) { return RAT_REP(x); }

/*!
* \brief
* 浮点数
*
* \remarks
* mpf_t
*/
struct flt_t : var_t
{
//	static void ruin(Var x) { mpf_clear(FLT_REP(x)); delete (flt_t*)x; }
	flt_t() : var_t(TYPE(flt)) { mpf_init(rep); }
	flt_t(Var x);
	flt_t(uint x) : var_t(TYPE(flt)) { mpf_init_set_ui(rep,x); }
	flt_t(sint x) : var_t(TYPE(flt)) { mpf_init_set_si(rep,x); }
	flt_t(double x) : var_t(TYPE(flt)) { mpf_init_set_d(rep,x); }
	flt_t(const char *x,int y) : var_t(TYPE(flt)) { mpf_init_set_str(rep,x,y); }
	virtual ~flt_t() { mpf_clear(rep); }
	typedef mpf_t rep_t;
	rep_t rep;
};
inline flt_t::rep_t &FLT_REP(Var x)
{
	return dynamic_cast<flt_t*>(x)->rep;
}
inline flt_t::flt_t(Var x) : var_t(TYPE(flt))
{
	mpf_init_set(rep,FLT_REP(x));
}
inline bool FltQ(Var x) { return Type(x) == TYPE(flt); }
inline var Flt() { return new flt_t; }
template <typename T>
inline var Flt(const T& x) { return new flt_t(x); }
inline var Flt(const char *x, int y) { return new flt_t(x,y); }
inline flt_t::rep_t& CFlt(Var x) { return FLT_REP(x); }

/*!
* \brief
* 字符串
*
* \remarks
* wstring
*/
struct str_t : var_t
{
//	static void ruin(Var x) { delete (str_t*)x; }
	str_t() : var_t(TYPE(str)) {}
	str_t(Var x);
	str_t(const wchar *x) : var_t(TYPE(str)),rep(x) {}
	str_t(const wstring &x) : var_t(TYPE(str)),rep(x) {}
	typedef wstring rep_t;
	rep_t rep;
};
inline str_t::rep_t &STR_REP(Var x)
{
	return dynamic_cast<str_t*>(x)->rep;
}
inline str_t::str_t(Var x) : var_t(TYPE(str)),rep(STR_REP(x)) {}
inline bool StrQ(Var x) { return Type(x) == TYPE(str); }
inline var Str() { return new str_t; }
template <typename T>
inline var Str(const T& x) { return new str_t(x); }
inline str_t::rep_t& CStr(Var x) { return STR_REP(x); }

/*!
* \brief
* 符号
*
* \remarks
* 从符号表中构造。
*/
struct tab_t : obj_t
{
	typedef stdext::hash_map<wstring,var> rep_t;
	rep_t rep;
};
inline tab_t::rep_t &TAB_REP(Var x)
{
	return dynamic_cast<tab_t*>(x)->rep;
}
inline var Tab() { return new tab_t; }
inline tab_t::rep_t& CTab(Var x) { return TAB_REP(x); }

struct sym_t : var_t
{
//	static void ruin(Var x) { delete (sym_t*)x; }
	sym_t(const wchar *x,Var y) : var_t(TYPE(sym)),rep(x,y) {}
	typedef std::pair<const wchar*,Var> rep_t;
	rep_t rep;
};
inline sym_t::rep_t &SYM_REP(Var x)
{
	return dynamic_cast<sym_t*>(x)->rep;
}
inline bool SymQ(Var x) { return Type(x) == TYPE(sym); }
template <typename T>
inline var Sym(const T& x, Var y)
{
	std::pair<tab_t::rep_t::iterator,bool>
		iter = CTab(y).insert(
		tab_t::rep_t::value_type(x,var()));
	if(iter.second)
		iter.first->second = new sym_t(
		iter.first->first.c_str(),y);
	return iter.first->second;
}
inline const wchar*& Name(Var x) { return SYM_REP(x).first; }
inline Var& Context(Var x) { return SYM_REP(x).second; }
inline sym_t::rep_t& CSym(Var x) { return SYM_REP(x); }

/*!
* \brief
* 向量
*
* \remarks
* 包装标准库vector以形成广义表嵌套结构，表达式以其为主体。
*
* \see
* ex_t
*/
struct vec_t : var_t
{
//	static void ruin(Var x) { delete (vec_t*)x; }
	vec_t() : var_t(TYPE(vec)) {}
	vec_t(size_t n) : var_t(TYPE(vec)),rep(n) {}
	typedef std::vector<var> rep_t;
	rep_t rep;
};
inline vec_t::rep_t &VEC_REP(Var x)
{
	return dynamic_cast<vec_t*>(x)->rep;
}
inline bool VecQ(Var x) { return Type(x) == TYPE(vec); }
inline var Vec() { return new vec_t; }
inline var Vec(size_t n) { return new vec_t(n); }
inline var Vec(Var a)
{
	vec_t *r = new vec_t(1);
	r->rep[0] = a;
	return r;
}
inline var Vec(Var a, Var b)
{
	vec_t *r = new vec_t(2);
	r->rep[0] = a;
	r->rep[1] = b;
	return r;
}
inline var Vec(Var a, Var b, Var c)
{
	vec_t *r = new vec_t(3);
	r->rep[0] = a;
	r->rep[1] = b;
	r->rep[2] = c;
	return r;
}
inline var Vec(Var a, Var b, Var c, Var d)
{
	vec_t *r = new vec_t(4);
	r->rep[0] = a;
	r->rep[1] = b;
	r->rep[2] = c;
	r->rep[3] = d;
	return r;
}
inline var& At(Var x, size_t y) { return VEC_REP(x)[y]; }
inline size_t Size(Var x) { return VEC_REP(x).size(); }
inline void Resize(Var x, size_t y) { VEC_REP(x).resize(y); }
inline void Reserve(Var x, size_t y) { VEC_REP(x).reserve(y); }
inline void Push(Var x, Var y) { VEC_REP(x).push_back(y); }
inline void Pop(Var x) { VEC_REP(x).pop_back(); }
inline var& Top(Var x) { return VEC_REP(x).back(); }
inline vec_t::rep_t& CVec(Var x) { return VEC_REP(x); }

/*!
* \brief
* 表达式
*
* \remarks
* 包括head与向量构成的body两部分，注意head也可能为表达式。
*
* \see
* vec_t
*/
struct ex_t : var_t
{
//	static void ruin(Var x) { delete (ex_t*)x; }
	virtual ~ex_t() {}
	ex_t(Var x, Var y) : var_t(TYPE(ex)),rep(x,y) {}
	typedef std::pair<var,var> rep_t;
	rep_t rep;
};
inline ex_t::rep_t &EX_REP(Var x)
{
	return dynamic_cast<ex_t*>(x)->rep;
}
inline var& Head(Var x) { return EX_REP(x).first; }
inline var& Body(Var x) { return EX_REP(x).second; }
inline bool ExQ(Var x) { return Type(x) == TYPE(ex); }
inline bool ExQ(Var x, Var y) { return ExQ(x) && Head(x) == y; }
inline var Ex(Var x, Var y) { return new ex_t(x,y); }
inline size_t Len(Var x) { return CVec(Body(x)).size(); }
inline var& Left(Var x) { return CVec(Body(x)).front(); }
inline var& Right(Var x) { return CVec(Body(x)).back(); }
inline var& Part(Var x, size_t y) { return CVec(Body(x))[y]; }
inline ex_t::rep_t& CEx(Var x) { return EX_REP(x); }

inline bool NumQ(Var x)
{
	switch(Type(x))
	{
	case TYPE(int):
	case TYPE(rat):
	case TYPE(flt):
		return true;
    default:
        return false;
	}
}
inline bool AtomQ(Var x)
{
	switch(Type(x))
	{
	case TYPE(vec):
	case TYPE(ex):
		return false;
    default:
        return true;
	}
}
//////////////////////////////////////
}
