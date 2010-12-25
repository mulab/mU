package mU.graph.graphics;

import java.awt.Color;

import javax.vecmath.Point2d;

import mU.Var;
import mU.graph.axemaker.AxeMaker;
import mU.graph.data.graph2d.*;
// import mU.parser.mexpr.Var;


public class Graphics2D extends Graphics {
	
	private Graph2D graph;
	private Color color = Color.blue;
	private boolean axeX = true;
	private boolean axeY = true;
	private double xmin,xmax,ymin,ymax,spx,spy;
	
	private final double mgcur = 0.05; //margin of curve
	// private final double mgforword = 
	private final double mgaxe = 0.01; //margin of axis
	
	public Graphics2D(Var expr){
		graph = new Graph2D();
		Var graphics = expr.part(1);
		Var options = expr.part(2);
		// System.out.println("options is "+options);
		boolean rangeSet = false;
		String xstring = null;
		String ystring = null;
		for(int i=1; i<options.length(); i++){
			Var op = options.part(i);
			if(op.tupleQ(PlotRange)){
				// System.out.println("op is "+op);
				rangeSet = true;
				// System.out.println("range is "+op);
				xmin = readDouble(op.part(1).part(1));
				xmax = readDouble(op.part(1).part(2));
				ymin = readDouble(op.part(2).part(1));
				ymax = readDouble(op.part(2).part(2));
			}
			else if(op.tupleQ(Axes)){
				axeX = (op.part(1)).sameQ(True);
				axeY = (op.part(2)).sameQ(True);
			}
			else if(op.tupleQ(AxesLabel)){
				xstring = readString(op.part(1));
				ystring = readString(op.part(2));
			}
			else if(op.tupleQ(GraphLabel)){
				graph.setName(readString(op.part(1)));
			}
			else if(op.tupleQ(Color255)){
				color = readColor255(op);
			}
			else if(op.tupleQ(Hue)){
				color = readHueToColor255(op);
			}
		}
		
		if(!rangeSet){
			
		}
		
		AxeMaker axemak = new AxeMaker(xmin,xmax,ymin,ymax,mgaxe,mgcur,axeX,axeY);
		axemak.setLabel(xstring,ystring);
		graph.setAxis(axemak.getAxisX(),axemak.getAxisY());
		xmin = axemak.getxmin();
		xmax = axemak.getxmax();
		ymin = axemak.getymin();
		ymax = axemak.getymax();
		spx = xmax-xmin;
		spy = ymax-ymin;
			
		// System.out.println("PlotCarrier:"+"xmin is "+xmin);
		// System.out.println("PlotCarrier:"+"xmax is "+xmax);
		// System.out.println("PlotCarrier:"+"ymin is "+ymin);
		// System.out.println("PlotCarrier:"+"ymax is "+ymax);
		
		if(graphics.tupleQ(Shape)){
			int shapenum = graphics.length();
			for(int i=1; i<shapenum; i++)
				creatGraphics(graph,graphics.part(i));
		}
		// System.out.println("graphics made");
	}
	
	public Graph2D getGraph(){
		return graph;
	}
	
	void creatGraphics(Graph2D graph,Var expr){
		// System.out.println("graphics expr is "+expr);
		if(expr.tupleQ(GraphicsComplex)){
			Var vexpr = expr.part(1);
			Point2d[] vert = readVerts2d(vexpr);
			//System.out.println("vert num is "+vert.length);
			Var shape = expr.part(2);
			int shapenum = shape.length();
			for(int i=1; i<shapenum; i++)
				creatDirective(graph,shape.part(i),vert);
		}
		else if(expr.tupleQ(Line)){
			// System.out.println("find line");
			creatLine(graph,expr);
		}
		else if(expr.tupleQ(ColorfulLine)){
			// System.out.println("find colorful line");
			creatColorfulLine(graph,expr);
		}
		else if(expr.tupleQ(Curve)){
			// System.out.println("find curve");
			creatCurve(graph,expr);
		}
		else if(expr.tupleQ(PointList)){
			// System.out.println("find point list");
			creatPointList(graph,expr);
		}
	}
	
	void creatDirective(Graph2D graph,Var expr,Point2d[] vert){
		// System.out.println("directive expr is "+expr);
		if(expr.tupleQ(Line)){
			// System.out.println("find line");
			creatLineDirective(graph,expr,vert);
		}
		else if(expr.tupleQ(Polygon)){
			// System.out.println("find polygon");
			creatPolygonDirective(graph,expr,vert);
		}
		else if(expr.tupleQ(QuadArray)){
			// System.out.println("find quad array");
			creatQuadArrayDirective(graph,expr,vert);
		}
		else if(expr.tupleQ(PointList)){
			// System.out.println("find line");
			creatPointListDirective(graph,expr,vert);
		}
	}
	
	void creatPointList(Graph2D graph,Var expr){
		int exprlen = expr.length();
		int pointlistnum = exprlen;
		Var options = expr.part(exprlen);
		Color color = this.color;
		if(options.tupleQ(Options)){
			// System.out.println("find options "+options);
			for(int i=1; i<options.length();i++){
				Var op = options.part(i);
				if(op.tupleQ(Color255)){
					color = readColor255(op);
				}
				else if(op.tupleQ(Hue)){
					color = readHueToColor255(op);
				}
			}
			pointlistnum--; //the last part is not line
		}
		//System.out.println("point list num is "+pointlistnum);
		for(int i=1; i<pointlistnum; i++){
			Var plist = expr.part(i);
			// System.out.println("point list is "+plist);
			Point2d[] vert = readVerts2d(plist);
			graph.addPointList(new PointListData(vert,color,xmin,spx,ymin,spy,mgcur,mgcur));
		}
	}
	
	void creatLine(Graph2D graph,Var expr){
		int exprlen = expr.length();
		int linenum = exprlen;
		Var options = expr.part(exprlen - 1);
		Color color = this.color;
		if(options.tupleQ(Options)){
			//System.out.println("find options "+options);
			for(int i=1; i<options.length();i++){
				Var op = options.part(i);
				if(op.tupleQ(Color255)){
					color = readColor255(op);
				}
				else if(op.tupleQ(Hue)){
					color = readHueToColor255(op);
				}
			}
			linenum--; //the last part is not line
		}
		for(int i=1; i<linenum; i++){
			Var line = expr.part(i);
			Point2d[] vert = readVerts2d(line);
			graph.addLine(new LineData(vert,color,xmin,spx,ymin,spy,mgcur,mgcur));
		}
	}
	
	void creatColorfulLine(Graph2D graph,Var expr){
		System.out.println("expr is "+expr);
		Var v = expr.part(1);
		Var c = expr.part(2);
		System.out.println("c is "+c);
		Point2d[] verts = readVerts2d(v);
		Color[] colors = readColor255s(c);
		graph.addColorfulLine(new ColorfulLineData(verts,colors,xmin,spx,ymin,spy,mgcur,mgcur));
	}
	
	void creatCurve(Graph2D graph,Var expr){
		int exprlen = expr.length();
		int linenum = exprlen;
		Var options = expr.part(exprlen);
		Color color = this.color;
		if(options.tupleQ(Options)){
			//System.out.println("find options "+options);
			for(int i=1; i<options.length();i++){
				Var op = options.part(i);
				if(op.tupleQ(Color255)){
					color = readColor255(op);
				}
				else if(op.tupleQ(Hue)){
					color = readHueToColor255(op);
				}
			}
			linenum--; //the last part is not line
		}
		for(int i=1; i<linenum; i++){
			Var line = expr.part(i);
			Point2d[] vert = readVerts2d(line.part(1));
			Point2d[] ctrl = readVerts2d(line.part(2));
			graph.addCurve(new CurveData(vert,ctrl,color,xmin,spx,ymin,spy,mgcur,mgcur));
		}
	}
	
	void creatLineDirective(Graph2D graph,Var expr,Point2d[] vert){
		System.out.println("line:"+expr);
		int exprlen = expr.length();
		int linenum = exprlen;
		Var options = expr.part(exprlen);
		Color color = this.color;
		if(options.tupleQ(Options)){
			//System.out.println("find options "+options);
			for(int i=1; i<options.length();i++){
				Var op = options.part(i);
				if(op.tupleQ(Color255)){
					color = readColor255(op);
				}
				else if(op.tupleQ(Hue)){
					color = readHueToColor255(op);
				}
			}
			linenum--; //the last part is not line
		}
		System.out.println("line num is "+linenum);
		for(int i=1; i<linenum; i++){
			Var line = expr.part(i);
			int[] index = readIndex(line);
			graph.addLine(new LineData(vert,index,color,xmin,spx,ymin,spy,mgcur,mgcur));
		}
	}
	
	void creatCurveDirective(Graph2D graph,Var expr,Point2d[] vert){
		int exprlen = expr.length();
		int linenum = exprlen;
		Var options = expr.part(exprlen);
		Color color = this.color;
		if(options.tupleQ(Options)){
			//System.out.println("find options "+options);
			for(int i=1; i<options.length();i++){
				Var op = options.part(i);
				if(op.tupleQ(Color255)){
					color = readColor255(op);
				}
				else if(op.tupleQ(Hue)){
					color = readHueToColor255(op);
				}
			}
			linenum--; //the last part is not line
		}
		for(int i=1; i<linenum; i++){
			Var line = expr.part(i);
			int[] pindex = readIndex(line.part(1));
			int[] cindex = readIndex(line.part(1));
			graph.addCurve(new CurveData(vert,pindex,cindex,color,xmin,spx,ymin,spy,mgcur,mgcur));
		}
	}
	
	void creatPolygonDirective(Graph2D graph,Var expr,Point2d[] vert){
		System.out.println("polg:"+expr);
		int exprlen = expr.length();
		int polynum = exprlen;
		Var options = expr.part(exprlen);
		Color color = this.color;
		if(options.tupleQ(Options)){
			//System.out.println("find options "+options);
			for(int i=1; i<options.length();i++){
				Var op = options.part(i);
				if(op.tupleQ(Color255)){
					color = readColor255(op);
				}
				else if(op.tupleQ(Hue)){
					//color = readHueToColor255(op).brighter().brighter().brighter();
					color = readHueToColor255(op);
				}
			}
			polynum--;
		}
		System.out.println("poly num is "+polynum);
		for(int i=1; i<polynum; i++){
			Var poly = expr.part(i);
			int[] index = readIndex(poly);
			graph.addPolygon(new PolygonData(vert,index,color,xmin,spx,ymin,spy,mgcur,mgcur));
		}
	}
	
	void creatQuadArrayDirective(Graph2D graph,Var expr,Point2d[] vert){
		System.out.println("quads:"+expr);
		int exprlen = expr.length();
		int polynum = exprlen;
		Var options = expr.part(exprlen);
		Color color = this.color;
		if(options.tupleQ(Options)){
			//System.out.println("find options "+options);
			for(int i=1; i<options.length();i++){
				Var op = options.part(i);
				if(op.tupleQ(Color255)){
					color = readColor255(op);
				}
				else if(op.tupleQ(Hue)){
					color = readHueToColor255(op);
				}
			}
			polynum--;
		}
		System.out.println("poly num is "+polynum);
		for(int i=1; i<polynum; i++){
			Var poly = expr.part(i);
			int quadnum = poly.length();
			System.out.println("quad num is "+quadnum);
			for(int j=1; j<quadnum; j++){
				Var quad = poly.part(j);
				int[] index = readIndex(quad);
				graph.addPolygon(new PolygonData(vert,index,color,xmin,spx,ymin,spy,mgcur,mgcur));
			}
		}
	}
	
	void creatPointListDirective(Graph2D graph,Var expr,Point2d[] vert){
		int exprlen = expr.length();
		int pointlistnum = exprlen;
		Var options = expr.part(exprlen);
		Color color = this.color;
		if(options.tupleQ(Options)){
			//System.out.println("find options "+options);
			for(int i=1; i<options.length();i++){
				Var op = options.part(i);
				if(op.tupleQ(Color255)){
					color = readColor255(op);
				}
				else if(op.tupleQ(Hue)){
					color = readHueToColor255(op);
				}
			}
			pointlistnum--; //the last part is not line
		}
		//System.out.println("point list num is "+pointlistnum);
		for(int i=1; i<pointlistnum; i++){
			Var plist = expr.part(i);
			//System.out.println("point list is "+plist);
			int[] index = readIndex(plist);
			graph.addPointList(new PointListData(vert,index,color,xmin,spx,ymin,spy,mgcur,mgcur));
		}
	}
}
