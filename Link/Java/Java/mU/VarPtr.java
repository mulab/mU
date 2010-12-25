package mU;

import java.math.BigDecimal;
import java.math.BigInteger;

import utils.Constants;

public class VarPtr {
    
    static {
		System.load(Constants.MU_JAVALINK_DLL_PATH);
//        System.load("D:\\Projects\\GeometryBoard\\mU\\Link\\Java\\javalink.dll");
    }
    
	public long ptr;
    
	// public static native long _copy(long $ptr);
    public VarPtr(long $ptr) {
        ptr = $ptr;
    }
    
    private static native void _destroy(long $ptr);
    private void destroy() {
		_destroy(ptr);
	}
    
    public void finalize() {
        destroy();
    }
    
    public static final VarPtr NULL		= new VarPtr(0);
    
    private static native long _newKey(String x);
    public static VarPtr newKey(String x) {
        return new VarPtr(_newKey(x));
    }
    
    private static native long _newKey(long x);
    public static VarPtr newKey(long x) {
        return new VarPtr(_newKey(x));
    }
    
    private static native long _newSymbol(String x);
    public static VarPtr newSymbol(String x) {
        return new VarPtr(_newSymbol(x));
    }
    
    private static native long _newSymbol(long c, String x);
    public static VarPtr newSymbol(VarPtr c, String x) {
    	if (c.type() == Var.SYMBOL)
    		return new VarPtr(_newSymbol(c.ptr, x));
    	return NULL;
    }
    
    private static native long _newTuple(long[] x);
    public static VarPtr newTuple(VarPtr[] x) {
        long[] t = new long[x.length];
        for (int i = 0; i < t.length; ++i)
        	t[i] = x[i].ptr;
        return new VarPtr(_newTuple(t));
    }    
    
    private static native long _newInteger(long x);
    public static VarPtr newInteger(long x) {
        return new VarPtr(_newInteger(x));
    }
    
    private static native long _newInteger(String x);
    public static VarPtr newInteger(String x) {
        return new VarPtr(_newInteger(x));
    }
    
    private static native long _newRational(long x, long y);
    public static VarPtr newRational(long x, long y) {
        return new VarPtr(_newRational(x, y));
    }
    
    private static native long _newRational(String x);
    public static VarPtr newRational(String x) {
        return new VarPtr(_newRational(x));
    }
    
    private static native long _newReal(double x);
    public static VarPtr newReal(double x) {
        return new VarPtr(_newReal(x));
    }
    
    private static native long _newReal(String x);
    public static VarPtr newReal(String x) {
        return new VarPtr(_newReal(x));
    }
    
    private static native long _newString(String x);
    public static VarPtr newString(String x) {
        return new VarPtr(_newString(x));
    }
    
    private static native long _newJavaObject(Object x);
    public static VarPtr newJavaObject(Object x) {
        return new VarPtr(_newJavaObject(x));
    }
    
    private static native int _type(long $ptr);
    public int type() {
    	return _type(ptr);
    }
    
    private static native String _getSymbol(long $ptr);
    public Symbol getSymbol() {
    	if (type() == Var.SYMBOL)
    		return Symbol.get(_getSymbol(ptr));
    	return Symbol.NULL;
    }
    
    private static native BigInteger _getBigInteger(long $ptr);
    public BigInteger getBigInteger() {
    	if (type() == Var.INTEGER)
    		return _getBigInteger(ptr);
    	return BigInteger.ZERO;
    }
    
    /*private static native int _getBigDecimal(long $ptr, byte[] $buf);
    private static BigDecimal getBigDecimal(long $ptr) { 
    	byte[] buf = null;
    	int scale = _getBigDecimal($ptr, buf);
    	return new BigDecimal(new BigInteger(buf), scale);
    }
    */
    
    private static native BigDecimal _getBigDecimal(long $ptr);
    public BigDecimal getBigDecimal() {
    	if (type() == Var.REAL)
    		return _getBigDecimal(ptr);
    	return BigDecimal.ZERO;
    }
    
    private static native String _getString(long $ptr);
    public String getString() {
    	if (type() == Var.STRING)
    		return _getString(ptr);
    	return "";
    }
    
    private static native Object _getJavaObject(long $ptr);
    public Object getJavaObject() {
    	if (type() == Var.JAVAOBJECT)
    		return _getJavaObject(ptr);
    	return null;
    }
    
    private static native long[] _tuple(long $ptr);
    public static VarPtr[] tuple(long[] x) {
        VarPtr[] r = new VarPtr[x.length];
        for (int i = 0; i < x.length; ++i)
        	r[i] = new VarPtr(x[i]);
    	return r;
    }
    public VarPtr[] tuple() {
        long[] t = _tuple(ptr);
        if (t == null)
        	return null;
        return tuple(t);
    }
    
    public Var toVar() {
        switch(type()) {
        //case Var.NULL:
        case -1:
        	return Var.NULL; 
        case Var.KEY:
        	return new Var(Symbol.get(_toString(ptr)));
        case Var.SYMBOL:
        	return new Var(Symbol.get(_getSymbol(ptr)));
        case Var.TUPLE:
        	return new Var(toVar(tuple()));
        case Var.INTEGER:
        	return new Var(_getBigInteger(ptr));
        case Var.REAL:
        	return new Var(_getBigDecimal(ptr));
        case Var.STRING:
        	return new Var(_getString(ptr));
        case Var.JAVAOBJECT:
        	return new Var(_getJavaObject(ptr));
        	// return Interface.cast(_getJavaObject(ptr));
        }
        // VARPTR
    	return new Var(this);
    }
    
	public static Var[] toVar(VarPtr[] x) {
    	Var[] r = new Var[x.length];
    	for (int i = 0; i < x.length; ++i)
    		r[i] = x[i].toVar();
    	return r;
	}
    
    private static native String _toString(long $ptr);
    public String toString() {
    	return _toString(ptr);
    }
    
    private static native int _hashCode(long $ptr);
    public int hashCode() {
    	return _hashCode(ptr);
    }
    
    private static native long _clone(long $ptr);
    public Object clone() {
    	return new VarPtr(_clone(ptr));
    }
    
    private static native boolean _equals(long x, long y);
    public boolean equals(Object obj) {
    	return _equals(ptr, ((VarPtr)obj).ptr);
    }
    
    private static native int _compare(long x, long y);
    public int compare(Object obj) {
    	return _compare(ptr, ((VarPtr)obj).ptr);
    }
    
    private static native int _primary(long $ptr);
    public int primary() {
    	return _primary(ptr);
    }
    
    private static native long _head(long $ptr);
    public VarPtr head() {
    	return new VarPtr(_head(ptr));
    }
    
    private static native double _toDouble(long $ptr);
    public double toDouble() {
    	return _toDouble(ptr);
    }
    
}
