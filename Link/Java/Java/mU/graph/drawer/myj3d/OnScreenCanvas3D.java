package mU.graph.drawer.myj3d;

import java.awt.*;

import javax.media.j3d.*;

public class OnScreenCanvas3D extends Canvas3D {

	private static final long serialVersionUID = 4713236747610563492L;

	OffScreenCanvas3D c;

	boolean print = false;

	boolean imageReady = false;

	public OnScreenCanvas3D(GraphicsConfiguration gconfig, boolean offscreenflag){
		super(gconfig, offscreenflag);
	}

	public void setOffScreenCanvas(OffScreenCanvas3D c){
		this.c = c;
	}

	public void setImageReady(){
		imageReady = true;
	}

	public void postSwap() {
		if(imageReady && !print){
			c.print(false);
			print = true;
		}
	}
} 