package mU.graph.data.graph3d.geometry;


import java.io.Serializable;
import java.util.ArrayList;

import javax.media.j3d.LineArray;
import javax.media.j3d.LineStripArray;
import javax.vecmath.Color3f;
import javax.vecmath.Point3d;

public class LineArrayData extends GeometryData implements Serializable{

	private static final long serialVersionUID = 7507539223998756230L;
	
	private int vCount=0;
	private ArrayList<Point3d> vert=new ArrayList<Point3d>();
	private ArrayList<Color3f> color=new ArrayList<Color3f>();
	
	public LineArrayData(){
		
	}
	
	public void addLine(Point3d s,Point3d e,Color3f c){
		Point3d ts=trans(s);
		Point3d te=trans(e);
		vCount+=2;
		vert.add(ts);
		vert.add(te);
		color.add(c);
		color.add(c);
	}
	
	public LineArray getGeometry(){
		LineArray la=new LineArray(vCount,LineStripArray.COORDINATES|LineStripArray.COLOR_3);
		la.setCoordinates(0,(Point3d[])vert.toArray(new Point3d[0]));
		la.setColors(0,(Color3f[])color.toArray(new Color3f[0]));
		return la;
	}
}
