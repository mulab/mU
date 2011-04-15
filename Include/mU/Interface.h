#pragma once
#include "Kernel.h"

namespace mU {
typedef const wchar* wcs;
API string cstr(wcs);
inline string cstr(const wstring& x) {
	return cstr(x.c_str());
}
API string cstr(Var);
API string cpath(const char*);
API void* cload(const char*);
API void* cnoload(const char*);
API void cunload(void*);
API void* csym(void*, const char*);
API bool cinstall(const char*);
API bool cuninstall(const char*);
API string cname(Var);
API void* cfunc(void*, Var);
}
