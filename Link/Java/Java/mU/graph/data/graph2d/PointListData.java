package mU.graph.data.graph2d;

import java.awt.Color;
import java.io.Serializable;
import javax.vecmath.Point2d;

public class PointListData extends GraphData implements Serializable{

	private static final long serialVersionUID = -6560125594066149654L;
	
	public Point2d[] vert;
	public Color color;
	
	public PointListData(Point2d[] p,Color c,
			double xmin,double spx,double ymin,double spy,
			double mgx,double mgy){
		vert = new Point2d[p.length];
		color = c;
		for(int i=0; i<vert.length; i++){
			vert[i] = convertPoint(p[i],xmin,spx,ymin,spy,mgx,mgy);
		}	
	}
	
	public PointListData(Point2d[] p,int[] index,Color c,
			double xmin,double spx,double ymin,double spy,
			double mgx,double mgy){
		vert = new Point2d[index.length];
		color = c;
		for(int i=0; i<index.length; i++){
			vert[i] = convertPoint(p[index[i]],xmin,spx,ymin,spy,mgx,mgy);
		}	
	}
}
