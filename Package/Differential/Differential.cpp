#include <mU/Number.h>
#include <mU/Kernel.h>

namespace mU {
namespace Differential {
namespace {

var TAG($D), TAG($Derivative), TAG($MultiD);

var D(Var x,Var y)
{
	map_t e;
	e[TAG(Derivative)]=TAG($Derivative);
	var r=Subs(e,x);
	//强制求值？要化简
	return Expand(Eval(Ex(TAG($D),Vec(r,y))));
}

Wrap(D)
{
	size_t m=Size(x);
	var r=At(x,0);
	for (size_t k=1;k<m;k++)
	{
		var para=At(x,k);
		if (Type(para)==TYPE(vec))
		{
			if (Type(At(para,1))==TYPE(int))
			{
				size_t n=Z::SI(At(para,1));
				for (int i=0;i<n;i++)
					r=D(r,At(para,0));
			}
			//处理符号阶的导数，无关则返回0
			else if (FreeQ(r,At(para,0))) return Int(0L);
			//否则原样返回
			else return 0;
		}
		else
			if (Type(para)==TYPE(sym) || Head(para)==TAG(Slot))
		{
			r=D(r,para);
		}
			else return 0;
	}

	map_t e;
	e[TAG($Derivative)] = TAG(Derivative);
	e[TAG($D)] = TAG(D);
	r= Subs(e,r);
	return r;
}

Wrap2($Derivative)
{
	if (Size(y)==1) {
		var head=Head(At(y,0));
		if (Head(head) == TAG($Derivative) && Size(x)==Size(Body(head)))
		{
			var r = Vec();
			for (size_t i=0;i<Size(x);i++)
			{
				Push(r,Plus(At(x,i),At(Body(head),i)));
			}
			return Ex(Ex(TAG($Derivative),r),Body(At(y,0)));
		}
	}
	return 0;
}

Wrap2(Derivative)
{
	size_t n=Size(x);
	var t=Vec();
	for (size_t i=0;i<n;i++)
	{
		Push(t, Ex(TAG(Slot),Vec(Int((uint)i+1))));
	}
	var r=Vec(Ex(At(y,0),t));
	for (size_t i=0;i<n;i++)
	{
		Push(r, Vec(Ex(TAG(Slot),Vec(Int((uint)i+1))), At(x,i)));
	}
	r=WRAP(D)(r);
	if ( Head(Head(Head(r)))==TAG(Derivative) && (Same(At(Body(Head(r)),0), At(y,0)) ||
		((Head(Head(At(y,0)))==TAG(Derivative)  && Same(At(Body(Head(r)),0), At(Body(At(y,0)),0)))  )
		 ))
		return Head(r);
	else
		return Ex(TAG(Function),Vec(r));
}

//\todo 最好改用脚本写
Wrap($MultiD)
{
	var f=At(x,0);
	var g=At(x,1);
	var v=At(x,2);
	var r=Int(0L);
	size_t n=Size(g);
	for (size_t i=0;i<n;i++)
	{
		var t=Vec();
		for (size_t j=0;j<n;j++)
		{
			if (i==j) Push(t,Int(1L));
				else Push(t,Int(0L));
		}
		var op1=Eval(Ex(Ex(Ex(TAG($Derivative),t),Vec(f)),g));
		var op2=D(At(g,i),v);
		r=Plus(r, Times(op1,op2));
	}
	return r;
}
}
void Initialize()
{
	static bool Initialized = false;
	if(Initialized) return;
	ParseFile(Path() + L"Package/Differential/D.u");

	DEF_SYSTEM_TAG_SYM($D)
	DEF_SYSTEM_TAG_SYM($Derivative)
	DEF_SYSTEM_TAG_SYM($MultiD)

	DEF_WRAPPED_CPROC(D)
	DEF_WRAPPED_CPROC($MultiD)

	//\todo 删
	DEF_WRAPPED_COPER(Derivative)
	DEF_WRAPPED_COPER($Derivative)

	Initialized = true;
}
}
}
DLLMAIN(mU::Differential::Initialize)
