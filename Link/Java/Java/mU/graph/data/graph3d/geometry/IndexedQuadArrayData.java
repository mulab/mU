package mU.graph.data.graph3d.geometry;


import javax.media.j3d.GeometryArray;
import javax.media.j3d.IndexedQuadArray;
import javax.vecmath.Color3f;
import javax.vecmath.Point3d;

import com.sun.j3d.utils.geometry.GeometryInfo;
import com.sun.j3d.utils.geometry.NormalGenerator;
import com.sun.j3d.utils.geometry.Stripifier;

import java.io.Serializable;

public class IndexedQuadArrayData extends GeometryData implements Serializable{
	
	private static final long serialVersionUID = 3014781185501765032L;

	private int[] pindex;
	private int[] cindex;
	private Point3d[] vert;
	private Color3f[] color;
	
	public IndexedQuadArrayData(){
		
	}
	
	public IndexedQuadArrayData(Point3d[] v,Color3f[] c,int[] pid,int[] cid){
		pindex = pid;
		cindex = cid;
		vert = v;
		color = c;
	}
	
	public void setPointIndex(int[] pid){
		pindex = pid;
		cindex = pid;
	}
	
	public void setVert(Point3d[] v){
		vert = v;
	}
	
	public void setColor(Color3f[] c){
		color = c;
	}
	
	public GeometryArray getGeometry(){
		IndexedQuadArray iqa = new IndexedQuadArray(vert.length, IndexedQuadArray.COORDINATES|IndexedQuadArray.COLOR_3,pindex.length);
		//IndexedQuadArray iqa = new IndexedQuadArray(vert.length, IndexedQuadArray.COORDINATES,pindex.length);
		
		Point3d[] coods = new Point3d[vert.length];
        for(int i=0; i<coods.length; i++){
        	coods[i] = trans(vert[i]);
        }
		iqa.setCoordinates(0,coods);
		
        iqa.setCoordinateIndices(0,pindex);
        
        iqa.setColors(0,color);
        iqa.setColorIndices(0,cindex);
        
        GeometryInfo gi = new GeometryInfo(iqa);
        NormalGenerator ng = new NormalGenerator();
        ng.generateNormals(gi);
        Stripifier st = new Stripifier();
        st.stripify(gi);
        return gi.getGeometryArray();
        
        //return iqa;
	}
}
