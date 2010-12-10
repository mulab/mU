#pragma once
#include "Common.h"

#undef API
#ifdef _MSC_VER
#ifdef INTERFACE_EXPORTS
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#else
#define API
#endif

namespace mU {
API string cstr(wcs);
inline string cstr(const wstring& x) {
	return cstr(x.c_str());
}
API string cstr(const var&);
API string cpath(const char*);
API void* cload(const char*);
API void* cnoload(const char*);
API void cunload(void*);
API void* csym(void*, const char*);
API bool cinstall(Kernel&, const char*);
API bool cuninstall(Kernel&, const char*);
API string cname(sym);
API string csig(const Tuple&);
API bool cassign(Kernel&, void*, sym);
API bool cvalue(Kernel&, void*, sym);
API bool cmethod(Kernel&, void*, const Tuple&);
API bool cinterface(Kernel&, void*, const Tuple&);

/*!
* \brief
* C赋值函数
*
* \remarks
* CAssign
*/
class CAssign : public Assign {
public:
    typedef bool(*Ptr)(Kernel&, const Tuple&, const var&);
    CAssign(Ptr $ptr) : ptr($ptr) {}
    virtual bool operator()(Kernel& k, const Tuple& x, const var& y) {
        return ptr(k, x, y);
    }
protected:
    Ptr ptr;
};
#define CASSIGN(p,f) p##_##f##_##Assign

/*!
* \brief
* C求值函数
*
* \remarks
* CValue
*/
struct CValue : public Value {
public:
    typedef void(*Ptr)(Kernel&, var&, Tuple&);
    CValue(Ptr $ptr) : ptr($ptr) {}
    virtual var operator()(Kernel& k, Tuple& x) {
        // 直接返回var则warning c4190
        var r = &x;
        ptr(k, r, x);
        return r;
    }
protected:
    Ptr ptr;
};
#define CVALUE(p,f) p##_##f##_##Value

/*!
* \brief
* C方法
*
* \remarks
* CMethod
*/
class CMethod : public Method {
public:
    //(Kernel& k, var& r, Tuple& x, var self, sym local)
    typedef void(*Ptr)(Kernel&, var&, Tuple&, var, sym);
    CMethod(Ptr $ptr) : ptr($ptr) {}
    API virtual var operator()(Kernel& k);
protected:
    Ptr ptr;
};
#define CMETHOD(p,f,sig) p##_##f##_##Method##sig

class CMatch;
API Match* matchC(Kernel&, const var&);
API Match* ruleC(Kernel&, const var&, const var&);
API Match* testC(Kernel&, const var&, const var&);
API Match* listC(Kernel&, const Tuple&);
API bool set(Kernel&, const Tuple&, const var&);
}
