#include "../Java/mU_Parser.h"
#include <mU/Parser.h>
#ifdef _MSC_VER
#pragma comment(lib,"Parser")
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
inline Parser* Ptr(jlong x) {
	return reinterpret_cast<Parser*>(x);
}
}

/*
 * Class:     mU_Parser
 * Method:    _constructor
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_mU_Parser__1constructor
  (JNIEnv * env, jclass cls, jlong $k) {
	  Kernel* k = reinterpret_cast<Kernel*>($k);
	  return reinterpret_cast<jlong>(new Parser(*k));
}

/*
 * Class:     mU_Parser
 * Method:    _destructor
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_mU_Parser__1destructor
  (JNIEnv * env, jclass cls, jlong $cptr) {
	  delete Ptr($cptr);
}

/*
 * Class:     mU_Parser
 * Method:    _parse
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_mU_Parser__1parse
  (JNIEnv * env, jclass cls, jlong $cptr, jstring x) {
	  Parser* cptr = Ptr($cptr);
	  const jchar* buf = env->GetStringChars(x, 0);
	  wstringstream ss;
	  ss << (wcs)buf;
	  cptr->start(ss);
	  var r = cptr->eval();
	  env->ReleaseStringChars(x, buf);
	  return NewRef(r);
}