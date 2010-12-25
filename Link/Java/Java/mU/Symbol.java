package mU;

import java.util.HashMap;


public class Symbol {
	
	private static HashMap<String, Symbol> symbolMap;
	static {
		symbolMap = new HashMap<String, Symbol>();
	}

	private static HashMap<Symbol, String> textMap;
	static {
		textMap = new HashMap<Symbol, String>();
	}

	public static Symbol get(String text) {
		Symbol symbol = symbolMap.get(text);
		if (symbol == null) {
			symbol = new Symbol();
			symbolMap.put(text, symbol);
			textMap.put(symbol, text);
		}
		return symbol;
	}
	
	public static Symbol system(String x) {
		return get("System`" + x);
	}
	
	public static Symbol global(String x) {
		return get("Global`" + x);
	}
	
	public static final Symbol FALSE		= system("False");
	public static final Symbol GLOBAL		= get("Global");
	public static final Symbol INTEGER		= system("Integer");
	public static final Symbol JAVA			= get("Java");
	public static final Symbol JAVAOBJECT	= get("Java`Object");
	public static final Symbol KEY	 		= system("Key");
	public static final Symbol LIST			= system("List");
	public static final Symbol MINUS		= system("Minus");
	public static final Symbol NULL 		= system("Null");
	public static final Symbol OBJECT 		= system("Object");
	public static final Symbol PLUS 		= system("Plus");
	public static final Symbol RATIONAL		= system("Rational");
	public static final Symbol REAL			= system("Real");
	public static final Symbol SYMBOL 		= system("Symbol");
	public static final Symbol SYSTEM		= get("System");
	public static final Symbol STRING		= system("String");
	public static final Symbol TUPLE		= system("Tuple");
	public static final Symbol TRUE			= system("True");
	
	public Symbol() {}

	public String toString() {
		return textMap.get(this);
	} 
}