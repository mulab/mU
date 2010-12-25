package mU.graph.data.graph3d.geometry;


import javax.media.j3d.IndexedLineStripArray;
import javax.vecmath.Color3f;
import javax.vecmath.Point3d;
import java.io.Serializable;

public class IndexedLineStripArrayData extends GeometryData implements Serializable{
	
	private static final long serialVersionUID = 3014781185501765032L;

	private int[] stripIndex;
	private int[] colorIndex;
	private Point3d[] vert;
	private Color3f[] color;
	
	public IndexedLineStripArrayData(Point3d[] v,Color3f[] c,int[] pid,int[] cid){
		vert = v;
		color = c;
		stripIndex = pid;
		colorIndex = cid;
	}
	
	public IndexedLineStripArrayData(){
		
	}
	
	public void setIndex(int[] pid){
		stripIndex = pid;
		colorIndex = pid;
	}
	
	public void setVert(Point3d[] v){
		vert = v;
	}
	
	public void setColor(Color3f[] c){
		color = c;
	}
	
	public IndexedLineStripArray getGeometry(){
		int[] stripIndexCounts = new int[1];
		stripIndexCounts[0] = stripIndex.length;
		IndexedLineStripArray iqa = new IndexedLineStripArray(vert.length, IndexedLineStripArray.COORDINATES|IndexedLineStripArray.COLOR_3,stripIndex.length,stripIndexCounts);
        
		Point3d[] coods = new Point3d[vert.length];
        for(int i=0; i<coods.length; i++){
        	coods[i] = trans(vert[i]);
        }
		iqa.setCoordinates(0,coods);
		
    	iqa.setCoordinateIndices(0,stripIndex);
        
    	//System.out.println("draw line: "+"id num is "+stripIndex.length);
        iqa.setColors(0,color);
    	iqa.setColorIndices(0,colorIndex);
        
        return iqa;
	}
}
