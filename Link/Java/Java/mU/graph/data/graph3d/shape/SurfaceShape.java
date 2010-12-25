package mU.graph.data.graph3d.shape;


import java.io.Serializable;

import javax.media.j3d.Appearance;
import javax.media.j3d.Material;
import javax.media.j3d.PolygonAttributes;
import javax.media.j3d.Shape3D;
import javax.vecmath.Color3f;
import javax.vecmath.Point3d;

import mU.graph.data.graph3d.Margin;
import mU.graph.data.graph3d.geometry.*;

public class SurfaceShape extends ShapeElement implements Serializable{

	private static final long serialVersionUID = 5230818316521725030L;
	
	public static final int styleUpperSurface = 1;
	public static final int styleUnderSurface = 2;
	public static final int styleLightedSurface = 3;
	public static final int typeQuad = 1;
	public static final int typeTriangle = 2;
	private static final double dzUpperSurface = 0;
	private static final double dzUnderSurface = -0.002;
	
	private GeometryData geo;
	private int style;
	
	public SurfaceShape(int type,Point3d[] v,int[] index,Color3f c,
			double xmin,double spx,double ymin,double spy,double zmin,double spz,
			Margin margin){
		Point3d[] vert = margin.convertVerts(v,xmin,spx,ymin,spy,zmin,spz);
		int[] cid = new int[1];
		cid[0] = 0;
		Color3f[] color = new Color3f[1];
		color[0] = c;
		if(type == typeQuad){
			System.out.println("it's quad");
			geo = new IndexedQuadArrayData(vert,color,index,cid);
		}
		else if(type == typeTriangle){
			System.out.println("it's triangle");
			geo = new IndexedTriangleArrayData(vert,color,index,cid);
		}
	}
	
	public SurfaceShape(int style,int type,Point3d[] v,int[] index,Color3f c,
			double xmin,double spx,double ymin,double spy,double zmin,double spz,
			Margin margin){
		this.style = style;
		Point3d[] vert = margin.convertVerts(v,xmin,spx,ymin,spy,zmin,spz);
		int[] cid = new int[1];
		cid[0] = 0;
		Color3f[] color = new Color3f[1];
		color[0] = c;
		if(style == styleUpperSurface){
			for(int i=0; i<vert.length; i++)
				vert[i].z += dzUpperSurface;
			cid = index;
			color = creatBrightRainbowColors(vert,margin.zstart,margin.zend);
			System.out.println("upper surface");
		}
		else if(style == styleUnderSurface){
			for(int i=0; i<vert.length; i++)
				vert[i].z += dzUnderSurface;
			cid = index;
			color = creatDarkRainbowColors(vert,margin.zstart,margin.zend);
		}
		else if(style == styleLightedSurface){
			cid = null;
			color = null;
		}
		if(type == typeQuad){
			System.out.println("it's quad");
			geo = new IndexedQuadArrayData(vert,color,index,cid);
		}
		else if(type == typeTriangle){
			System.out.println("it's triangle");
			geo = new IndexedTriangleArrayData(vert,color,index,cid);
		}
	}
	
	public Shape3D getElement(){
		Shape3D quad = new Shape3D();
		quad.setGeometry(geo.getGeometry());
		if(style == styleUpperSurface || style == styleUnderSurface){
			Appearance appearance = new Appearance();
			//appearance.setMaterial(new Material());
			PolygonAttributes attributes = new PolygonAttributes();
			attributes.setPolygonMode(PolygonAttributes.POLYGON_FILL);
			attributes.setCullFace(PolygonAttributes.CULL_NONE);
			appearance.setPolygonAttributes(attributes);
			quad.setAppearance(appearance);
		}
		else if(style == styleLightedSurface){
			Appearance appearance = new Appearance();
			appearance.setMaterial(new Material(new Color3f(0.2f,0.2f,0.2f),new Color3f(0,0,0.5f),new Color3f(1,1,1),new Color3f(0,0,0),5));
			//appearance.setMaterial(new Material());
			PolygonAttributes attributes = new PolygonAttributes();
			attributes.setPolygonMode(PolygonAttributes.POLYGON_FILL);
			attributes.setCullFace(PolygonAttributes.CULL_NONE);
			appearance.setPolygonAttributes(attributes);
			quad.setAppearance(appearance);
		}
		return quad;
	}
	
	private Color3f[] creatBrightRainbowColors(Point3d[] vert,double sz,double ez){
		Color3f[] color = new Color3f[vert.length];
		for(int i=0; i<vert.length; i++)
			color[i] = getRainbowColor((float)((vert[i].z-sz)/(ez-sz)));
		return color;
	}
	
	private Color3f[] creatDarkRainbowColors(Point3d[] vert,double sz,double ez){
		Color3f[] color = new Color3f[vert.length];
		for(int i=0; i<vert.length; i++)
			color[i] = makeDarker(getRainbowColor((float)((vert[i].z-sz)/(ez-sz))));
		return color;
	}
	
	private Color3f getRainbowColor(float val){
		float red = 0;
		float green = 0;
		float blue = 0;
        if(val <= (float)63/255){
          val = 4 * (val - 0);
          blue = 1;
          green = val;
        }
        else if(val <= (float)127/255){
          val = 4 * (val - (float)64/255);
          green = 1;
          blue = 1 - val;
        }
        else if(val <= (float)191/255){
          val = 4 * (val - (float)128/255);
          green = 1;
          red = val;
        }
        else{
            val = 4 * (val - (float)192/255);
            red = 1;
            green = 1 - val;
        }
        return new Color3f(red,green,blue);
	}
	
	private Color3f makeDarker(Color3f c){
		return new Color3f(c.x*0.6f,c.y*0.6f,c.z*0.6f);
	}
}
