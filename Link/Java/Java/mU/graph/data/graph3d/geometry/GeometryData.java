package mU.graph.data.graph3d.geometry;

import java.io.Serializable;

import javax.media.j3d.Geometry;
import javax.vecmath.Point3d;

public abstract class GeometryData implements Serializable{

	private static final long serialVersionUID = -3564912019091081255L;
	
	public Point3d trans(Point3d p){
		return new Point3d(2*p.x-1,2*p.y-1,2*p.z-1);
	}
	
	public abstract Geometry getGeometry();
}
