package mU.graph.data.graph3d.element.axis;


import javax.media.j3d.LineArray;
import javax.media.j3d.Shape3D;
import javax.vecmath.Point3d;

import mU.graph.drawer.myj3d.TextTransformGroup;

public class AxisZ3D extends Axis3D {

	private static final long serialVersionUID = 2985278132735630080L;
	private static final Point3d wordMove = new Point3d(0,-0.1,0);

	@Override
	protected Shape3D getLine() {
		Shape3D lineshape = new Shape3D();
		LineArray line=new LineArray(2,LineArray.COORDINATES|LineArray.COLOR_3);
		Point3d[] coods = new Point3d[2];
		coods[0] = trans(new Point3d(height[0],height[1],start));
		coods[1] = trans(new Point3d(height[0],height[1],end));
		line.setCoordinates(0,coods);
		line.setColor(0,new float[]{0,0,0});
		lineshape.addGeometry(line);
		return lineshape;
	}

	@Override
	protected Shape3D getRuler() {
		Shape3D rulershape = new Shape3D();
		LineArray ruler = new LineArray(2*rulers.valuedruler.size(),LineArray.COORDINATES|LineArray.COLOR_3);
		Point3d[] cood = new Point3d[2*rulers.valuedruler.size()];
		for(int i=0; i<rulers.valuedruler.size(); i++){
			cood[2*i] = trans(new Point3d(height[0],height[1],rulers.valuedruler.get(i).getPos()));
			cood[2*i+1] = trans(new Point3d(height[0]+rullen,height[1],rulers.valuedruler.get(i).getPos()));
		}
		ruler.setCoordinates(0, cood);
		ruler.setColor(1,new float[]{0,0,0});
		rulershape.addGeometry(ruler);
		return rulershape;
	}

	@Override
	public TextTransformGroup[] getStaticElement(){
		TextTransformGroup[] ttgs = new TextTransformGroup[rulers.valuedruler.size()];
		for(int i=0; i<rulers.valuedruler.size(); i++){
			Point3d pos = trans(new Point3d(height[0],height[1],rulers.valuedruler.get(i).getPos()));
			pos = addPoint(pos,wordMove);
			ttgs[i] = new TextTransformGroup(pos,rulers.valuedruler.get(i).getString());
		}
		return ttgs;
	}
}
