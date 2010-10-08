#include <mU/Kernel.h>
#include <mU/Interface.h>
#ifndef _MSC_VER
#include <dlfcn.h>
#endif

namespace mU {
string cstr(wcs x) {
    return string(x, x + wcslen(x)).c_str();
}
string cstr(const var& x) {
    return cstr(cast<String>(x).toS());
}
string cpath(const char* x) {
#ifdef _MSC_VER
		return string(x) + string(".dll");
#else
        return string("lib") + string(x) + string(".so");
#endif
}
void* cload(const char* x) {
	return
#ifdef _MSC_VER
        LoadLibraryA(cpath(x).c_str())
#else
		cout << cpath(x) << endl;
        dlopen(x, RTLD_LAZY)
#endif
        ;
}
void* cnoload(const char* x) {
    return
#ifdef _MSC_VER
        GetModuleHandleA(cpath(x).c_str())
#else
        cout << cpath(x) << endl;
        dlopen(x, RTLD_NOLOAD)
#endif
        ;
}
void cunload(void* x) {
#ifdef _MSC_VER
    FreeLibrary(reinterpret_cast<HMODULE>(x));
#else
    dlclose(x);
#endif
}
void* csym(void* m, const char* x) {
    return
#ifdef _MSC_VER
        GetProcAddress(reinterpret_cast<HMODULE>(m), x)
#else
        dlsym(m, x)
#endif
        ;
}
bool cinstall(Kernel& k, const char* x) {
    typedef void(*Ptr)(Kernel&);
    void* m = cload(x);
    if (!m)
        return false;
    void* ptr = csym(m, "Install");
    if (ptr)
        reinterpret_cast<Ptr>(ptr)(k);
    return true;
}
bool cuninstall(Kernel& k, const char* x) {
    typedef void(*Ptr)(Kernel&);
    void* m = cnoload(x);
    if (!m)
        return false;
    void* ptr = csym(m, "Uninstall");
    if (ptr)
        reinterpret_cast<Ptr>(ptr)(k);
    cunload(m);
    return true;
}
string cname(sym x) {
    wstring s;
    while (x) {
        s = _W('_') + s;
        s = x->name() + s;
        x = x->context;
    }
    return cstr(s.c_str());
}
string csig(const Tuple& x) {
    stringstream ss;
    ss << x.size - 1;
    return ss.str();
}
bool cvalue(Kernel& k, void* m, sym x) {
    string s = cname(x) + "Value";
    void* ptr = csym(m, s.c_str());
    if (ptr) {
        k.values[x] = new CValue(reinterpret_cast<CValue::Ptr>(ptr));
        return true;
    }
    return false;
}
bool cassign(Kernel& k, void* m, sym x) {
    string s = cname(x) + "Assign";
    void* ptr = csym(m, s.c_str());
    if (ptr) {
        k.assigns[x] = new CAssign(reinterpret_cast<CAssign::Ptr>(ptr));
        return true;
    }
    return false;
}
bool cmethod(Kernel& k, void* m, const Tuple& x) {
    var h = x[0];
    while (h.isTuple())
        h = h.tuple()[0];
    if (!h.isSymbol())
        return false;
    string s = cname(h.symbol()) + "Method" + csig(x);
    void* ptr = csym(m, s.c_str());
    if (ptr)
        return set(k, x, new CMethod(reinterpret_cast<CMethod::Ptr>(ptr)));
    return false;
}
bool cinterface(Kernel& k, void* m, const Tuple& x) {
    if (x[0] == $.Assign) {
        for (uint i = 1; i < x.size; ++i)
            if (!x[i].isSymbol() || !cassign(k, m, x[i].symbol()))
                return false;
        return true;
    }
    if (x[0] == $.Value) {
        for (uint i = 1; i < x.size; ++i)
            if (!x[i].isSymbol() || !cvalue(k, m, x[i].symbol()))
                return false;
        return true;
    }
    return cmethod(k, m, x);
}
var CMethod::operator()(Kernel& k) {
    var r = k.top();
    ptr(k, r, k.top().tuple(), k.self(), k.local());
    return r;
}
}
