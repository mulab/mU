package mU;

import java.math.BigInteger;
import java.math.BigDecimal;
    
public final class Var {
	
	public static final Var NULL		= new Var(Symbol.NULL);
	//public static final int NULL		= -1;
	
	public static final int OBJECT 		= 0;
	public static final int KEY 		= 1;
	public static final int SYMBOL 		= 2;
	public static final int TUPLE 		= 3;
	
	public static final int INTEGER		= 4;
	public static final int RATIONAL	= 5;
	public static final int REAL 		= 6;
	public static final int STRING 		= 7;
	
	public static final int JAVAOBJECT	= 8;
	public static final int VARPTR		= 9;
	
	public static final int INVALID		= 10;
		
	public Symbol type() {
		switch (type) {
		case OBJECT:
			return Symbol.OBJECT;	
		case KEY:
			return Symbol.KEY;
		case SYMBOL:
			return Symbol.SYMBOL;
		case TUPLE:
			return Symbol.TUPLE;
		case INTEGER:
			return Symbol.INTEGER;
		case RATIONAL:
			return Symbol.RATIONAL;
		case REAL:
			return Symbol.REAL;
		case STRING:
			return Symbol.STRING;
		case JAVAOBJECT:
			return Symbol.JAVAOBJECT;
		case VARPTR:
			return ((VarPtr)val).head().getSymbol();
		}
		return Symbol.NULL;
	}
    
	public int type;
	public Object val;
	private volatile int cachedHashCode = 0;

	public Var(Symbol val) {
		
		this.type = SYMBOL;
		this.val = val;
	}
	
	public Var(Var[] val) {
		
		this.type = TUPLE;
		// this.tuple = tuple != null ? (Var[]) tuple.clone() : new Var[0];
		this.val = val;
	}
	
	public Var(BigInteger val) {
		
		this.type = INTEGER;
		this.val = val;
		
	}

	public Var(BigDecimal val) {
		
		this.type = REAL;
		this.val = val;
		
	}
	
	public Var(String val) {
		
		this.type = STRING;
		this.val = val;
	}
	
	public Var(Object val) {
		
		this.type = JAVAOBJECT;
		this.val = val;
	}
	
	public Var(VarPtr val) {
		
		this.type = VARPTR;
		this.val = val;
	}
	
	public Var(int type, String val) {
		
		this.type = type;
		switch (type) {
			case SYMBOL:
				this.val = Symbol.get(val);
				break;
			case INTEGER:
				this.val = new BigInteger(val);
				break;
			case REAL:
				this.val = new BigDecimal(val);
				break;
			case STRING:
				this.val = val;
				break;
			default:
				throw new IllegalArgumentException("Unsupported type in Var(type, string) constructor: " + type);
		}
	}
    
	public Var[] tuple() {
		return (Var[])val;
	}
	
	public Var tuple(int i) {
		return ((Var[])val)[i];
	}
	
    public VarPtr toVarPtr() {
        switch(type) {
        case SYMBOL:
        	return VarPtr.newSymbol(((Symbol)val).toString());
        case TUPLE:
        	VarPtr[] r = new VarPtr[tuple().length];
        	for (int i = 0; i < tuple().length; ++i)
        		r[i] = tuple(i).toVarPtr();
        	return VarPtr.newTuple(r);
        case INTEGER:
        	return VarPtr.newInteger(((BigInteger)val).toString());
        case REAL:
        	return VarPtr.newReal(((BigDecimal)val).toString());
        case STRING:
        	return VarPtr.newString((String)val);
        case VARPTR:
        	return (VarPtr)val;
        }
        // JAVAOBJECT
        return VarPtr.newJavaObject(val);
    }
    
	public static VarPtr[] toVarPtr(Var[] x) {
    	VarPtr[] r = new VarPtr[x.length];
    	for (int i = 0; i < x.length; ++i)
    		r[i] = x[i].toVarPtr();
    	return r;
	}
	
	public boolean equals(Object obj) {
		
		if (obj == this)
			return true;
		if (!(obj instanceof Var))
			return false;
		Var otherExpr = (Var) obj;
		if (cachedHashCode != 0 && otherExpr.cachedHashCode != 0 &&
				cachedHashCode != otherExpr.cachedHashCode)
			return false;		
		if (type != otherExpr.type)
			return false;
		if (type != TUPLE)  {
			switch (type) {
				case SYMBOL:
				case INTEGER:
				case REAL:
				case STRING:
                    return val.equals(otherExpr.val);
				default:
					// Just to make the compiler happy; should never get here.
					return false;
			}
		} else {
			if (tuple().length != otherExpr.tuple().length)
				return false;
			for (int i = 0; i < tuple().length; i++)
				if (!tuple(i).equals(otherExpr.tuple(i)))
					return false;
			return true;
		}
	}
	
	
	public int hashCode() {
		
		if (cachedHashCode != 0)
			return cachedHashCode;
		
		if (atomQ())
			return val.hashCode();
			
		// Algorithm from "Effective Java" item 8.
		int hash = 17;
		hash = 37 * hash + type;
		if (type == TUPLE)
			for (int i = 0; i < tuple().length; i++)
				hash = 37 * hash + tuple(i).hashCode();
	
		cachedHashCode = hash;
		return hash;
	}
	
	public int length() {
		
		return type == TUPLE ? tuple().length : 0;
	}
	
	public Var part(int i) {
		
		if (type != TUPLE || Math.abs(i) > tuple().length)
			throw new IllegalArgumentException("Cannot take part " + i + " from this Var because it has length " + length() + ".");
		else if (i >= 0)
			return tuple(i);
		else 
			return tuple(tuple().length + i);
	}
	
	public String toString() {
		
		switch (type) {
			case SYMBOL:
				return val.toString();
			case TUPLE: {
				boolean isList = (tuple(0).val == (Object)Symbol.LIST);
				int len = length();
				StringBuffer buf = new StringBuffer(len * 2);
				buf.append(isList ? "{" : (tuple(0).toString() + "["));
				for (int i = 1; i < len; i++) {
					buf.append(tuple(i).toString());
					if (i < len - 1)
						buf.append(",");
				}
				buf.append(isList ? '}' : ']');
				return new String(buf);
			}
			case INTEGER:
			case REAL:
                return val.toString();
			case STRING: {
				String s = val.toString();
				StringBuffer buf = new StringBuffer(s.length() + 10);
				buf.append('"');
				int len = s.length();
				for (int i = 0; i < len; i++) {
					char c = s.charAt(i);
					if (c == '\\' || c == '"')
						buf.append('\\');
					buf.append(c);
				}
				buf.append('"');
				return new String(buf);
			}
			case JAVAOBJECT:
				return Symbol.JAVAOBJECT.toString() + '[' + val.toString() + ']';
			case VARPTR:
				return val.toString();
		}
		return "";
	}
	
	public double toDouble() {
		switch (type) {
			case TUPLE:
				if (length() > 1) {
					if (tuple(0).symbolQ(Symbol.MINUS))
						return -tuple(1).toDouble();
					else if (tuple(0).symbolQ(Symbol.PLUS))
						return tuple(1).toDouble(); 
				}
				break;
			case INTEGER:
				return ((BigInteger) val).doubleValue();
            case REAL:
                return ((BigDecimal) val).doubleValue();
			default:
				System.err.println("This Var is of type " + type() + " and cannot be represented as a Java double");
		}
		// return Double.NaN;
		return 0.0;
	}
	
	public Symbol getSymbol() {
		if (type == SYMBOL)
			return (Symbol)val;
		return Symbol.NULL;
	}
	
	public Var[] getTuple() {
		if (type == TUPLE)
			return (Var[])val;
		return null;
	}
	
	public BigInteger getBigInteger() {
		if (type == INTEGER)
			return (BigInteger)val;
		return BigInteger.ZERO;
	}
	
	public BigDecimal getBigDecimal() {
		if (type == REAL)
			return (BigDecimal)val;
		return BigDecimal.ZERO;
	}
	
	public String getString() {
		if (type == STRING)
			return (String)val;
		return "";
	}
	
	public Object getVarPtr() {
		if (type == VARPTR)
			return (VarPtr)val;
		return VarPtr.NULL;
	}
	
	public boolean symbolQ() {
		return type == SYMBOL;
	}
	
	public boolean symbolQ(Symbol x) {
		return type == SYMBOL && this.val == (Object)x; 
	}
	
	public boolean nullQ() {
		return symbolQ(Symbol.NULL);
	}
	
	public boolean trueQ() {
		return symbolQ(Symbol.TRUE);
	}
	
	public boolean falseQ() {
		return symbolQ(Symbol.FALSE);
	}
	
	public boolean integerQ() {
		return type == INTEGER;
	}
	
	public boolean realQ() {
		return type == REAL;
	}
	
	public boolean stringQ() {
		return type == STRING;
	}
	
	public boolean atomQ() {
		return type != TUPLE;
	}
	
	public boolean numberQ() {
		return type == INTEGER || type == REAL;
	}
	
	public boolean tupleQ() {
		return type == TUPLE;
	}
	
	public boolean tupleQ(Symbol head) {
		return type == TUPLE && tuple(0).val == (Object)head;
	}
	
	public boolean listQ() {
		return tupleQ(Symbol.LIST);
	}
	
	public boolean sameQ(Symbol x) {
		return type == SYMBOL && val == (Object)x;
	}
	
	public interface Visitor
	{

		public abstract boolean visit(Var[] x);

		public abstract void visit(Var x);

	}
	
	public void accept(Visitor visitor) {
		if (type == TUPLE) {
			if (!visitor.visit(tuple()))
				return;
			tuple(0).accept(visitor);
			for (int i = 1; i < length(); i++)
				tuple(i).accept(visitor);
		} else
			visitor.visit(this);
	}
	
}

