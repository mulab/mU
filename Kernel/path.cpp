#include <mU/Kernel.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ShellAPI.h>
namespace mU {
//////////////////////////////////////
#define T(x) (((cmodule_t*)(Var)(x))->rep)
struct cmodule_t : obj_t
{
	void print(wostream &f) { f << L"CModule[" << rep << L']'; }
	HMODULE rep;
};
wstring Path()
{
	static wchar buf[0x100];
	GetModuleFileNameW(NULL,buf,0x100);
	wcsrchr(buf,L'\\')[1] = 0;
	return buf;
}
var Install(const wstring &x)
{
	cmodule_t *r = new cmodule_t;
	r->rep = LoadLibraryW(x.c_str());
	if(r->rep) return r;
	delete r;
	return 0;
}
bool Uninstall(Var x)
{
	return FreeLibrary(T(x)) == TRUE;
}
#undef T
bool Run(const wstring &x)
{
	STARTUPINFOW si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );

	// Start the child process.
	if(CreateProcessW(NULL,	// No module name (use command line)
		(wchar*)x.c_str(),	// Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory
		&si,            // Pointer to STARTUPINFO structure
		&pi )           // Pointer to PROCESS_INFORMATION structure
		)
	{
		// Wait until child process exits.
		WaitForSingleObject( pi.hProcess, INFINITE );

		// Close process and thread handles.
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );
		return true;
	}
	return false;
}
#define T(x) (((cthread_t*)(Var)(x))->rep)
struct cthread_t : obj_t
{
	void print(wostream &f) { f << L"CThread[" << rep << L']'; }
	var args;
	HANDLE rep;
};
namespace {
	// FIXME: is this __in parameter decorator really necessary?
	//        it's causing compilation errors in mingw, commenting
	//        it out for now
	DWORD WINAPI ThreadProc(/*__in*/  LPVOID lpParameter)
	{
		Eval(*(var*)lpParameter);
		return 0;
	}
}
var Task(Var x)
{
	DWORD dwThreadId;
	cthread_t *r = new cthread_t;
	r->args = x;
	r->rep =
		CreateThread(
		NULL,		// default security attributes
		0,			// use default stack size
		ThreadProc,	// thread function name
		&r->args,	// argument to thread function
		0,			// use default creation flags
		&dwThreadId);
	return r;
}
bool Kill(Var x)
{
	return TerminateThread(T(x),-1) == TRUE;
}
#undef T
#else
#include <dlfcn.h>
namespace mU {
//////////////////////////////////////
#define T(x) (((cmodule_t*)(Var)(x))->rep)
struct cmodule_t : obj_t
{
	void print(wostream &f) { f << L"CModule[" << rep << L']'; }
	void* rep;
};
wstring Path()
{
    static char buf[0x100];
    memset(buf,0,0x100);
    readlink("/proc/self/exe",buf,0x100);
	strrchr(buf,L'/')[1] = 0;
	return wstring(buf,buf + strlen(buf));
}
var Install(const wstring &x)
{
	cmodule_t *r = new cmodule_t;
    	r->rep = dlopen(string(x.begin(),x.end()).c_str(),RTLD_LAZY);
	if(r->rep)
	{
		typedef void(*DllMain)();
		DllMain f = (DllMain)dlsym(r->rep,"DllMain");
		if(f) f();
		return r;
	}
	wcerr
        << L"Install:"
		<< dlerror() << std::endl;
	delete r;
	return 0;
}
bool Uninstall(Var x)
{
    return dlclose(T(x)) == 0;
}
#undef T
bool Run(const wstring &x)
{
	return system(string(x.begin(),x.end()).c_str()) == 0;
}
#define T(x) (((cthread_t*)(Var)(x))->rep)
struct cthread_t : obj_t
{
	void print(wostream &f) { f << L"CThread[" << rep << L']'; }
	var args;
	pthread_t rep;
};
namespace {
	void* ThreadProc(void* lpParameter)
	{
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
		pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
		Eval(*(var*)lpParameter);
		return 0;
	}
}
var Task(Var x)
{
	cthread_t *r = new cthread_t;
	r->args = x;
	pthread_create(
		&r->rep,
		NULL,		// default security attributes
		ThreadProc,	// thread function name
		&r->args);	// argument to thread function
	return r;
}
bool Kill(Var x)
{
	return pthread_cancel(T(x)) == 0;
}
#undef T
#endif
//////////////////////////////////////
}
