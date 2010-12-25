package mU.graph.drawer.myj3d;

import java.awt.Font;

import javax.media.j3d.Transform3D;
import javax.media.j3d.TransformGroup;
import javax.vecmath.Color3f;
import javax.vecmath.Matrix3d;
import javax.vecmath.Point3d;
import javax.vecmath.Vector3d;

import com.sun.j3d.utils.geometry.Text2D;

public class TextTransformGroup extends TransformGroup {

	private static final int wordSize = 18;
	
	private Vector3d pos;
	
	public void setTextRotate(Matrix3d m){
		Transform3D trans = new Transform3D();
		trans.setTranslation(multiply(m,pos));
		this.setTransform(trans);
	}
	
	public TextTransformGroup(Point3d p){
		this.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE);
		this.setCapability(TransformGroup.ALLOW_TRANSFORM_READ);
		pos = new Vector3d(p.x,p.y,p.z);
	}
	
	public TextTransformGroup(Point3d p, String s){
		this.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE);
		this.setCapability(TransformGroup.ALLOW_TRANSFORM_READ);
		pos = new Vector3d(p.x,p.y,p.z);
		Text2D message = new Text2D(s,new Color3f(0f,0f,0f),"Serif",wordSize,Font.PLAIN);
		Transform3D t3d = new Transform3D();
		t3d.setTranslation(pos);
		this.setTransform(t3d);
		this.addChild(message);
	}
	
	public void addText(String s){
	}
	
	private Vector3d multiply(Matrix3d m,Vector3d v){
		Vector3d v2 = new Vector3d();
		v2.setX(m.getM00()*v.getX()+m.getM01()*v.getY()+m.getM02()*v.getZ());
		v2.setY(m.getM10()*v.getX()+m.getM11()*v.getY()+m.getM12()*v.getZ());
		v2.setZ(m.getM20()*v.getX()+m.getM21()*v.getY()+m.getM22()*v.getZ());
		return v2;
	}
}
