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

	DEF_TAG_SYM_WRAPPED_CPROC(Pipe)
	DEF_TAG_SYM_WRAPPED_CPROC(Buffer)
	DEF_TAG_SYM_WRAPPED_CPROC(File)
	DEF_TAG_SYM_WRAPPED_CPROC(Link)
	DEF_TAG_SYM_WRAPPED_CPROC(Read)
	DEF_TAG_SYM_WRAPPED_CPROC(Write)
	DEF_TAG_SYM_WRAPPED_CPROC(Server)
	DEF_TAG_SYM_WRAPPED_CPROC(Request)
	DEF_TAG_SYM_WRAPPED_CPROC(Parse)

	Initialized = true;
}
}
}
DLLMAIN(mU::Stream::Initialize)
