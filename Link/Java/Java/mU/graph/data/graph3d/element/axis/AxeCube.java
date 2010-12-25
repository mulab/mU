package mU.graph.data.graph3d.element.axis;


import java.io.Serializable;

import javax.media.j3d.BranchGroup;
import javax.media.j3d.Shape3D;
import javax.vecmath.Color3f;
import javax.vecmath.Point3d;

import mU.graph.axemaker.AxeCubeMaker;
import mU.graph.data.graph3d.element.ElementGroup;
import mU.graph.data.graph3d.geometry.LineArrayData;

public class AxeCube implements Serializable,ElementGroup{

	private static final long serialVersionUID = 3242543462688039860L;
	
	private LineArrayData line;
	public Axis3D[] axis;
	private static Color3f color0=new Color3f(0.8f,0.8f,0.8f);
	
	public AxeCube(AxeCubeMaker acmak){
		line = new LineArrayData();
		axis = acmak.getAxe();
		
		double sx = acmak.getStartx();
		double ex = acmak.getEndx();
		double sy = acmak.getStarty();
		double ey = acmak.getEndy();
		double sz = acmak.getStartz();
		double ez = acmak.getEndz();

		line.addLine(new Point3d(sx,sy,sz),new Point3d(sx,ey,sz),color0);
		line.addLine(new Point3d(sx,sy,ez),new Point3d(sx,ey,ez),color0);
		line.addLine(new Point3d(sx,sy,ez),new Point3d(ex,sy,ez),color0);
		line.addLine(new Point3d(sx,ey,sz),new Point3d(sx,ey,ez),color0);
		line.addLine(new Point3d(sx,ey,sz),new Point3d(ex,ey,sz),color0);
		line.addLine(new Point3d(sx,ey,ez),new Point3d(ex,ey,ez),color0);
		line.addLine(new Point3d(ex,sy,sz),new Point3d(ex,sy,ez),color0);
		line.addLine(new Point3d(ex,sy,ez),new Point3d(ex,ey,ez),color0);
		line.addLine(new Point3d(ex,ey,sz),new Point3d(ex,ey,ez),color0);
	}
	
	public BranchGroup getElement(){
		BranchGroup root=new BranchGroup();
		Shape3D lineshape=new Shape3D();
		lineshape.addGeometry(line.getGeometry());
		root.addChild(lineshape);
		for(int i=0; i<3; i++){
			root.addChild(axis[i].getElement());
		}
		return root;
	}
}
