#include "../Java/mU_Kernel.h"
#include <mU/Kernel.h>
#ifdef _MSC_VER
#pragma comment(lib,"Kernel")
#endif

using namespace mU;

namespace {
inline jfieldID fieidID(JNIEnv* env, jobject obj, const char* field, const char* sig) {
	jclass cls = env->GetObjectClass(obj);
	return env->GetFieldID(cls, field, sig);
}
inline Var* VarPtr(jlong x) {
	return reinterpret_cast<Var*>(x);
}
inline jlong NewRef(Var* x) {
	return reinterpret_cast<jlong>(x->copy());
}
inline jlong NewRef(const var& x) {
	return reinterpret_cast<jlong>(x.ptr->copy());
}
inline Kernel* Ptr(jlong x) {
	return reinterpret_cast<Kernel*>(x);
}
}

/*
 * Class:     mU_Kernel
 * Method:    _constructor
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_mU_Kernel__1constructor
  (JNIEnv * env, jclass cls) {
	  return reinterpret_cast<jlong>(new Kernel());
}

/*
 * Class:     mU_Kernel
 * Method:    _destructor
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_mU_Kernel__1destructor
  (JNIEnv * env, jclass cls, jlong $cptr) {
	  delete Ptr($cptr);
}

/*
 * Class:     mU_Kernel
 * Method:    _eval
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_mU_Kernel__1eval
  (JNIEnv * env, jclass cls, jlong $cptr, jlong x) {
	  Kernel* cptr = Ptr($cptr);
	  return NewRef(cptr->eval(VarPtr(x)));
}