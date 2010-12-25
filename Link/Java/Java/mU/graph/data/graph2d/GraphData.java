package mU.graph.data.graph2d;

import javax.vecmath.Point2d;

public abstract class GraphData {

	protected Point2d convertPoint(Point2d p,
			double xmin,double spx,double ymin,double spy,
			double mgx,double mgy){
		return new Point2d((p.x-xmin)/spx*(1-2*mgx)+mgx,(p.y-ymin)/spy*(1-2*mgy)+mgy);
	}
}
