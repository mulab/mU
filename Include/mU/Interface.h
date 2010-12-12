#pragma once
#include "Kernel.h"

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

struct CValue : public Value {
public:
    typedef void(*Ptr)(Kernel&, var&, Tuple&);
    CValue(Ptr $ptr) : ptr($ptr) {}
    virtual var operator()(Kernel& k, Tuple& x) {
        var r = &x;
        ptr(k, r, x);
        return r;
    }
protected:
    Ptr ptr;
};
#define CVALUE(p,f) p##_##f##_##Value

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
}
