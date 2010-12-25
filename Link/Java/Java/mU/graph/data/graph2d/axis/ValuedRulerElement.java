package mU.graph.data.graph2d.axis;

import java.io.Serializable;

public class ValuedRulerElement implements Serializable{

	private static final long serialVersionUID = -5444865767745840759L;
	
	private String valueofstr;
	private double value;
	private double position;
	private boolean visible;
	
	public ValuedRulerElement(String val,double pos,boolean vis){
		this.valueofstr=val;
		this.value=Double.parseDouble(val);
		this.position=pos;
		this.visible=vis;
	}
	
	public String getString(){
		return valueofstr;
	}
	
	public double getValue(){
		return value;
	}
	
	public double getPos(){
		return position;
	}
	
	public boolean isVisible(){
		return visible;
	}
}
