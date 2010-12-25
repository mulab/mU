package mU.graph.data.graph2d;

import java.awt.Color;
import java.io.Serializable;
import javax.vecmath.Point2d;

public class ColorfulLineData extends GraphData implements Serializable{

	private static final long serialVersionUID = 1964155029785989463L;
	
	public Point2d[] vert;
	public Color[] color;
	
	public ColorfulLineData(Point2d[] data,Color[] c,
			double xmin,double spx,double ymin,double spy,
			double mgx,double mgy){
		vert = new Point2d[data.length];
		for(int i=0; i<vert.length; i++){
			vert[i] = convertPoint(data[i],xmin,spx,ymin,spy,mgx,mgy);
		}
		color = c;
	}
	
	//graphics complex line
	public ColorfulLineData(Point2d[] data,int[] index,Color[] c,
			double xmin,double spx,double ymin,double spy,
			double mgx,double mgy){
		vert = new Point2d[index.length];
		for(int i=0; i<vert.length; i++){
			vert[i] = convertPoint(data[index[i]],xmin,spx,ymin,spy,mgx,mgy);
		}
		color = c;
	}
}
