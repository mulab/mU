#pragma once
#include "Kernel.h"

#ifdef _WIN32
#ifdef STREAM_EXPORTS	// TODO: change this to Stream_EXPORTS
#define STREAM_API __declspec(dllexport) extern
#else
#define STREAM_API __declspec(dllimport) extern
#pragma comment(lib,"Stream")
#endif
#else
#define STREAM_API extern
#endif

namespace mU {
namespace Stream {
STREAM_API var Routine(CProc);
STREAM_API var Routine(COper);
STREAM_API var Invoke(Var,Var);
STREAM_API var Invoke(Var,Var,Var);
struct stream_t : public obj_t { wiostream *rep; };
inline wiostream& CStream(Var x) { return *((stream_t*)x)->rep; }
STREAM_API var Buffer();
STREAM_API var Buffer(const wstring&);
STREAM_API var File(const wstring&);
STREAM_API var Link(const wstring&,size_t);
inline var Link(size_t x) { return Link(L"127.0.0.1",x); }
STREAM_API var Server(const wstring&,size_t);
inline var Server(size_t x) { return Server(L"127.0.0.1",x); }
STREAM_API var Request(Var);
STREAM_API var Pipe(const wstring&);
}
}
