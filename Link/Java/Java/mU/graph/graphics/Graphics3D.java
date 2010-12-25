package mU.graph.graphics;

import javax.vecmath.Color3f;
import javax.vecmath.Point3d;

import mU.Var;
import mU.graph.axemaker.AxeCubeMaker;
import mU.graph.data.graph3d.Graph3D;
import mU.graph.data.graph3d.Margin;
import mU.graph.data.graph3d.element.axis.AxeCube;
import mU.graph.data.graph3d.shape.LineShape;
import mU.graph.data.graph3d.shape.SurfaceShape;
// import mU.parser.mexpr.Var;


public class Graphics3D extends Graphics {

	private Graph3D graph;
/*	private static double mgXY1 = 0.05;
	private static double mgFlatZu1 = 0.26;
	private static double mgFlatZd1 = 0.36;
	private static double mgNormalZu1 = 0.05;
	private static double mgNormalZd1 = 0.05;
	private static double mgXY2 = 0;
	private static double mgFlatZu2 = 0.25;
	private static double mgFlatZd2 = 0.35;
	private static double mgNormalZu2 = 0;
	private static double mgNormalZd2 = 0;*/
	private double xmin,xmax,ymin,ymax,zmin,zmax,spx,spy,spz;
	private Color3f color = new Color3f(0,0,0);
	private Margin mgShape = new Margin(0.05,0.95,0.05,0.95,0.05,0.95);
	private Margin mgAxe = new Margin(0,1,0,1,0,1);
	
	public Graphics3D(Var expr){
		
		graph = new Graph3D();
		Var graphics = expr.part(1);
		Var options = expr.part(2);
		//System.out.println("options is "+options);
		boolean rangeSet = false;
		for(int i=1; i<options.length(); i++){
			Var op = options.part(i);
			if(op.tupleQ(PlotRange)){
				rangeSet = true;
				//System.out.println("range is "+range);
				xmin = readDouble(op.part(1).part(1));
				xmax = readDouble(op.part(1).part(2));
				ymin = readDouble(op.part(2).part(1));
				ymax = readDouble(op.part(2).part(2));
				zmin = readDouble(op.part(3).part(1));
				zmax = readDouble(op.part(3).part(2));
				spx = xmax-xmin;
				spy = ymax-ymin;
				spz = zmax-zmin;
			}
			else if(op.tupleQ(FlatSpace)){
				mgShape = new Margin(0.05,0.95,0.05,0.95,0.26,0.64);
				mgAxe = new Margin(0,1,0,1,0.25,0.65);
			}
			else if(op.tupleQ(Color3f)){
				color = readColor3f(op);
			}
		}
		
		if(!rangeSet){
			
		}

		AxeCubeMaker acmak = new AxeCubeMaker(xmin,xmax,ymin,ymax,zmin,zmax,mgShape,mgAxe);
		graph.setAxeCube(new AxeCube(acmak));
		
		if(graphics.tupleQ(Shape)){
			int shapenum = graphics.length();
			for(int i=1; i<shapenum; i++)
				creatGraphics(graph,graphics.part(i));
		}
		//System.out.println("graphics made");
	}
	
	public Graph3D getGraph(){
		return graph;
	}
	
	void creatGraphics(Graph3D graph,Var expr){
		if(expr.tupleQ(GraphicsComplex)){
			Var vexpr = expr.part(1);
			Point3d[] vert = readVerts3d(vexpr);
			//System.out.println("vert num is "+vert.length);
			Var shape = expr.part(2);
			int shapenum = shape.length();
			for(int i=1; i<shapenum; i++)
				creatDirective(graph,shape.part(i),vert);
		}
		else if(expr.tupleQ(Line)){
			System.out.println("find line");
			creatLine(graph,expr);
		}
	}
	
	void creatDirective(Graph3D graph,Var expr,Point3d[] vert){
		/*if(expr.tupleQ(Var.LISTexpr)){
			Color lastHue = hue;
			hue = readColor(expr.part(1));
		}*/
		//System.out.println("directive expr is "+expr);
		if(expr.tupleQ(Line)){
			//System.out.println("find line");
			creatLineDirective(graph,expr,vert);
		}
		if(expr.tupleQ(QuadArray)){
			//System.out.println("find quad array");
			creatQuadArrayDirective(graph,expr,vert);
		}
		if(expr.tupleQ(TriangleArray)){
			//System.out.println("find quad array");
			creatTriangleArrayDirective(graph,expr,vert);
		}
	}
	
	void creatQuadArrayDirective(Graph3D graph,Var expr,Point3d[] vert){
		int exprlen = expr.length();
		int quadnum = exprlen;
		Var options = expr.part(exprlen - 1);
		Color3f color = this.color;
		int surfaceType = 0;
		//System.out.println("options is "+options);
		if(options.tupleQ(Options)){
			//System.out.println("option num is "+options.length());
			for(int i=1; i<options.length();i++){
				Var op = options.part(i);
				//System.out.println("op is "+op);
				//System.out.println("op head is "+op.head());
				if(op.tupleQ(Color3f)){
					color = readColor3f(op);
				}
				else if(op.tupleQ(Surface)){
					surfaceType = 1;
					//System.out.println("this is a surface");
				}
				else if(op.tupleQ(DoubleSurface)){
					surfaceType = 2;
					//System.out.println("this is a surface");
				}
				else if(op.tupleQ(LightedSurface)){
					surfaceType = 3;
					graph.isLighted = true;
				}
			}
			quadnum--; //the last part is not line
		}
		//System.out.println("quad num is "+quadnum);
		for(int i=1; i<quadnum; i++){
			Var quad = expr.part(i);
			int[] index = readIndex(quad);
			if(surfaceType == 2){
				System.out.println("this is a double surface");
				graph.addShape(new SurfaceShape(SurfaceShape.styleUpperSurface,SurfaceShape.typeQuad,vert,index,color,
						xmin,spx,ymin,spy,zmin,spz,mgShape));
				graph.addShape(new SurfaceShape(SurfaceShape.styleUnderSurface,SurfaceShape.typeQuad,vert,index,color,
						xmin,spx,ymin,spy,zmin,spz,mgShape));
			}
			else if(surfaceType == 1){
				System.out.println("this is a surface");
				graph.addShape(new SurfaceShape(SurfaceShape.styleUpperSurface,SurfaceShape.typeQuad,vert,index,color,
						xmin,spx,ymin,spy,zmin,spz,mgShape));
			}
			else if(surfaceType == 3){
				System.out.println("this is a lighted surface");
				graph.addShape(new SurfaceShape(SurfaceShape.styleLightedSurface,SurfaceShape.typeQuad,vert,index,color,
						xmin,spx,ymin,spy,zmin,spz,mgShape));
			}
			else{
				graph.addShape(new SurfaceShape(SurfaceShape.typeQuad,vert,index,color,
						xmin,spx,ymin,spy,zmin,spz,mgShape));
			}
		}
	}
	
	void creatTriangleArrayDirective(Graph3D graph,Var expr,Point3d[] vert){
		int exprlen = expr.length();
		int quadnum = exprlen;
		Var options = expr.part(exprlen - 1);
		Color3f color = this.color;
		int surfaceType = 0;
		//System.out.println("options is "+options);
		if(options.tupleQ(Options)){
			//System.out.println("option num is "+options.length());
			for(int i=1; i<options.length();i++){
				Var op = options.part(i);
				//System.out.println("op is "+op);
				//System.out.println("op head is "+op.head());
				if(op.tupleQ(Color3f)){
					color = readColor3f(op);
				}
				else if(op.tupleQ(Surface)){
					surfaceType = 1;
					//System.out.println("this is a surface");
				}
				else if(op.tupleQ(DoubleSurface)){
					surfaceType = 2;
					//System.out.println("this is a surface");
				}
				else if(op.tupleQ(LightedSurface)){
					surfaceType = 3;
					graph.isLighted = true;
				}
			}
			quadnum--; //the last part is not line
		}
		//System.out.println("quad num is "+quadnum);
		for(int i=1; i<quadnum; i++){
			Var quad = expr.part(i);
			int[] index = readIndex(quad);
			if(surfaceType == 2){
				System.out.println("this is a double surface");
				graph.addShape(new SurfaceShape(SurfaceShape.styleUpperSurface,SurfaceShape.typeTriangle,vert,index,color,
						xmin,spx,ymin,spy,zmin,spz,mgShape));
				graph.addShape(new SurfaceShape(SurfaceShape.styleUnderSurface,SurfaceShape.typeTriangle,vert,index,color,
						xmin,spx,ymin,spy,zmin,spz,mgShape));
			}
			else if(surfaceType == 1){
				System.out.println("this is a surface");
				graph.addShape(new SurfaceShape(SurfaceShape.styleUpperSurface,SurfaceShape.typeTriangle,vert,index,color,
						xmin,spx,ymin,spy,zmin,spz,mgShape));
			}
			else if(surfaceType == 3){
				System.out.println("this is a lighted surface");
				graph.addShape(new SurfaceShape(SurfaceShape.styleLightedSurface,SurfaceShape.typeTriangle,vert,index,color,
						xmin,spx,ymin,spy,zmin,spz,mgShape));
			}
			else{
				graph.addShape(new SurfaceShape(SurfaceShape.typeTriangle,vert,index,color,
						xmin,spx,ymin,spy,zmin,spz,mgShape));
			}
		}
	}
	
	void creatLineDirective(Graph3D graph,Var expr,Point3d[] vert){
		int exprlen = expr.length();
		int linenum = exprlen;
		Var options = expr.part(exprlen - 1);
		Color3f color = this.color;
		boolean mesh = false;
		if(options.tupleQ(Options)){
			//System.out.println("find options "+options);
			for(int i=1; i<options.length();i++){
				Var op = options.part(i);
				if(op.tupleQ(Color3f)){
					color = readColor3f(op);
				}
				if(op.tupleQ(Mesh)){
					mesh = true;
				}
			}
			linenum--; //the last part is not line
		}
		for(int i=1; i<linenum; i++){
			Var line = expr.part(i);
			int[] index = readIndex(line);
			if(mesh){
				graph.addShape(new LineShape(LineShape.styleUpperMesh,vert,index,color,
						xmin,spx,ymin,spy,zmin,spz,mgShape));
				graph.addShape(new LineShape(LineShape.styleUnderMesh,vert,index,color,
						xmin,spx,ymin,spy,zmin,spz,mgShape));
			}
			else{
				graph.addShape(new LineShape(vert,index,color,
						xmin,spx,ymin,spy,zmin,spz,mgShape));
			}
		}
	}
	
	void creatLine(Graph3D graph,Var expr){
		int exprlen = expr.length();
		int linenum = exprlen;
		Var options = expr.part(exprlen);
		Color3f color = this.color;
		if(options.tupleQ(Options)){
			//System.out.println("find options "+options);
			for(int i=1; i<options.length();i++){
				Var op = options.part(i);
				if(op.tupleQ(Color255)){
					color = readColor255ToColor3f(op);
				}
				else if(op.tupleQ(Hue)){
					color = readHueToColor3f(op);
				}
			}
			linenum--; //the last part is not line
		}
		//System.out.println("line num is "+linenum);
		for(int i=1; i<linenum; i++){
			Var line = expr.part(i);
			//System.out.println("line is "+line);
			Point3d[] vert = readVerts3d(line);
			graph.addShape(new LineShape(vert,color,
					xmin,spx,ymin,spy,zmin,spz,mgShape));
		}
	}
}
