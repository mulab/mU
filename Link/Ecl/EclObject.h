#pragma once
#include "Ecl.h"

namespace mU {
class EclObject : public Object {
	API static cl_object Pool;
	API static cl_object Insert(cl_object);
	API static void Erase(cl_object);
	API static void PrintPool();
	cl_object node;  // linked list pointer: car(node) == obj
public:
	API static sym $Ecl, $EclObject;
	API static cl_object $mU;
	API static bool Inited;
	API static void Init(int argc, char* argv[]);
	API static void Close();

	cl_object obj;   // the wrapped object
	virtual ~EclObject() {
		if (node)
			Erase(node);
	}
	API virtual EclObject* clone() const;
	API virtual int compare(const Object&) const;
	API virtual bool equal(const Object&) const;
	API virtual size_t hash() const;
	API virtual void print(wostream&) const;
	EclObject() : Object($EclObject), obj(NULL), node(NULL) {}
	EclObject(cl_object x) : Object($EclObject), node(NULL) { set(x); }
	EclObject& operator=(cl_object x) {
		set(x);
		return *this;
	}
	void set(cl_object x) {
		if (node) {
			Erase(node);
			node = NULL;
		}
		obj = x;
		if (!(Null(obj) || FIXNUMP(obj) || CHARACTERP(obj)))
			node = Insert(obj);
	}
};
}
