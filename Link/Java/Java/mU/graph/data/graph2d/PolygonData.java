package mU.graph.data.graph2d;

import java.awt.Color;
import java.io.Serializable;
import java.util.ArrayList;

import javax.vecmath.Point2d;

public class PolygonData extends GraphData implements Serializable {

	private static final long serialVersionUID = -1294169904039609536L;
	
	public Point2d[] vert;
	public Color color;
	
	public PolygonData(ArrayList<Point2d> c1,ArrayList<Point2d> c2,Color c){
		vert=new Point2d[c1.size()+c2.size()];
		for(int i=0;i<c1.size();i++)
			vert[i] = new Point2d(c1.get(i).x,c1.get(i).y);
		for(int i=c2.size()-1;i>=0;i--)
			vert[c1.size()-1+i] = new Point2d(c2.get(i).x,c2.get(i).y);
		color=c;
	}
	
	public PolygonData(Point2d[] data,Color c,
			double xmin,double spx,double ymin,double spy,
			double mgx,double mgy){
		vert = new Point2d[data.length];
		for(int i=0; i<vert.length; i++){
			vert[i] = convertPoint(data[i],xmin,spx,ymin,spy,mgx,mgy);
		}
		color=c;
	}
	
	//graphics complex line
	public PolygonData(Point2d[] data,int[] index,Color c,
			double xmin,double spx,double ymin,double spy,
			double mgx,double mgy){
		vert = new Point2d[index.length];
		for(int i=0; i<vert.length; i++){
			vert[i] = convertPoint(data[index[i]],xmin,spx,ymin,spy,mgx,mgy);
		}
		color=c;
	}
}
