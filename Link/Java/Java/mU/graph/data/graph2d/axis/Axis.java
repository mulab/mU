package mU.graph.data.graph2d.axis;

import java.io.Serializable;

public class Axis implements Serializable{

	private static final long serialVersionUID = -1133593609434574849L;

	public double height;
	public double start,end;
	public RulerList rulers = new RulerList();
	public String name = null;
	
	public static double rullen=0.012;
	public static double subrullen=0.006;
	
	public void creatSubRuler(){
		this.rulers.creatSubRuler(start,end);
	}
}