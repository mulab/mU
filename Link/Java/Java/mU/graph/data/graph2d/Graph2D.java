package mU.graph.data.graph2d;



import java.util.ArrayList;

import mU.graph.data.Graph;
import mU.graph.data.graph2d.axis.Axis;

public class Graph2D extends Graph{

	private static final long serialVersionUID = 5465051722587006026L;
	
	public Axis ax = null,ay = null;
	public ArrayList<PointListData> plists;
	public ArrayList<CurveData> curves;
	public ArrayList<LineData> lines;
	public ArrayList<ColorfulLineData> colorlines;
	public ArrayList<PolygonData> polys;
	
	public Graph2D(){
		type = Graph.G2D;
	}
	
	public void setAxis(Axis ax,Axis ay){
		this.ax = ax;
		this.ay = ay;
	}
	
	public void addPointList(PointListData plist){
		if(plists == null)
			plists = new ArrayList<PointListData>();
		plists.add(plist);
	}
	
	public void addCurve(CurveData curve){
		if(curves == null)
			curves = new ArrayList<CurveData>();
		curves.add(curve);
	}
	
	public void addLine(LineData line){
		if(lines == null)
			lines = new ArrayList<LineData>();
		lines.add(line);
	}
	
	public void addColorfulLine(ColorfulLineData line){
		if(colorlines == null)
			colorlines = new ArrayList<ColorfulLineData>();
		colorlines.add(line);
	}
	
	public void addPolygon(PolygonData poly){
		if(polys == null)
			polys = new ArrayList<PolygonData>();
		polys.add(poly);
	}
}
