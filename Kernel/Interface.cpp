#include <mU/Interface.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <ShellAPI.h>
#else
#include <dlfcn.h>
#endif

namespace mU {
string cstr(wcs x) {
    return string(x, x + wcslen(x)).c_str();
}
string cstr(Var x) {
    return cstr(CStr(x).c_str());
}
string cpath(const char* x) {
#ifdef _WIN32
#ifdef _MSC_VER
    return string(x) + string(".dll");
#else
    return string("lib") + string(x) + string(".dll");
#endif
#else
    return string("lib") + string(x) + string(".so");
#endif
}
void* cload(const char* x) {
#ifdef _WIN32
	return LoadLibraryA(cpath(x).c_str());
#else
	string s = cpath(x);
	return dlopen(s.c_str(), RTLD_LAZY);
#endif
}
void* cnoload(const char* x) {
#ifdef _WIN32
	return GetModuleHandleA(cpath(x).c_str());
#else
	string s = cpath(x);
	return dlopen(s.c_str(), RTLD_LAZY | RTLD_NOLOAD);
#endif
}
void cunload(void* x) {
#ifdef _WIN32
    FreeLibrary(reinterpret_cast<HMODULE>(x));
#else
    dlclose(x);
#endif
}
void* csym(void* m, const char* x) {
    return
#ifdef _WIN32
        (void*)GetProcAddress(reinterpret_cast<HMODULE>(m), x)
#else
        dlsym(m, x)
#endif
        ;
}
bool cinstall(const char* x) {
    typedef void(*Ptr)();
    void* m = cload(x);
    if (!m)
        return false;
#ifndef _WIN32
	void *dllmain = csym(m, "DllMain");
	if (dllmain)
		reinterpret_cast<Ptr>(dllmain)();
#endif
    void* ptr = csym(m, "Install");
    if (ptr)
        reinterpret_cast<Ptr>(ptr)();
    return true;
}
bool cuninstall(const char* x) {
    typedef void(*Ptr)();
    void* m = cnoload(x);
    if (!m)
        return false;
    void* ptr = csym(m, "Uninstall");
    if (ptr)
        reinterpret_cast<Ptr>(ptr)();
    cunload(m);
    return true;
}
string cname(Var x) {
    wstring s = ContextName[Context(x)];
	std::replace(s.begin(), s.end(), _W('`'),_W('_'));
	s += Name(x);
    return cstr(s.c_str());
}
void* cfunc(void* m, Var x) {
    string s = cname(x);
    return csym(m, s.c_str());
}
}

using namespace mU;

CAPI var System_CInstall(Var x) {
	return cinstall(cstr(At(x,0)).c_str()) ? True : False;
}
CAPI var System_CUninstall(Var x) {
	return cuninstall(cstr(At(x,0)).c_str()) ? True : False;
}
CAPI var System_CProc(Var x) {
	void* m = cnoload(cstr(At(x,0)).c_str());
	if (!m)
		return False;
	for (size_t i = 1; i < Size(x); ++i) {
		CProc ptr = (CProc)cfunc(m, At(x,i));
		if (!ptr) {
			wcerr << _W("CProc ") << Name(At(x,i)) << _W(" not found!") << std::endl;
			return False;
		}
		CProcs[At(x,i)] = ptr;
	}
	return True;
}
