package mU.graph.data.graph2d;

import java.awt.Color;
import java.io.Serializable;
import javax.vecmath.Point2d;

public class LineData extends GraphData implements Serializable{

	private static final long serialVersionUID = -1897844819959578712L;
	
	public Point2d[] vert;
	public Color color;
	
	public LineData(Point2d p0,Point2d p1,Color c){
		vert = new Point2d[2];
		vert[0] = p0;
		vert[1] = p1;
		color = c;
	}
	
	public LineData(Point2d[] verts,int p0,int p1,Color c){
		vert = new Point2d[2];
		vert[0] = verts[p0];
		vert[1] = verts[p1];
		color = c;
	}
	
	public LineData(Point2d[] data,Color c,
			double xmin,double spx,double ymin,double spy,
			double mgx,double mgy){
		vert = new Point2d[data.length];
		for(int i=0; i<vert.length; i++){
			vert[i] = convertPoint(data[i],xmin,spx,ymin,spy,mgx,mgy);
		}
		color=c;
	}
	
	//graphics complex line
	public LineData(Point2d[] data,int[] index,Color c,
			double xmin,double spx,double ymin,double spy,
			double mgx,double mgy){
		vert = new Point2d[index.length];
		for(int i=0; i<vert.length; i++){
			vert[i] = convertPoint(data[index[i]],xmin,spx,ymin,spy,mgx,mgy);
		}
		color=c;
	}
}
