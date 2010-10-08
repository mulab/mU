#include <mU/System.h>

namespace mU {
wstring Path() {
#ifdef _MSC_VER
    wchar buf[0x100];
    memset(buf, 0, 0x100);
    GetModuleFileNameW(NULL, buf, 0x100);
    wcsrchr(buf, _W('\\'))[1] = 0;
    return buf;
#else
    char buf[0x100];
    memset(buf, 0, 0x100);
    readlink("/proc/self/exe", buf, 0x100);
    strrchr(buf, L'/')[1] = 0;
    return wstring(buf, buf + strlen(buf));
#endif
}
bool Shell(wcs x) {
#ifdef _MSC_VER
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );
    // Start the child process.
    if (CreateProcess(NULL,	// No module name (use command line)
                      const_cast<wchar*>(x),	// Command line
                      NULL,           // Process handle not inheritable
                      NULL,           // Thread handle not inheritable
                      FALSE,          // Set handle inheritance to FALSE
                      0,              // No creation flags
                      NULL,           // Use parent's environment block
                      NULL,           // Use parent's starting directory
                      &si,            // Pointer to STARTUPINFO structure
                      &pi )           // Pointer to PROCESS_INFORMATION structure
       ) {
        // Wait until child process exits.
        WaitForSingleObject( pi.hProcess, INFINITE );
        // Close process and thread handles.
        CloseHandle( pi.hProcess );
        CloseHandle( pi.hThread );
        return true;
    }
    return false;
#else
    return system(string(x, x + wcslen(x)).c_str()) == 0;
#endif
}
double Timing(Kernel& k, var& r, const var& x) {
	timer t;
	t.start();
	r = k.eval(x);
	t.end();
	return t.value;
}
}

using namespace mU;

CAPI void VALUE(Path)(Kernel& k, var& r, Tuple& x) {
    r = new String(Path());
}
CAPI void METHOD(Shell, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
    r = boolean(Shell(cast<String>(x[1]).str.c_str()));
}
CAPI void VALUE(Timing)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2) {
		var b;
		timer t;
		t.start();
		for (uint i = 0; i < 100000; ++i)
			b = k.eval(x[1]);
		t.end();
		r = tuple($.List, new Integer((uint)t.value), b);
		return;
	}
}
// #define T(x) (((cthread_t*)(Var)(x))->rep)
// struct cthread_t : obj_t {
// 	void print(wostream &f) {
// 		f << L"CThread[" << rep << L']';
// 	}
// 	var args;
// 	HANDLE rep;
// };
// namespace {
// 	DWORD WINAPI ThreadProc(__in  LPVOID lpParameter) {
// 		Eval(*(var*)lpParameter);
// 		return 0;
// 	}
// }
// var Task(Var x) {
// 	DWORD dwThreadId;
// 	cthread_t *r = new cthread_t;
// 	r->args = x;
// 	r->rep =
// 		CreateThread(
// 		NULL,		// default security attributes
// 		0,			// use default stack size
// 		ThreadProc,	// thread function name
// 		&r->args,	// argument to thread function
// 		0,			// use default creation flags
// 		&dwThreadId);
// 	return r;
// }
// bool Kill(Var x) {
// 	return TerminateThread(T(x), -1) == TRUE;
// }
