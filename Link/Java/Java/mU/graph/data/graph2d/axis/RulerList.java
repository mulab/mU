package mU.graph.data.graph2d.axis;

import java.io.Serializable;
import java.util.ArrayList;

public class RulerList implements Serializable{

	private static final long serialVersionUID = -2032034255369722507L;

	public ArrayList<ValuedRulerElement> valuedruler;
	public ArrayList<Double> subruler;
	
	public RulerList(){
		valuedruler=new ArrayList<ValuedRulerElement>();
	}
	
	public void add(ValuedRulerElement ruler){
		int i;
		for(i=0;i<valuedruler.size() && valuedruler.get(i).getPos()<ruler.getPos();i++){}
		valuedruler.add(i,ruler);
	}
	
	public void creatSubRuler(double start,double end){
		subruler=new ArrayList<Double>();
		for(int i=0;i<valuedruler.size();i++){
		}
		double dl=(valuedruler.get(1).getPos()-valuedruler.get(0).getPos())/5;
		for(int i=-1;valuedruler.get(0).getPos()+i*dl>start;i--){
			subruler.add(valuedruler.get(0).getPos()+i*dl);
		}
		for(int i=0;i<valuedruler.size()-1;i++){
			for(int j=1;j<=4;j++){
				subruler.add(valuedruler.get(i).getPos()+j*dl);
			}
		}
		for(int i=1;valuedruler.get(valuedruler.size()-1).getPos()+i*dl<end;i++){
			subruler.add(valuedruler.get(valuedruler.size()-1).getPos()+i*dl);
		}
	}
}
