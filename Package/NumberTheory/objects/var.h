#pragma once

#ifdef _MSC_VER
#ifdef VAR_EXPORTS
#define VAR_API __declspec(dllexport)
#else
#define VAR_API
// #define VAR_API __declspec(dllimport)
// #pragma comment(lib,"var")
#endif
#else
#define VAR_API
#endif

namespace maTHmU {
//////////////////////////////////////
typedef int INT;
typedef short SHORT;
typedef char CHAR;
typedef bool BOOL;
typedef double DOUBLE;
typedef unsigned int UINT;
typedef unsigned short USHORT;
typedef unsigned char UCHAR;
typedef void VOID;
//////////////////////////////////////
class var;
typedef const var& VAR;
//////////////////////////////////////
class VAR_API var {
	friend VAR_API UINT GetType(VAR);
	friend VAR_API VOID SetType(var&,UINT);
public:
	static void start();
	var();
	var(VAR);
	var(VOID*);
	var(VOID*, UINT);
	var& operator = (VAR);
	~var();
	VOID* ptr;
private:
	void clone(VAR);
	void set(VAR);
	void incr() const;
	void decr();
	void init();
	void clear();
	UINT id;
};
typedef var (*N_OP) ();
typedef var (*UN_OP)(VAR);
typedef var (*BIN_OP)(VAR,VAR);
typedef var (*TER_OP)(VAR,VAR,VAR);
typedef var (*SC_OP)(VAR,INT);
typedef INT (*CMP_OP)(VAR,VAR);
typedef BOOL (*LESS_OP)(VAR,VAR);
typedef VOID (*DES_OP)(VOID*);
//////////////////////////////////////
VAR_API DES_OP& TypeTable(UINT);
VAR_API UINT AddType(DES_OP);
VAR_API VOID RemoveType(UINT);
//////////////////////////////////////
VAR_API var& NameTable(const CHAR*);
VAR_API BOOL FindName(const CHAR*);
VAR_API VOID RemoveName(const CHAR*);
//////////////////////////////////////
}
