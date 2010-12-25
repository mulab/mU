package mU.graph.data.graph3d;

import java.util.ArrayList;

import mU.graph.data.Graph;
import mU.graph.data.graph3d.element.axis.AxeCube;
import mU.graph.data.graph3d.shape.ShapeElement;


public class Graph3D extends Graph{

	private static final long serialVersionUID = 4248392447483596547L;
	
	public AxeCube cube;
	public ArrayList<ShapeElement> shapes;
	public boolean isLighted = false;
	
	public void setAxeCube(AxeCube cube){
		this.cube = cube;
	}
	
	public void addShape(ShapeElement shape){
		if(shapes == null){
			shapes = new ArrayList<ShapeElement>();
		}
		shapes.add(shape);
	}
}
