package mU.graph.data.graph2d;

import java.awt.Color;
import java.io.Serializable;
import java.util.ArrayList;
import javax.vecmath.Point2d;

public class CurveData extends GraphData implements Serializable{

	private static final long serialVersionUID = 8894690461707708159L;
	
	public ArrayList<Point2d> vert=new ArrayList<Point2d>();
	public ArrayList<Point2d> ctrl=new ArrayList<Point2d>();
	public Color color;
	
	public CurveData(){
		
	}
			
	public CurveData(Point2d[] data,Point2d[] control,Color c,
			double xmin,double spx,double ymin,double spy,
			double mgx,double mgy){
		for(int i=0; i<data.length; i++){
			vert.add(convertPoint(data[i],xmin,spx,ymin,spy,mgx,mgy));
		}
		for(int i=0; i<control.length; i++){
			ctrl.add(convertPoint(control[i],xmin,spx,ymin,spy,mgx,mgy));
		}
		color=c;
	}
	
	//graphics complex curve
	public CurveData(Point2d[] data,int[] pid,int[] cid,Color c,
			double xmin,double spx,double ymin,double spy,
			double mgx,double mgy){
		for(int i=0; i<pid.length; i++){
			vert.add(convertPoint(data[pid[i]],xmin,spx,ymin,spy,mgx,mgy));
		}
		for(int i=0; i<cid.length; i++){
			ctrl.add(convertPoint(data[cid[i]],xmin,spx,ymin,spy,mgx,mgy));
		}
		color=c;
	}
	
	public void addPoint(Point2d p){
		int i;
		for(i=0;i<vert.size() && vert.get(i).x<p.x;i++){}
		vert.add(i,p);
	}	
}
