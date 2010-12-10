#include <mU/utils.h>
#include <mU/System.h>

namespace mU {
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
    if (x.size == 1) {
		r = new String(path());
		return;
	}
	if (x.size == 2 && x[1].isObject($.String)) {
		r = new String(path() + x[1].cast<String>().str);
		return;
	}
}
CAPI void METHOD(Shell, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
    r = boolean(shell(x[1].cast<String>().str.c_str()));
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
