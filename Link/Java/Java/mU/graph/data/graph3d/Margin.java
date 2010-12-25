package mU.graph.data.graph3d;

import javax.vecmath.Point3d;

public class Margin {
	
	public double xstart;
	public double xend;
	public double ystart;
	public double yend;
	public double zstart;
	public double zend;
	
	public Margin(double sx,double ex,double sy,double ey,double sz,double ez){
		xstart = sx;
		xend = ex;
		ystart = sy;
		yend = ey;
		zstart = sz;
		zend = ez;
	}
	
	public Point3d[] convertVerts(Point3d[] v,
			double xmin,double spx,double ymin,double spy,double zmin,double spz){
		Point3d[] vert = new Point3d[v.length];
		for(int i=0; i<vert.length; i++)
			vert[i] = convertVert(v[i],xmin,spx,ymin,spy,zmin,spz);
		return vert;
	}
	
	public Point3d convertVert(Point3d v,
			double xmin,double spx,double ymin,double spy,double zmin,double spz){
		return new Point3d((v.x-xmin)/spx*(xend-xstart)+xstart,
				(v.y-ymin)/spy*(yend-ystart)+ystart,
				(v.z-zmin)/spz*(zend-zstart)+zstart);
	}
}
