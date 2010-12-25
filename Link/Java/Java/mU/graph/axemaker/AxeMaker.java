package mU.graph.axemaker;

import java.text.DecimalFormat;

import mU.graph.data.graph2d.axis.Axis;
import mU.graph.data.graph2d.axis.ValuedRulerElement;


public class AxeMaker {

	private Axis[] axis = new Axis[2];
	private double[] zero = new double[2];
	private double[] span = new double[2];
	private final int[] basenum = new int[]{8,8};
	private int[] amount = new int[2];
	private double[] delta = new double[2];
	private DecimalFormat[] format = new DecimalFormat[2];
	private boolean[] zeroIn = new boolean[2];
	private double[] min = new double[2];
	private double[] max = new double[2];
	private boolean showX;
	private boolean showY;
	
	public AxeMaker(double inputxmin,double inputxmax,double inputymin,double inputymax,double mgaxe,double mgcur,boolean showX,boolean showY){
		
		double[] inputmin = new double[]{inputxmin,inputymin};
		double[] inputmax = new double[]{inputxmax,inputymax};
		
		min[0] = inputmin[0];
		max[0] = inputmax[0];
		min[1] = inputmin[1];
		max[0] = inputmax[0];
		
		this.showX = showX;
		this.showY = showY;
		
		boolean[] showWord = new boolean[]{!showY,!showX};
		
		for(int i=0; i<2; i++){
			/*System.out.println("AxeMaker:"+"i is "+i);
			System.out.println("AxeMaker:"+"min is "+inputmin[i]);
			System.out.println("AxeMaker:"+"max is "+inputmax[i]);*/
			
			axis[i] = new Axis();
			
			span[i] = inputmax[i]-inputmin[i];
		
			if(inputmax[i]*inputmin[i] <= 0) 
				zeroIn[i] = true;
			else if(Math.min(Math.abs(inputmax[i]),Math.abs(inputmin[i]))<Math.abs(inputmax[i]-inputmin[i])/basenum[i]){
				zeroIn[i] = true;
			}
			else{
				zeroIn[i] = false;
			}
			
			if(zeroIn[i]){
				//System.out.println("AxeMaker:"+"Zero in ");
				int nat,negnat,posnat;
				double len = Math.log10((inputmax[i]-inputmin[i])/basenum[i]);
				amount[i] = (int)(Math.rint(len));
				delta[i] = Math.pow(10,amount[i]);
				format[i] = getFormat(amount[i]);
				//System.out.println("AxeMaker:"+"dx is "+delta[i]);
				if(inputmin[i] > 0){
					negnat = 0;
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
					negnat = 0;
					posnat = (int)(Math.rint((inputmax[i])/delta[i]));
					nat = negnat+posnat;
				}
				else if(inputmax[i] < 0){
					negnat = (int)(Math.rint((-inputmin[i])/delta[i]));
					posnat = 0;
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
					posnat = 0;
					nat = negnat+posnat;
				}
				else{
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
				}
				/*System.out.println("AxeMaker:"+"negnat is "+negnat);
				System.out.println("AxeMaker:"+"posnat is "+posnat);
				System.out.println("AxeMaker:"+"nat is "+nat);*/
				max[i] = Math.max(posnat*delta[i],inputmax[i]);
				min[i] = Math.min(-negnat*delta[i],inputmin[i]);
				span[i] = max[i]-min[i];
				for(int j=-negnat; j<=-1; j++){
					axis[i].rulers.add(new ValuedRulerElement(format[i].format(j*delta[i]),(1-2*mgcur)/span[i]*(j*delta[i]-min[i])+mgcur,true));
				}
				axis[i].rulers.add(new ValuedRulerElement(format[i].format(0),(1-2*mgcur)/span[i]*(0-min[i])+mgcur,showWord[i]));
				for(int j=1; j<=posnat; j++){
					axis[i].rulers.add(new ValuedRulerElement(format[i].format(j*delta[i]),(1-2*mgcur)/span[i]*(j*delta[i]-min[i])+mgcur,true));
				}
				zero[i] = (1-2*mgcur)/span[i]*(0-min[i])+mgcur;
				//System.out.println("AxeMaker:"+"dx is "+delta[i]);
			}
			else if(inputmin[i] > 0){
				//System.out.println("AxeMaker:"+" >0");
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
				
				/*System.out.println("AxeMaker:"+"negnat is "+negnat);
				System.out.println("AxeMaker:"+"posnat is "+posnat);
				System.out.println("AxeMaker:"+"nat is "+nat);*/
				max[i] = Math.max(posnat*delta[i],inputmax[i]);
				min[i] = Math.min(negnat*delta[i],inputmin[i]);
				span[i] = max[i]-min[i];
				
				axis[i].rulers.add(new ValuedRulerElement(format[i].format(negnat*delta[i]),(1-2*mgcur)/span[i]*(negnat*delta[i]-min[i])+mgcur,showWord[i]));
				for(int j=negnat+1; j<=posnat; j++){
					axis[i].rulers.add(new ValuedRulerElement(format[i].format(j*delta[i]),(1-2*mgcur)/span[i]*(j*delta[i]-min[i])+mgcur,true));
				}
				zero[i] = (1-2*mgcur)/span[i]*(negnat*delta[i]-min[i])+mgcur;
				//System.out.println("AxeMaker:"+"dx is "+delta[i]);
			}
			else if(inputmax[i] < 0){
				//System.out.println("AxeMaker:"+" <0");
				int nat,negnat,posnat;
				double len = Math.log10((inputmax[i]-inputmin[i])/basenum[i]);
				amount[i] = (int)(Math.rint(len));
				delta[i] = Math.pow(10,amount[i]);
				format[i] = getFormat(amount[i]);
				//System.out.println("AxeMaker:"+"former delta is "+delta[i]);
				negnat = (int)(Math.rint((-inputmin[i])/delta[i]));
				posnat = (int)(Math.rint((-inputmax[i])/delta[i]));
				nat = negnat-posnat;
				/*System.out.println("AxeMaker:"+"former delta is "+delta[i]);
				System.out.println("AxeMaker:"+"former negnat is "+negnat);
				System.out.println("AxeMaker:"+"former posnat is "+posnat);
				System.out.println("AxeMaker:"+"former nat is "+nat);*/
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
				
				/*System.out.println("AxeMaker:"+"negnat is "+negnat);
				System.out.println("AxeMaker:"+"posnat is "+posnat);
				System.out.println("AxeMaker:"+"nat is "+nat);*/
				max[i] = Math.max(-posnat*delta[i],inputmax[i]);
				min[i] = Math.min(-negnat*delta[i],inputmin[i]);
				span[i] = max[i]-min[i];
				
				axis[i].rulers.add(new ValuedRulerElement(format[i].format(-negnat*delta[i]),(1-2*mgcur)/span[i]*(-negnat*delta[i]-min[i])+mgcur,showWord[i]));
				for(int j=-negnat+1; j<=-posnat; j++){
					axis[i].rulers.add(new ValuedRulerElement(format[i].format(j*delta[i]),(1-2*mgcur)/span[i]*(j*delta[i]-min[i])+mgcur,true));
				}
				zero[i] = (1-2*mgcur)/span[i]*(-negnat*delta[i]-min[i])+mgcur;
				//System.out.println("AxeMaker:"+"dx is "+delta[i]);
			}
		}
		
		axis[0].height=zero[1];
		axis[1].height=zero[0];
		for(int i=0; i<2; i++){
			axis[i].start = mgaxe;
			axis[i].end = 1-mgaxe;
			axis[i].creatSubRuler();
		}
	}
	
	public void setLabel(String xstr,String ystr){
		axis[0].name = xstr;
		axis[1].name = ystr;
	}
	
	public Axis getAxisX(){
		if(showX)
			return axis[0];
		else
			return null;
	}
	
	public Axis getAxisY(){
		if(showY)
			return axis[1];
		else
			return null;
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
