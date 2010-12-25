#include "mU0Object.h"

extern "C" __declspec(dllimport) int mU0_main(int argc, char *argv[]);
namespace mU {
sym mU0Object::$mU0, mU0Object::$mU0Object;
bool mU0Object::Inited = false;
void mU0Object::Init() {
	if (Inited)
		return;

	$mU0 = SYM(root, mU0);
	$mU0Object = SYM($mU0, Object);
	mU0::ParseFile(mU0::Path() + _W("mU0.ini"));

	Inited = true;
}
void mU0Object::Main(int argc, char* argv[]) {
	Init();
	mU0_main(argc, argv);
}
void mU0Object::Close() {
	Inited = false;
}
mU0Object* mU0Object::clone() const {
	return new mU0Object(obj);
}
int mU0Object::compare(const Object& x) const {
	return mU0::Compare(obj, x.cast<mU0Object>().obj);
}
bool mU0Object::equal(const Object& x) const {
	return mU0::Same(obj, x.cast<mU0Object>().obj);
}
size_t mU0Object::hash() const {
	return Object::hash();
}
void mU0Object::print(wostream& o) const {
	if (obj)
		mU0::Print(mU0::Pretty(obj), o);
}
}