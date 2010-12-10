#pragma once
#include "var.h"
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#else
#include <sys/time.h>
#endif

namespace mU {
API wstring date();
struct timer {
	double value;
#ifdef _WIN32
	timer() {
		QueryPerformanceFrequency(&Frequency);
	}
	void start() {
		QueryPerformanceCounter(&timerB);
	}
	void end() {
		QueryPerformanceCounter(&timerE);
		value = (double)(timerE.QuadPart - timerB.QuadPart)
			/ (double)Frequency.QuadPart * 1000000.0;
	}
	LARGE_INTEGER timerB, timerE, Frequency;
#else
	timer() {}
	void start() {
		gettimeofday(&timerB, NULL);
	}
	void end() {
		gettimeofday(&timerE, NULL);
		value = (double)((timerE.tv_usec - timerB.tv_usec)
			+ (timerE.tv_sec - timerB.tv_sec) * 1000000);
	}
	timeval timerB, timerE;
#endif
};
API wstring path();
API bool shell(wcs);
API wstring mbs2wcs(const char*);
inline wstring mbs2wcs(const string& x) {
	return mbs2wcs(x.c_str());
}
API string wcs2mbs(wcs);
inline string wcs2mbs(const wstring& x) {
	return wcs2mbs(x.c_str());
}

#define __POS__0(x) "[" __FUNCTION__ "]" __FILE__ "(" #x ")"
#define __POS__1(x) __POS__0(x)
#define __POS__ __POS__1(__LINE__)
#ifndef _STR2WSTR
#define __STR2WSTR(str)    _W(str)
#define _STR2WSTR(str)     __STR2WSTR(str)
#define __FILEW__          _STR2WSTR(__FILE__)
#define __FUNCTIONW__      _STR2WSTR(__FUNCTION__)
#endif
#define __POSW__0(x) _STR2WSTR("[") __FUNCTIONW__ _STR2WSTR("]") __FILEW__ _STR2WSTR("(") _STR2WSTR(#x) _STR2WSTR(")")
#define __POSW__1(x) __POSW__0(x)
#define __POSW__ __POSW__1(__LINE__)

#ifndef __FUNCSIG__
#define __FUNCSIG__ __PRETTY_FUNCTION__
#endif
#ifdef NDEBUG
#define fdebugf(fp, ...) ((void)0)
#else
API void _fdebugf(FILE * fp, const char * funcname, const char * filename, const int line, char *fmt, ...);
#define fdebugf(fp, ...) (void)(_fdebugf(fp, __FUNCSIG__, __FILE__, __LINE__, ##__VA_ARGS__))
#endif
#define dump(x, fmt, v) fdebugf(stdout, "%s = %" #fmt, #x, v);
}