package mU.graph.axemaker;

import java.text.DecimalFormat;

import mU.graph.data.graph2d.axis.ValuedRulerElement;
import mU.graph.data.graph3d.Margin;
import mU.graph.data.graph3d.element.axis.Axis3D;
import mU.graph.data.graph3d.element.axis.AxisX3D;
import mU.graph.data.graph3d.element.axis.AxisY3D;
import mU.graph.data.graph3d.element.axis.AxisZ3D;


public class AxeCubeMaker {
	
	private Axis3D[] axis = new Axis3D[3];
	private double[] span = new double[3];
	private final int[] basenum = new int[]{4,4,2};
	private boolean[] zeroIn = new boolean[3];
	private int[] amount = new int[3];
	private double[] delta = new double[3];
	private DecimalFormat[] format = new DecimalFormat[3];
	private double[] min = new double[3];
	private double[] max = new double[3];
	private double[] start1;
	private double[] end1;
	private double[] start2;
	private double[] end2;
	
	public AxeCubeMaker(double inputxmin,double inputxmax,
			double inputymin,double inputymax,
			double inputzmin,double inputzmax,
			Margin mgShape,Margin mgAxe){
		
		double[] inputmin = new double[]{inputxmin,inputymin,inputzmin};
		double[] inputmax = new double[]{inputxmax,inputymax,inputzmax};
		start1 = new double[]{mgShape.xstart,mgShape.ystart,mgShape.zstart};
		end1 = new double[]{mgShape.xend,mgShape.yend,mgShape.zend};
		start2 = new double[]{mgAxe.xstart,mgAxe.ystart,mgAxe.zstart};
		end2 = new double[]{mgAxe.xend,mgAxe.yend,mgAxe.zend};
		
		axis[0] = new AxisX3D();
		axis[1] = new AxisY3D();
		axis[2] = new AxisZ3D();
		
		for(int i=0; i<3; i++){
			span[i] = inputmax[i]-inputmin[i];
		
			if(inputmax[i]*inputmin[i] <= 0) 
				zeroIn[i] = true;
			else{
				zeroIn[i] = false;
			}
			
			if(zeroIn[i]){
				int nat,negnat,posnat;
				double len = Math.log10((inputmax[i]-inputmin[i])/basenum[i]);
				amount[i] = (int)(Math.rint(len));
				delta[i] = Math.pow(10,amount[i]);
				format[i] = getFormat(amount[i]);
				negnat = (int)(Math.rint((-inputmin[i])/delta[i]));
				posnat = (int)(Math.rint((inputmax[i])/delta[i]));
				nat = negnat+posnat;
				while(nat>=15 && ((negnat%10==0 && posnat%5==0) || (negnat%5==0 && posnat%10==0))){
					negnat /= 5;
					posnat /= 5;
					nat = negnat+posnat;
					delta[i] *= 5;
				}
				while(nat>6){
					negnat /= 2;
					posnat /= 2;
					nat = negnat+posnat;
					delta[i] *= 2;
				}
				negnat = (int)(Math.rint((-inputmin[i])/delta[i]));
				posnat = (int)(Math.rint((inputmax[i])/delta[i]));
				nat = negnat+posnat;
				
				max[i] = Math.max(posnat*delta[i],inputmax[i]);
				min[i] = Math.min(-negnat*delta[i],inputmin[i]);
				span[i] = max[i]-min[i];
				
				for(int j=-negnat; j<=posnat; j++){
					axis[i].rulers.add(new ValuedRulerElement(format[i].format(j*delta[i]),(end1[i]-start1[i])/span[i]*(j*delta[i]-min[i])+start1[i],true));
				}
				//zero[i] = (1-mgd1[i]-mgu1[i])/span[i]*(0-min[i])+mgd1[i];
			}
			else if(inputmin[i] > 0){
				int nat,negnat,posnat;
				double len = Math.log10((inputmax[i]-inputmin[i])/basenum[i]);
				amount[i] = (int)(Math.rint(len));
				delta[i] = Math.pow(10,amount[i]);
				format[i] = getFormat(amount[i]);
				negnat = (int)(Math.rint((inputmin[i])/delta[i]));
				posnat = (int)(Math.rint((inputmax[i])/delta[i]));
				nat = posnat-negnat;
				while(nat>=15 && ((negnat%10==0 && posnat%5==0) || (negnat%5==0 && posnat%10==0))){
					negnat /= 5;
					posnat /= 5;
					nat = posnat-negnat;
					delta[i] *= 5;
				}
				while(nat>6){
					negnat /= 2;
					posnat /= 2;
					nat = posnat-negnat;
					delta[i] *= 2;
				}
				negnat = (int)(Math.rint((inputmin[i])/delta[i]));
				posnat = (int)(Math.rint((inputmax[i])/delta[i]));
				nat = posnat-negnat;
				
				max[i] = Math.max(posnat*delta[i],inputmax[i]);
				min[i] = Math.min(negnat*delta[i],inputmin[i]);
				span[i] = max[i]-min[i];
				
				for(int j=negnat; j<=posnat; j++){
					axis[i].rulers.add(new ValuedRulerElement(format[i].format(j*delta[i]),(end1[i]-start1[i])/span[i]*(j*delta[i]-min[i])+start1[i],true));
				}
				//zero[i] = (1-mgd1[i]-mgu1[i])/span[i]*(negnat*delta[i]-min[i])+mgd1[i];
			}
			else if(inputmax[i] < 0){
				int nat,negnat,posnat;
				double len = Math.log10((inputmax[i]-inputmin[i])/basenum[i]);
				amount[i] = (int)(Math.rint(len));
				delta[i] = Math.pow(10,amount[i]);
				format[i] = getFormat(amount[i]);
				negnat = (int)(Math.rint((-inputmin[i])/delta[i]));
				posnat = (int)(Math.rint((-inputmax[i])/delta[i]));
				nat = negnat-posnat;
				while(nat>=15 && ((negnat%10==0 && posnat%5==0) || (negnat%5==0 && posnat%10==0))){
					negnat /= 5;
					posnat /= 5;
					nat = negnat-posnat;
					delta[i] *= 5;
				}
				while(nat>6){
					negnat /= 2;
					posnat /= 2;
					nat = posnat-negnat;
					delta[i] *= 2;
				}
				negnat = (int)(Math.rint((-inputmin[i])/delta[i]));
				posnat = (int)(Math.rint((-inputmax[i])/delta[i]));
				nat = negnat-posnat;
				
				max[i] = Math.max(-posnat*delta[i],inputmax[i]);
				min[i] = Math.min(-negnat*delta[i],inputmin[i]);
				span[i] = max[i]-min[i];
				
				for(int j=-negnat; j<=-posnat; j++){
					axis[i].rulers.add(new ValuedRulerElement(format[i].format(j*delta[i]),(end1[i]-start1[i])/span[i]*(j*delta[i]-min[i])+start1[i],true));
				}
				//zero[i] = (1-mgd1[i]-mgu1[i])/span[i]*(-negnat*delta[i]-min[i])+mgd1[i];
			}
		}
		
		axis[0].setHeight(start2[0],start2[1],start2[2]);
		axis[1].setHeight(end2[0],start2[1],start2[2]);
		axis[2].setHeight(start2[0],start2[1],start2[2]);
			
		axis[0].start = start2[0];
		axis[0].end = end2[0];
		axis[1].start = start2[1];
		axis[1].end = end2[1];
		axis[2].start = start2[2];
		axis[2].end = end2[2];
		/*for(int i=0; i<3; i++){
			axis[i].creatSubRuler();
		}*/
	}
	
	public Axis3D getAxisX(){
		return axis[0];
	}
	
	public Axis3D getAxisY(){
		return axis[1];
	}
	
	public Axis3D getAxisZ(){
		return axis[2];
	}
	
	public Axis3D[] getAxe(){
		return axis;
	}
	
	public double getxmax(){
		return max[0];
	}
	
	public double getxmin(){
		return min[0];
	}
	
	public double getymax(){
		return max[1];
	}
	
	public double getymin(){
		return min[1];
	}
	
	public double getzmax(){
		return max[2];
	}
	
	public double getzmin(){
		return min[2];
	}
	
	public double getStartx(){
		return start2[0];
	}
	
	public double getStarty(){
		return start2[1];
	}
	
	public double getStartz(){
		return start2[2];
	}
	
	public double getEndx(){
		return end2[0];
	}
	
	public double getEndy(){
		return end2[1];
	}
	
	public double getEndz(){
		return end2[2];
	}
	
	private DecimalFormat getFormat(int amount){
		DecimalFormat format = new DecimalFormat();
		if(amount >= 0){
			format = new DecimalFormat("#0");
		}
		else{
			String str = "#0.";
			for(int i=1; i<=-amount; i++){
				str += "0";
			}
			format = new DecimalFormat(str);
		}
		return format;
	}
}