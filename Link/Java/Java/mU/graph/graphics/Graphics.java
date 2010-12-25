package mU.graph.graphics;

import java.awt.Color;

import javax.vecmath.Color3f;
import javax.vecmath.Point2d;
import javax.vecmath.Point3d;

import mU.Symbol;
import mU.Var;
// import mU.parser.mexpr.Var;
// import mU.parser.mexpr.MString;
// import mU.parser.mexpr.MSymbol;

public class Graphics {
	
	protected static final Symbol GraphicsComplex = Symbol.system("GraphicsComplex");
	protected static final Symbol PlotRange = Symbol.system("PlotRange");
	protected static final Symbol FlatSpace = Symbol.system("FlatSpace");
	protected static final Symbol Shape = Symbol.system("Shape");
	protected static final Symbol Options = Symbol.system("Options");
	protected static final Symbol Color3f = Symbol.system("Color3f");
	protected static final Symbol Color255 = Symbol.system("Color255");
	protected static final Symbol Axes = Symbol.system("Axes");
	protected static final Symbol AxesLabel = Symbol.system("AxesLabel");
	protected static final Symbol GraphLabel = Symbol.system("GraphLabel");
	protected static final Symbol True = Symbol.system("True");
	protected static final Symbol Hue = Symbol.system("Hue");
	protected static final Symbol Mesh = Symbol.system("Mesh");
	protected static final Symbol DoubleSurface = Symbol.system("DoubleSurface");
	protected static final Symbol Surface = Symbol.system("Surface");
	protected static final Symbol LightedSurface = Symbol.system("LightedSurface");
	protected static final Symbol Polygon = Symbol.system("Polygon");
	protected static final Symbol Line = Symbol.system("Line");
	protected static final Symbol ColorfulLine = Symbol.system("ColorfulLine");
	protected static final Symbol PointList = Symbol.system("PointList");
	protected static final Symbol QuadArray = Symbol.system("QuadArray");
	protected static final Symbol TriangleArray = Symbol.system("TriangleArray");
	protected static final Symbol Curve = Symbol.system("Curve");
	protected static final Symbol Real = Symbol.system("Real");
	protected static final Symbol Int = Symbol.system("Integer");
	protected static final Symbol Minus = Symbol.system("Minus");
	
	protected static int[] readIndex(Var expr){
		//System.out.println("read index "+expr);
		int[] index = new int[expr.length() - 1];
		for(int i=0; i<index.length; i++){
			index[i] = readInteger(expr.tuple(i + 1));
		}
		return index;
	}
	
	protected static Point2d[] readVerts2d(Var expr){
//		System.out.println("read verts "+expr);
		Point2d[] vert = new Point2d[expr.length() - 1];
		for(int i=0; i<vert.length; i++){
			vert[i] = readPoint2d(expr.part(i + 1));
		}
		return vert;
	}
	
	protected static Point2d readPoint2d(Var expr){
//		System.out.println("read point "+expr);
		return new Point2d(readDouble(expr.part(1)),readDouble(expr.part(2)));
	}
	
	protected static Point3d[] readVerts3d(Var expr){
		//System.out.println("read verts "+expr);
		Point3d[] vert = new Point3d[expr.length() - 1];
		for(int i=0; i<vert.length; i++){
			vert[i] = readPoint3d(expr.part(i + 1));
		}
		return vert;
	}
	
	protected static Point3d readPoint3d(Var expr){
		//System.out.println("read point "+expr);
		return new Point3d(readDouble(expr.part(1)),readDouble(expr.part(2)),readDouble(expr.part(3)));
	}
	
	protected static Color3f readColor3f(Var expr){
		return new Color3f((float)readDouble(expr.part(1)),(float)readDouble(expr.part(2)),(float)readDouble(expr.part(3)));
	}
	
	protected static Color readColor3fToColor255(Var expr){
		System.out.println("color is "+(int)(readDouble(expr.part(1))*255)+","+(int)(readDouble(expr.part(2))*255)+","+(int)(readDouble(expr.part(3))*255));
		return new Color((int)(readDouble(expr.part(1))*255),(int)(readDouble(expr.part(2))*255),(int)(readDouble(expr.part(3))*255));
	}
	
	protected static Color3f readColor255ToColor3f(Var expr){
		return new Color3f(((float)readInteger(expr.part(1)))/255,((float)readInteger(expr.part(2)))/255,((float)readInteger(expr.part(3)))/255);
	}
	
	protected static Color readColor255(Var expr){
		return new Color(readInteger(expr.part(1)),readInteger(expr.part(2)),readInteger(expr.part(3)));
	}
	
	protected static Color[] readColor255s(Var expr){
		Color[] colors = new Color[expr.length()];
		for(int i=1; i<=expr.length(); i++)
			colors[i-1] = readColor255(expr.part(i));

		return colors;
	}
	
	protected static Color[] readColor3fsToColor255s(Var expr){
		Color[] colors = new Color[expr.length()];
		for(int i=1; i<=expr.length(); i++)
			colors[i-1] = readColor3fToColor255(expr.part(i));

		return colors;
	}
	
	protected static Color readHueToColor255(Var expr){
		return new Color(Color.HSBtoRGB((float)readDouble(expr.part(1)),(float)readDouble(expr.part(2)),(float)readDouble(expr.part(3))));
	}

	protected static Color3f readHueToColor3f(Var expr){
		Color c = new Color(Color.HSBtoRGB((float)readDouble(expr.part(1)),(float)readDouble(expr.part(2)),(float)readDouble(expr.part(3))));
		return new Color3f((float)c.getRed()/255,(float)c.getGreen()/255,(float)c.getBlue()/255);
	}
	
	protected static int readInteger(Var expr){
		return (int)readDouble(expr);
	}
	
	protected static double readDouble(Var expr){
		return expr.toDouble();
	}
	
	protected static String readString(Var expr){
		return expr.getString();
	}
}