#include <mU/Number.h>
#include <mU/Stream.h>

namespace mU {
namespace Stream {
namespace {
Wrap(Buffer)
{
	return Size(x) ? Buffer(CStr(At(x,0))) : Buffer();
}
Wrap(File)
{
	return File(CStr(At(x,0)));
}
Wrap(Link)
{
	if(Size(x) > 1)
		return Link(CStr(At(x,0)),Z::UI(At(x,1)));
	return Link(Z::UI(At(x,0)));
}
Wrap(Read)
{
	var r = Size(x) > 1
		? Read(CStream(At(x,0)),At(x,1))
		: Read(CStream(At(x,0)));
	return r ? r : Nil;
}
Wrap(Write)
{
	var y = Vec();
	CVec(y).assign(CVec(x).begin() + 1,CVec(x).end());
	Write(CStream(At(x,0)),y);
	return Null;
}
Wrap(Server)
{
	if(Size(x) > 1)
		return Server(CStr(At(x,0)),Z::UI(At(x,1)));
	return Server(Z::UI(At(x,0)));
}
Wrap(Request)
{
	return Request(At(x,0));
}
Wrap(Invoke)
{
	return Size(x) > 2
		? Invoke(At(x,0),At(x,1),At(x,2))
		: Invoke(At(x,0),At(x,1));
}
Wrap(Parse)
{
	var r = Parse(CStream(At(x,0)));
	return r ? r : Nil;
}
Wrap(Pipe)
{
	return Pipe(CStr(At(x,0)));
}
}
void Initialize()
{
	static bool Initialized = false;
	if(Initialized) return;
#define T(x) var TAG(x) = Sym(L###x,System);CProcs[TAG(x)] = WRAP(x);
	T(Pipe)T(Buffer)T(File)T(Link)T(Read)T(Write)
	T(Server)T(Request)T(Parse)
#undef T
#define T(x) var TAG(x) = Sym(L###x,System);COpers[TAG(x)] = WRAP(x);

#undef T
#define T(x,y) Attributes[TAG(x)].insert(y);
	
#undef T
		Initialized = true;
}
}
}
DLLMAIN(mU::Stream::Initialize)
