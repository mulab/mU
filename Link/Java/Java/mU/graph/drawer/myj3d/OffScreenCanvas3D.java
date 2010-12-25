package mU.graph.drawer.myj3d;

import java.awt.GraphicsConfiguration;
import java.awt.image.BufferedImage;
import javax.media.j3d.*;

public class OffScreenCanvas3D extends Canvas3D {

	private static final long serialVersionUID = -1603728127954434804L;

	Raster drawRaster;

	boolean printing = false;
	
	public OffScreenCanvas3D(GraphicsConfiguration gconfig,boolean offscreenflag){
		super(gconfig, offscreenflag);
	}
	
	public BufferedImage print(boolean toWait){
		if(!toWait)
			printing = true;
		BufferedImage bImage = new BufferedImage(500, 500, BufferedImage.TYPE_INT_ARGB);
		
		ImageComponent2D buffer = new ImageComponent2D(
		ImageComponent.FORMAT_RGBA, bImage);
		buffer.setCapability(ImageComponent2D.ALLOW_IMAGE_READ);
		
		this.setOffScreenBuffer(buffer);
		this.renderOffScreenBuffer();
		
		if (toWait){
			this.waitForOffScreenRendering();
		}
		return this.getOffScreenBuffer().getImage();
	}
}