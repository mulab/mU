#pragma once
#include "var.h"

namespace mU {
class Match;
// Rewriting Engine + Map Database + Pattern Matcher
class Kernel {
public:
    // 重要系统栈,对用户表现为stack,但debug时需要能查看内部,所以用vector
    std::vector<sym> mContext;
    std::vector<std::list<sym> > mContextPath;
	std::vector<sym> mLocal;
    std::vector<var> mSelf;
    std::vector<var> mStack;

    API Kernel();

    API sym symbol(wcs);
    sym& context() {
        return mContext.back();
    }
    API bool beginContext(sym);
    API bool endContext();
    std::list<sym>& contextPath() {
        return mContextPath.back();
    }
    API bool beginPackage(sym);
    API bool endPackage();
    sym local() {
        return mLocal.back();
    }
    void beginLocal(sym x) {
        mLocal.push_back(x);
    }
    void beginLocal() {
        beginLocal(root->clone());
    }
    void endLocal() {
        mLocal.pop_back();
    }
    var& self() {
        return mSelf.back();
    }
    API void destroy(sym);
    API void clear(sym);

    API void print(wostream&, wchar);
    API void print(wostream&, wcs);
    API void print(wostream&, sym);
    API void print(wostream&, const Key&);
    API void print(wostream&, const Object&);
    API void print(wostream&, const Tuple&);
    API void print(wostream&, const var&);
    void println(const var& x, wostream& o = wcout) {
        print(o, x);
        o << endl;
    }

    // Rules Database

    // Attribute控制求值过程,顺序等
    typedef std::tr1::unordered_set<sym> Attribute;
    std::tr1::unordered_map<sym, Attribute> attributes;
    API Tuple* rewrite(Tuple*);
    API Tuple* thread(const Tuple&);
    API Tuple* flatten(sym, const Tuple&);

	API bool match(var&, const var&, Match&);
	API var replace(const var&, Match&);
	std::set<var> mBind;
	bool bound(const var& x) {
		return mBind.count(x) != 0;
	}
	bool bind(const var& x, const var& y) {
		mBind.insert(x);
		return assign(x, y);
	}
	bool unbind(const var& x) {
		mBind.erase(x);
		return assign(x, null);
	}

    // Rule
	std::tr1::unordered_map<sym, var> owns;
	std::tr1::unordered_map<sym, Map> certains;
	std::tr1::unordered_map<sym, Map> matches;
	
	// Interface
	std::tr1::unordered_map<sym, var> assigns;
    std::tr1::unordered_map<sym, var> values;

    // assign
    API bool assign(sym, const var&);
    API bool assign(const Key&, const var&);
    API bool assign(const Tuple&, const var&);
    API bool assign(const var&, const var&);
	API bool rule(const Tuple&, const Match*);

    // value
    API var value(sym);
    API var value(const Key&);
    API var value(Tuple&);
    API var value(const var&);
	API bool certain(var&, sym, const Tuple&);
	API bool match(var&, sym, const Tuple&);

    // get
    API var get(sym, const Key&);
    var slot(sym x, const Key& y) {
        beginSelf(x);
        var r = get(x, y);
        endSelf();
        return r;
    }
    API var get(const Tuple&, const Key&);
    var slot(const Tuple& x, const Key& y) {
        beginSelf(&x);
        var r = get(x, y);
        endSelf();
        return r;
    }
    API var get(const var&, const Key&);
    var slot(const var& x, const Key& y) {
        beginSelf(x);
        var r = get(x, y);
        endSelf();
        return r;
    }
    API var get(const var&, const Tuple&);
    var slot(const var& x, const Tuple& y) {
        beginSelf(x);
        var r = get(x, y);
        endSelf();
        return r;
    }
    API void beginSelf(const var&);
    API void endSelf();

    // set
    API bool set(sym, const Key&, const var&);
    API bool set(Tuple&, const Key&, const var&);
    API bool set(const var&, const Key&, const var&);
    API bool set(const var&, const Tuple&, const var&);

    // eval
	var& top() {
		return mStack.back();
	}
	void push(const var& x) {
		mStack.push_back(x);
	}
	void pop() {
		mStack.pop_back();
	}
    API void start();
    API void abort();
    API void debug();
    API var eval(const var&);
	API var lazy(const var&);

	// TODO: 其他内核变量改用Symbol Value控制
	uint recursion;
	uint depth;
	ostringstream error;
};
}