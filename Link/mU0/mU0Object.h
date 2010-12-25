#pragma once
#include "mU0.h"

namespace mU {
class mU0Object : public Object {
public:
	API static sym $mU0, $mU0Object;
	API static bool Inited;
	API static void Init();
	API static void Main(int argc, char* argv[]);
	API static void Close();
	template<class T>
	inline static mU0Object* New(T x) {
		return new mU0Object(to_mu0(x));
	}

	mU0::var obj;
	API virtual mU0Object* clone() const;
	API virtual int compare(const Object&) const;
	API virtual bool equal(const Object&) const;
	API virtual size_t hash() const;
	API virtual void print(wostream&) const;
	mU0Object() : Object($mU0Object) {}
	mU0Object(mU0::VAR x) : Object($mU0Object), obj(x) {}
	var normal() const {
		return from_mu0(obj);
	}
};
}
