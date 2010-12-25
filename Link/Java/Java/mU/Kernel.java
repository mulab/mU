package mU;

import utils.Constants;

// import java.lang.reflect.*;

public class Kernel {

    static {
    	System.load(Constants.MU_JAVABIND_DLL_PATH);
//    	System.load("D:\\Projects\\GeometryBoard\\mU\\Link\\Java\\javabind.dll");
	}
    
    public long cptr;
    
    private static native long _constructor();
    public Kernel() {
        cptr = _constructor();
    }
    
    private static native void _destructor(long $cptr);
    private void destructor() {
		_destructor(cptr);
	}
    protected void finalize() {
        destructor();
    }
    
    private static native long _eval(long $cptr, long x);
    public VarPtr eval(VarPtr x) {
    	return new VarPtr(_eval(cptr, x.ptr));
    }
}
