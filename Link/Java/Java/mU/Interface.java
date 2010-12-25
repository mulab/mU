package mU;

import java.lang.reflect.Array;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.math.BigDecimal;
import java.math.BigInteger;

public class Interface {

	public interface Assign {
		public boolean execute(Kernel k, VarPtr[] x, VarPtr y);
	}

	public interface Value {
		public VarPtr execute(Kernel k, VarPtr[] x);
	}

	public interface Method {
		public VarPtr execute(Kernel k);
	}
	
	@SuppressWarnings("rawtypes")
	public static Var invoke(Object obj, String methodName, Var[] x) {
		Object[] args = new Object[x.length];
		Class clazz;
		if (obj instanceof Class)
			clazz = (Class) obj;
		else
			clazz = obj.getClass();
		java.lang.reflect.Method[] methods = clazz.getMethods();
		java.lang.reflect.Method method = null;
		for (int i = 0; i < methods.length; i++) {
			if (!methods[i].getName().equals(methodName))
				continue;
			Class[] parameters = methods[i].getParameterTypes();
			if (parameters.length != args.length)
				continue;
			boolean okMethod = true;
			for (int j = 0; j < parameters.length; j++) {
				try {
					args[j] = cast(x[j], parameters[j]);
				} catch (Exception e)	{
					okMethod = false;
					break;
				}
			}
			if (okMethod) {
				method = methods[i];
				break;
			}
		}
		if (method == null) {
			System.err.println("Invalid method call. No satisfied method " + methodName + "!");
			System.err.println("All possible methods: ");
			for (int i = 0; i < methods.length; ++i) {
				if (!methods[i].getName().equals(methodName))
					continue;
				System.err.println(methods[i]);
			}
			return Var.NULL;
		}
		Object ret = null;
		try {
			if(Modifier.isPublic(method.getModifiers()))
				method.setAccessible(true);
			if (obj instanceof Class)
				ret = method.invoke(null, args);
			else
				ret = method.invoke(obj, args);	
		} catch (Exception e)	{
			System.err.println(e.getMessage() + " occured while invoke " + method + "!");
			// throw new mU.Exception(e);
			return Var.NULL;
		}
		return cast(ret);
	}
	
	public static VarPtr invoke(Object obj, String methodName, VarPtr[] x) {
		return invoke(obj, methodName, VarPtr.toVar(x)).toVarPtr();
	}
	
	// @SuppressWarnings("unused")
	public static long _invoke(Object obj, String methodName, long[] x) {
		return invoke(obj, methodName, VarPtr.tuple(x)).ptr;
	}
	
	@SuppressWarnings("rawtypes")
	public static Var newInstance(Class cls, Var[] x) {
		Object[] args = new Object[x.length];
		Constructor[] constructors = cls.getConstructors();
	    Constructor constructor = null;
		for (int i = 0; i < constructors.length; i++) {
			Class[] parameters = constructors[i].getParameterTypes();
			if (parameters.length != args.length)
				continue;
			boolean okConstructor = true;
			for (int j = 0; j < parameters.length; j++) {
				try {
					args[j] = cast(x[j], parameters[j]);
				} catch (Exception e) {
					okConstructor = false;
					break;
				}
			}
			if (okConstructor) {
				constructor = constructors[i];
				break;
			}
		}
		if (constructor == null) {
			System.err.println("Invalid constructor call. No satisfied constructor for " + cls + "!");
			return Var.NULL;
		}
		Object ret = null;
	    try {
	    	ret = constructor.newInstance(args);
	    }
	    catch (Exception e) {
	    	System.err.println(e.getMessage() + " occured while newInstance " + constructor + "!");
			// throw new mU.Exception(e);
	    	return Var.NULL;
	    }
	    if (ret == null) {
	    	System.err.println("Couldn't instantiate java Object by " + constructor + "!");
	    	return Var.NULL;
	    }
	    return new Var(ret);
	}
	
	@SuppressWarnings("rawtypes")
	public static VarPtr newInstance(Class cls, VarPtr[] x) {
		return newInstance(cls, VarPtr.toVar(x)).toVarPtr();
	}
	
	// @SuppressWarnings("unused")
	@SuppressWarnings("rawtypes")
	public static long _newInstance(Class cls, long[] x) {
		return newInstance(cls, VarPtr.tuple(x)).ptr;
	}
	
	@SuppressWarnings("rawtypes")
	public static Var get(Object obj, String fieldName) {
		Field field = null;
	    Class cls;
	    if (obj instanceof Class)
	    	cls = (Class) obj;
	    else
	    	cls = obj.getClass();
	    try {
	    	field = cls.getField(fieldName);
	    } catch (Exception e) {
	    	return Var.NULL;
	    }
    	if (field == null)
	        return Var.NULL;
      	Object ret = null;
      	try {
	        ret = field.get(obj);
      	} catch (Exception e1) {
      		System.err.println("Error occured while get field " + field + "!");
			// throw new mU.Exception(e);
      		return Var.NULL;
	    }
	    return cast(ret);
	}
	
	// @SuppressWarnings("unused")
	public static long _get(Object obj, String fieldName) {
		return get(obj, fieldName).toVarPtr().ptr;
	}
	
	@SuppressWarnings("rawtypes")
	public static boolean set(Object obj, String fieldName, Var x) {
		Field field = null;
	    Class cls;
	    if (obj instanceof Class)
	    	cls = (Class) obj;
	    else
	    	cls = obj.getClass();
	    try {
	    	field = cls.getField(fieldName);
	    } catch (Exception e) {
	    	return false;
	    }
    	if (field == null)
	        return false;
      	try {
			if(Modifier.isPublic(field.getModifiers()))
				field.setAccessible(true);
	        field.set(obj, cast(x, field.getClass()));
	        return true;
      	} catch (Exception e1) {
      		System.err.println("Error occured while set field " + field + "!");
			// throw new mU.Exception(e);
	    }
	    return false;
	}
	
	public static boolean set(Object obj, String fieldName, VarPtr x) {
		return set(obj, fieldName, x.toVar());
	}
	
	// @SuppressWarnings("unused")
	public static boolean _set(Object obj, String fieldName, long x) {
		return set(obj, fieldName, new VarPtr(x));
	}
	  
	public static Var cast(Object obj) {
	    if (obj == null)
	    	return Var.NULL;
	    else if (obj instanceof Var)
	    	return (Var)obj;
	    else if (obj instanceof Boolean) {
	    	Boolean t = (Boolean) obj;
	    	return new Var(t.booleanValue() ? Symbol.TRUE : Symbol.FALSE);
	    }
	    else if (obj instanceof Byte
	    		|| obj instanceof Short
	    		|| obj instanceof Integer 
	    		|| obj instanceof Long) {
	    	Number t = (Number)obj; 
	    	return new Var(BigInteger.valueOf(t.longValue()));
	    }
	    else if (obj instanceof Float
	    		|| obj instanceof Double) {
	    	Number t = (Number)obj; 
	    	return new Var(BigDecimal.valueOf(t.doubleValue()));
	    }
	    else if (obj instanceof Character) {
	    	Character t = (Character)obj;
	    	return new Var(String.valueOf(t.charValue()));
	    }
	    else if (obj instanceof String)
	    	return new Var((String)obj);
	    else if (obj instanceof VarPtr)
	    	return new Var((VarPtr)obj);
	    // else if (obj instanceof Assign || obj instanceof Value || obj instanceof Method)
	    else if (obj instanceof char[])
	    	return new Var(new String((char[])obj));
	    else if (obj instanceof byte[])
	    	return new Var(new String((byte[])obj));
	    else if (obj instanceof Object[]) {
	    	Object[] t = (Object[])obj;
	    	Var[] buf = new Var[t.length];
	    	for (int i = 0; i < t.length; ++i)
	    		buf[i] = cast(t[i]);
	    	return new Var(buf);
	    }
	    return new Var(obj);
	}
	
	// @SuppressWarnings("unused")
	public static long _cast(Object obj) {
		return cast(obj).toVarPtr().ptr;
	}

	@SuppressWarnings({ "rawtypes", "unchecked" })
	public static Number cast(Number x, Class clazz) {
		if (Number.class.isAssignableFrom(clazz))
			// return (Number)clazz.cast(x);
			return x;
		if (clazz.isPrimitive()) {
			if (clazz == Byte.TYPE)
				return new Byte(x.byteValue());
			else if (clazz == Short.TYPE)
				return new Short(x.shortValue());
			if (clazz == Integer.TYPE)
				return new Integer(x.intValue());
			else if (clazz == Long.TYPE)
				return new Long(x.longValue());
			else if (clazz == Float.TYPE)
				return new Float(x.floatValue());
			else if (clazz == Double.TYPE)
				return new Double(x.doubleValue());
		} else if (clazz.isAssignableFrom(Number.class)) {
		    if (clazz.isAssignableFrom(Byte.class))
		    	return new Byte(x.byteValue());
		    else if (clazz.isAssignableFrom(Short.class))
		    	return new Short(x.shortValue());
			if (clazz.isAssignableFrom(Integer.class))
				return new Integer(x.intValue());
			else if (clazz.isAssignableFrom(Long.class))
				return new Long(x.longValue());
			else if (clazz.isAssignableFrom(Float.class))
				return new Float(x.floatValue());
			else if (clazz.isAssignableFrom(Double.class))
				return new Double(x.doubleValue());
		}
		// return x;
		return null;
	}
	
	@SuppressWarnings("rawtypes")
	public static Number cast(double d, Class cls) {
		return cast(new Double(d), cls);
	}
	
	@SuppressWarnings({ "rawtypes", "unchecked" })
	public static Object cast(Var var, Class clazz) {
	    if (clazz.isAssignableFrom(Var.class))
	    	return var; 
	    boolean okType = true;
	    int type = var.type;
	    Object val = var.val;
	    if (type == Var.SYMBOL) {
	    	Symbol t = (Symbol)val;
	    	if (t == Symbol.NULL)
		    	return null;
		    if (t == Symbol.TRUE || t == Symbol.FALSE) {
		    	if (clazz.isPrimitive()) {
		    		if (clazz != Boolean.TYPE)
		    			okType = false;
		    	} else if (!clazz.isAssignableFrom(Boolean.class))
		    		okType = false;
		    	if (okType)
		    		return new Boolean(t == Symbol.TRUE);
		    }
	    } else if (type == Var.TUPLE) {
		    if (clazz.isArray()) {
		    	Class componentCls = clazz.getComponentType();
		    	Var[] t = (Var[])val;
//		    	Object[] r = new Object[t.length]; Object[] can't cast to e.g. Integer[] 
		    	Object[] r = (Object[])Array.newInstance(componentCls, t.length);
		    	for (int i = 0; i < t.length; ++i)
		    		r[i] = (String)cast(t[i], componentCls);
		    	return r;
		    } else if (!clazz.isAssignableFrom(Var[].class))
		    	okType = false;
		    if (okType)
		    	return (Var[])val;
	    } else if (type == Var.INTEGER || type == Var.REAL) {
	    	Number t = cast((Number)val, clazz);
	    	if (t != null)
	    		return t;
			okType = false;
	    } else if (type == Var.STRING) {
	    	if (clazz.isPrimitive()) {
				if (clazz == Character.TYPE)
					return new Character(((String)val).charAt(0));
				okType = false;
	    	} else if (clazz.isArray()) {
	    		Class componentCls = clazz.getComponentType();
	    		if (componentCls == Character.class)
	    			return ((String)val).toCharArray();
	    		else if (componentCls == Byte.class)
	    			return ((String)val).getBytes();
	    		okType = false;
	    	} else if (!clazz.isAssignableFrom(String.class))
	    		okType = false;
	    	if (okType)
	    		return (String)val;
	    }
	    else if (type == Var.JAVAOBJECT) {
	    	if (!clazz.isAssignableFrom(val.getClass())) {
	    		/*if (val instanceof Number) {
	    			Number t = cast((Number)val, clazz);
			    	if (t != null)
			    		return clazz.cast(t);
	    		}
	    		*/
	    		okType = false;
	    	}
	    	if (okType)
	    		return clazz.cast(val);
	    }
	    else if (type == Var.VARPTR) {
	    	if (!clazz.isAssignableFrom(VarPtr.class))
	    		okType = false;
	    	if (okType)
	    		return (VarPtr)val;
	    }
	    if (!okType)
	    	throw new IllegalArgumentException("Can't cast from " 
	    			+ var.type() + '[' + val.getClass() + "] to " + clazz + "!");
	    return val;
	}
	
	@SuppressWarnings("rawtypes")
	public static Object cast(VarPtr x, Class cls) {
		return cast(x.toVar(), cls);
	}
	
	// @SuppressWarnings("unused")
	@SuppressWarnings("rawtypes")
	public static Object _cast(long x, Class cls) {
		Object r = null;
		try {
			r = cast(new VarPtr(x), cls);
		} catch (Exception e) {
			System.err.println(e.getMessage());
		}
		return r;
	}
	
}
