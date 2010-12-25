package mU;

public class Parser {
    
	public long cptr;
    private transient Kernel kernel;
    
    private static native long _constructor(long $k);
    public Parser(Kernel k) {
    	kernel = k;
        cptr = _constructor(k.cptr);
    }
    
    private static native void _destructor(long $cptr);
    private void destructor() {
		_destructor(cptr);
	}
    protected void finalize() {
        destructor();
    }
    
    private static native long _parse(long $cptr, String x);
    public VarPtr parse(String x) {
    	return new VarPtr(_parse(cptr, x));
    }
}
