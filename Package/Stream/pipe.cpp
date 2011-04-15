#include <mU/Stream.h>
#define BOOST_ALL_DYN_LINK
#include <boost/array.hpp>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
namespace mU {
namespace Stream {
struct wpipebuf : std::wstreambuf
{
	wpipebuf()
	{
		setp(put_buf.data(), put_buf.data() + bufsize);
	}
	int_type underflow(void)
	{
		bool ok;
		DWORD cnt;
		BOOL res = ReadFile(in, get_bufa.data(), bufsize / sizeof(char_type), &cnt, NULL);
		ok = (res && cnt > 0);
		if(ok)
		{
			wchar_t *dst = get_buf.data();
			const char *src = get_bufa.data();
			size_t rcnt, wcnt = 0;
			while(cnt)
			{
				rcnt = mbtowc(dst,src,cnt);
				src += rcnt;
				cnt -= rcnt;
				++dst;
				++wcnt;
			}
			setg(get_buf.data(), get_buf.data(), get_buf.data() + wcnt);
			return traits_type::to_int_type(*gptr());
		}
		else
			return traits_type::eof();
	}
	int_type overflow(int c)
	{
		if(sync() == -1)
			return traits_type::eof();
		if(!traits_type::eq_int_type(c, traits_type::eof()))
		{
			traits_type::assign(*pptr(), c);
			pbump(1);
		}
		return traits_type::not_eof(c);
	}
	int sync()
	{
		char *dst = put_bufa.data();
		const wchar_t *src = pbase();
		size_t mcnt, cnt = 0;
		while(src != pptr())
		{
			mcnt = wctomb(dst,*src);
			dst += mcnt;
			cnt += mcnt;
			++src;
		}
		bool ok;
		DWORD rcnt;
		BOOL res = WriteFile(out, put_bufa.data(), cnt, &rcnt, NULL);
		ok = (res && rcnt == cnt);
		if(ok) pbump(pbase() - pptr());
		return ok ? 0 : -1;
	}
	HANDLE in,out;
	enum { bufsize = 512 };
	boost::array<wchar_t, bufsize> get_buf;
	boost::array<wchar_t, bufsize> put_buf;
	boost::array<char, bufsize / sizeof(wchar_t)> get_bufa;
	boost::array<char, bufsize * sizeof(wchar_t)> put_bufa;
};
struct wpipestream : wiostream
{
	wpipestream(const wstring &x) : wiostream(&buffer)
	{
		HANDLE hOutputReadTmp,hOutputRead,hOutputWrite;
		HANDLE hInputWriteTmp,hInputRead,hInputWrite;
		HANDLE hErrorWrite;
		SECURITY_ATTRIBUTES sa;

		// Set up the security attributes struct.
		sa.nLength= sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;

		// Create the child output pipe.
		CreatePipe(&hOutputReadTmp,&hOutputWrite,&sa,0);

		// Create a duplicate of the output write handle for the std error
		// write handle. This is necessary in case the child application
		// closes one of its std output handles.
		DuplicateHandle(GetCurrentProcess(),hOutputWrite,
			GetCurrentProcess(),&hErrorWrite,0,
			TRUE,DUPLICATE_SAME_ACCESS);

		// Create the child input pipe.
		CreatePipe(&hInputRead,&hInputWriteTmp,&sa,0);
		
		// Create new output read handle and the input write handles. Set
		// the Properties to FALSE. Otherwise, the child inherits the
		// properties and, as a result, non-closeable handles to the pipes
		// are created.
		DuplicateHandle(GetCurrentProcess(),hOutputReadTmp,
			GetCurrentProcess(),
			&hOutputRead, // Address of new handle.
			0,FALSE, // Make it uninheritable.
			DUPLICATE_SAME_ACCESS);

		DuplicateHandle(GetCurrentProcess(),hInputWriteTmp,
			GetCurrentProcess(),
			&hInputWrite, // Address of new handle.
			0,FALSE, // Make it uninheritable.
			DUPLICATE_SAME_ACCESS);

		// Close inheritable copies of the handles you do not want to be
		// inherited.
		CloseHandle(hOutputReadTmp);
		CloseHandle(hInputWriteTmp);
		PROCESS_INFORMATION pi;
		STARTUPINFOW si;

		// Set up the start up info struct.
		ZeroMemory(&si,sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESTDHANDLES;
		si.hStdOutput = hOutputWrite;
		si.hStdInput  = hInputRead;
		si.hStdError  = hErrorWrite;
		CreateProcessW(NULL,(wchar*)x.c_str(),NULL,NULL,TRUE,
			DETACHED_PROCESS,NULL,NULL,&si,&pi);
		CloseHandle(hOutputWrite);
		CloseHandle(hInputRead);
		CloseHandle(hErrorWrite);
		hThread = pi.hThread;
		hProcess = pi.hProcess;
		buffer.in = hOutputRead;
		buffer.out = hInputWrite;
	}
	void close()
	{
		CloseHandle(buffer.in);
		CloseHandle(buffer.out);
		CloseHandle(hProcess);
		CloseHandle(hThread);
	}
	HANDLE hThread, hProcess;
	wpipebuf buffer;
};
#else
namespace mU {
namespace Stream {
// TODO: 实现管道通信类
class wpipestream : public std::wiostream
{
public:
	wpipestream(const wstring &x) {}
	void close() {}
};
#endif

struct pipe_t : stream_t
{
	pipe_t(const wstring &x) { rep = new wpipestream(x); }
	~pipe_t() { ((wpipestream*)rep)->close(); delete (wpipestream*)rep; }
	void print(wostream &f) { f << L"Pipe[" << rep << L']'; }
};
var Pipe(const wstring &x) { return new pipe_t(x); }
}
}
