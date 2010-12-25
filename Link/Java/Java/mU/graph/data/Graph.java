package mU.graph.data;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;

public class Graph implements Serializable{

	private static final long serialVersionUID = -8477043365583088356L;
	
	public String name = null;
	public int type;
	
	public static int G2D=0;
	public static int G3D=1;
	
	public void setName(String name){
		this.name = name;
	}
	
	/*public static Graph getGraphFromBoxForm(byte[] buffer){
		try{
			Graph graph;
			ByteArrayOutputStream baos = new ByteArrayOutputStream();
			sun.misc.BASE64Decoder decoder = new sun.misc.BASE64Decoder();
			decoder.decodeBuffer(new ByteArrayInputStream(buffer), baos);
			ByteArrayInputStream bais = new ByteArrayInputStream(baos.toByteArray());
			ObjectInputStream ois = new ObjectInputStream(bais);
		    graph=(Graph)ois.readObject();
		    ois.close();
		    return graph;
		}
		catch(Exception e){
			System.out.println(e);
		}
		return null;
	}
	
	public String toString(){
		try{
			ByteArrayOutputStream baos = new ByteArrayOutputStream();
			ObjectOutputStream oos = new ObjectOutputStream(baos);
		    oos.writeObject(this);
		    oos.close();
		    sun.misc.BASE64Encoder encoder = new sun.misc.BASE64Encoder();
		    ByteArrayInputStream bais = new ByteArrayInputStream(baos.toByteArray());
		    encoder.encode(bais, baos);
		    //System.out.println(baos.toString());
		    return baos.toString();
		}
		catch(Exception e){
			System.out.println(e);
		}
		return null;
	}
	*/
}
