package mU.graph.data.graph3d.shape;


import java.io.Serializable;

import javax.media.j3d.Shape3D;
import javax.vecmath.Color3f;
import javax.vecmath.Point3d;

import mU.graph.data.graph3d.Margin;
import mU.graph.data.graph3d.geometry.GeometryData;
import mU.graph.data.graph3d.geometry.IndexedLineStripArrayData;

public class LineShape extends ShapeElement implements Serializable{

	private static final long serialVersionUID = -6457128549697575692L;
	public static final int styleUpperMesh = 1;
	public static final int styleUnderMesh = 2;
	private static final double dzUpperMesh = 0.002; //delta z of upper mesh
	//private static final double dzUpperMesh = 0.000;
	private static final double dzUnderMesh = -0.004;
	//private static final double dzUnderMesh = -0.002;
	
	private GeometryData geo;
	
	public LineShape(Point3d[] v,int[] index,Color3f c,
			double xmin,double spx,double ymin,double spy,double zmin,double spz,
			Margin margin){
		Point3d[] vert = margin.convertVerts(v,xmin,spx,ymin,spy,zmin,spz);
		int[] cid = new int[index.length];
		for(int i=0; i<cid.length; i++)
			cid[i] = 0;
		Color3f[] color = new Color3f[1];
		color[0] = c;
		geo = new IndexedLineStripArrayData(vert,color,index,cid);
	}
	
	public LineShape(Point3d[] v,Color3f c,
			double xmin,double spx,double ymin,double spy,double zmin,double spz,
			Margin margin){
		Point3d[] vert = margin.convertVerts(v,xmin,spx,ymin,spy,zmin,spz);
		int[] index = new int[v.length];
		for(int i=0; i<index.length; i++)
			index[i] = i;
		int[] cid = new int[v.length];
		for(int i=0; i<cid.length; i++)
			cid[i] = 0;
		Color3f[] color = new Color3f[1];
		color[0] = c;
		geo = new IndexedLineStripArrayData(vert,color,index,cid);
	}
	
	public LineShape(int style,Point3d[] v,int[] index,Color3f c,
			double xmin,double spx,double ymin,double spy,double zmin,double spz,
			Margin margin){
		Point3d[] vert = margin.convertVerts(v,xmin,spx,ymin,spy,zmin,spz);
		if(style == styleUpperMesh){
			for(int i=0; i<vert.length; i++)
				vert[i].z += dzUpperMesh;
		}
		else if(style == styleUnderMesh){
			for(int i=0; i<vert.length; i++)
				vert[i].z += dzUnderMesh;
		}
		int[] cid = new int[1];
		cid[0] = 0;
		Color3f[] color = new Color3f[1];
		color[0] = c;
		geo = new IndexedLineStripArrayData(vert,color,index,cid);
	}
	
	public Shape3D getElement(){
		Shape3D line = new Shape3D();
		line.setGeometry(geo.getGeometry());
		return line;
	}
}
