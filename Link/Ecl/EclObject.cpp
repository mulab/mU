#include "EclObject.h"
#include <mU/utils.h>
#include <csignal>

namespace mU {
cl_object EclObject::Pool;
cl_object EclObject::Insert(cl_object x) {
    cl_object next, newnode;
    next = cl_cadr(Pool);
    newnode = cl_cons(x, cl_cons(next, Pool));
    cl_rplaca(cl_cdr(Pool), newnode);
    if (next != Cnil)
        cl_rplacd(cl_cdr(next), newnode);
    return newnode;
}
void EclObject::Erase(cl_object x) {
    cl_object next, prev;
    next = cl_cadr(x);
    prev = cl_cddr(x);
    if (next != Cnil)
        cl_rplacd(cl_cdr(next), prev);
    if (prev != Cnil)
        cl_rplaca(cl_cdr(prev), next);
}
void EclObject::PrintPool() {
	cl_object c = Pool;
	while(1) {
		printf(ecl_base_string_pointer_safe(cl_write_to_string(1, cl_car(c))));
		c = cl_cadr(c);
		if (c == Cnil)
			break;
	}
}
sym EclObject::$Ecl, EclObject::$EclObject;
cl_object EclObject::$mU;
bool EclObject::Inited = false;
void EclObject::Init(int argc, char* argv[]) {
	if (Inited)
		return;

	$Ecl = SYM(root, Ecl);
	$EclObject = SYM($Ecl, Object);
	ecl_set_option(ECL_OPT_INCREMENTAL_GC, 0);
	ecl_set_option(ECL_OPT_TRAP_SIGSEGV, 0);
	ecl_set_option(ECL_OPT_TRAP_SIGFPE, 0);
	ecl_set_option(ECL_OPT_TRAP_SIGINT, 0);
	ecl_set_option(ECL_OPT_TRAP_SIGILL, 0);
	ecl_set_option(ECL_OPT_TRAP_INTERRUPT_SIGNAL, 0);
	ecl_set_option(ECL_OPT_SIGNAL_HANDLING_THREAD, 0);
	ecl_set_option(ECL_OPT_SET_GMP_MEMORY_FUNCTIONS, 0);
	/*uint codes[] = {SIGINT, SIGILL, SIGFPE, SIGSEGV, SIGTERM, SIGBREAK, SIGABRT};
	typedef void (*act_t)(int);
	act_t acts[sizeof(codes) / sizeof(uint)];
	for (uint i = 0; i < sizeof(codes) / sizeof(uint); ++i)
		acts[i] = signal(codes[i], NULL);
		// acts[i] = signal(codes[i], SIG_IGN);
		*/
	cl_boot(argc, argv);
	/*for (uint i = 0; i < sizeof(codes) / sizeof(uint); ++i)
		signal(codes[i], acts[i]);
		*/
	ecl_disable_interrupts();
	read_from_string_clobj = cl_eval(ecl_read_from_cstring("(symbol-function 'read-from-string)"));
	$mU = cl_eval(ecl_read_from_cstring("(and (make-package :mU) (use-package :mU) (in-package :mU))"));
	Pool = cl_cons(Cnil, cl_cons(Cnil, Cnil));
	cl_set(ecl_read_from_cstring("*pool*"), Pool);
	cl_eval(ecl_read_from_cstring("\
        (defun safe-eval (form)\
            (handler-case\
                (values (eval form))\
                (serious-condition (cnd)\
                    (values nil (princ-to-string cnd)))))\
        "));
    safe_eval_clobj = cl_eval(ecl_read_from_cstring("(symbol-function 'safe-eval)"));
    cl_eval(ecl_read_from_cstring("\
        (defun safe-apply (func args)\
            (handler-case\
                (values (apply func args))\
                (serious-condition (cnd)\
                    (values nil (princ-to-string cnd)))))\
        "));

    safe_apply_clobj = cl_eval(ecl_read_from_cstring("(symbol-function 'safe-apply)"));
	cl_eval(ecl_read_from_cstring("\
        (defun safe-funcall (func arg)\
            (handler-case\
                (values (funcall func arg))\
                (serious-condition (cnd)\
                    (values nil (princ-to-string cnd)))))\
        "));
    safe_funcall_clobj = cl_eval(ecl_read_from_cstring("(symbol-function 'safe-funcall)"));
    
	Inited = true;
}
void EclObject::Close() {
	cl_shutdown();
	Inited = false;
}
EclObject* EclObject::clone() const {
	return new EclObject(cl_copy_tree(obj));
}
int EclObject::compare(const Object& x) const {
	cl_type t = type_of(obj);
	long r = t - type_of(x.cast<EclObject>().obj);
	if (r)
		return r;
	if (ECL_NUMBER_TYPE_P(t))
		return ecl_number_compare(obj, x.cast<EclObject>().obj);
	else if (t == t_base_string)
		return ecl_char_compare(obj, x.cast<EclObject>().obj);
	return Object::compare(x);
}
bool EclObject::equal(const Object& $other) const {
	return ecl_equalp(obj, $other.cast<EclObject>().obj);
}
size_t EclObject::hash() const {
	return fixint(cl_sxhash(obj));
}
void EclObject::print(wostream& o) const {
	cl_object t = cl_write_to_string(1, obj);
	const char* s = ecl_base_string_pointer_safe(t);
	o << mbs2wcs(s);
}
}