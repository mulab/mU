package mU.graph.data.graph3d.element.axis;


import java.io.Serializable;

import javax.media.j3d.BranchGroup;
import javax.media.j3d.Shape3D;
import javax.vecmath.Point3d;

import mU.graph.data.graph2d.axis.RulerList;
import mU.graph.data.graph3d.element.ElementGroup;
import mU.graph.drawer.myj3d.TextTransformGroup;

public abstract class Axis3D implements Serializable,ElementGroup{

	private static final long serialVersionUID = -6754530101919871719L;
	
	public double[] height = new double[3];
	public double start,end;
	public RulerList rulers = new RulerList();
	public String name;
	
	public static double rullen = 0.01;
	public static double subrullen = 0.005;
	
	public void setHeight(double h0, double h1, double h2){
		height[0] = h0;
		height[1] = h1;
		height[2] = h2;
	}
	public void creatSubRuler(){
		this.rulers.creatSubRuler(start,end);
	}
	
	public BranchGroup getElement(){
		BranchGroup root = new BranchGroup();
		root.addChild(getLine());
		root.addChild(getRuler());
		
		return root;
	}
	
	protected Point3d addPoint(Point3d p1,Point3d p2){
		return new Point3d(p1.x+p2.x,p1.y+p2.y,p1.z+p2.z);
	}
	
	public Point3d trans(Point3d p){
		return new Point3d(2*p.x-1,2*p.y-1,2*p.z-1);
	}
	
	public abstract TextTransformGroup[] getStaticElement();
	
	protected abstract Shape3D getLine();
	
	protected abstract Shape3D getRuler();
}