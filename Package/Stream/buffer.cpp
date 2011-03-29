#include <mU/Stream.h>

namespace mU {
namespace Stream {
struct cproc_t : obj_t
{
	void print(wostream &f) { f << L"CProc[" << rep << L']'; }
	cproc_t(CProc x) : rep(x) {}
	CProc rep;
};
struct coper_t : obj_t
{
	void print(wostream &f) { f << L"COper[" << rep << L']'; }
	coper_t(COper x) : rep(x) {}
	COper rep;
};
var Routine(CProc x) { return new cproc_t(x); }
var Routine(COper x) { return new coper_t(x); }
var Invoke(Var x, Var y) { return ((cproc_t*)(x))->rep(y); }
var Invoke(Var x, Var y, Var z) { return ((coper_t*)(x))->rep(y,z); }
struct buffer_t : stream_t
{
	buffer_t() { rep = new wstringstream; }
	buffer_t(const wstring &x) { rep = new wstringstream(x); }
	~buffer_t() { delete (wstringstream*)rep; }
	void print(wostream &f)
	{
		f << L"Buffer[\"" <<
			((wstringstream*)rep)->str() << L"\"]";
	}
};
struct file_t : stream_t
{
	file_t(const wstring &x) : path(x)
	{
		wfstream *r = new wfstream(string(x.begin(),x.end()).c_str());
		if(!*r)
		{
			r->clear();
			r->open(string(x.begin(),x.end()).c_str(),
			wios::in | wios::out | wios::trunc);
		}
		rep = r;
	}
	~file_t() { ((wfstream*)rep)->close(); delete (wfstream*)rep; }
	void print(wostream &f) { f << L"File[\"" << path << "\"]"; }
	wstring path;
};
var Buffer() { return new buffer_t; }
var Buffer(const wstring &x) { return new buffer_t(x); }
var File(const wstring &x) { return new file_t(x); }
}
}