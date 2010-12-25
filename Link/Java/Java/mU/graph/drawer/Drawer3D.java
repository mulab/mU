package mU.graph.drawer;


import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.*;
import java.util.ArrayList;

import javax.swing.*;
import javax.imageio.stream.FileImageOutputStream;
import javax.media.j3d.*;
import javax.vecmath.*;

import mU.graph.data.graph3d.Graph3D;
import mU.graph.drawer.myj3d.*;

import com.sun.j3d.utils.behaviors.mouse.*;
import com.sun.j3d.utils.universe.*;


public class Drawer3D implements BasicDrawer,MouseBehaviorCallback{
	
	private Graph3D graph;
	private JPanel panel=new JPanel();
	private Rectangle rect;
	
	public OffScreenCanvas3D offScreenCanvas3D;
	public OnScreenCanvas3D onScreenCanvas3D;
	private TransformGroup staRoot;
	private BranchGroup objRoot;
	private ArrayList<TextTransformGroup> ttgs = new ArrayList<TextTransformGroup>();
	
	public Drawer3D(Graph3D graph,Rectangle r){
		System.setProperty("j3d.rend","d3d");
		rect = r;
		setBounds(rect);
		this.graph = graph;
		setupCanvas();
	}
	
	public void setGraph(Graph3D graph){
		this.graph = graph;
		setupCanvas();
	}
	
	public void setBounds(Rectangle r){
		panel.setBounds(r);
	}
	
	/*public boolean saveGraph(String name){
		if (graph == null) return false;
		graph.name = name;
		try{
			ByteArrayOutputStream baos = new ByteArrayOutputStream();
			ObjectOutputStream oos = new ObjectOutputStream(baos);
		    oos.writeObject(graph);
		    oos.close();
		    sun.misc.BASE64Encoder encoder = new sun.misc.BASE64Encoder();
		    ByteArrayInputStream bais = new ByteArrayInputStream(baos.toByteArray());
		    encoder.encode(bais, baos);
		}
		catch(Exception e){
			System.out.println(e);
			return false;
		}
		System.out.println("Graph successfully saved!");
		return true;
	}
	*/
	
	public JPanel exportPanel(){
		return panel;
	}
	
	private void setupCanvas(){
		/*panel.setLayout(new BorderLayout());
		canvas = new Canvas3D(SimpleUniverse.getPreferredConfiguration());
		panel.add("Center",canvas);
		Viewer viewer = new Viewer(canvas);
		
		Vector3d viewpoint = new Vector3d(-0.1,0.0,4); //��ʼ�۲��λ��        
		Transform3D t = new Transform3D();
		t.set(viewpoint);
		ViewingPlatform v = new ViewingPlatform();
		v.getViewPlatformTransform().setTransform(t);
		SimpleUniverse u = new SimpleUniverse(v,viewer);
		BranchGroup scene=creatSceneGraph();
		u.addBranchGraph(scene);*/
		
		panel.setLayout(new BorderLayout());
	    GraphicsConfiguration config = SimpleUniverse
	        .getPreferredConfiguration();

	    // create the on-screen canvas
	    onScreenCanvas3D = new OnScreenCanvas3D(config, false);
	    panel.add("Center",onScreenCanvas3D);

	    // create a simple universe
	    SimpleUniverse u = new SimpleUniverse(onScreenCanvas3D);

	    // This will move the ViewPlatform back a bit so the
	    // objects in the scene can be viewed.
	    Vector3d viewpoint = new Vector3d(-0.1,0.0,4); //��ʼ�۲��λ��        
		Transform3D t = new Transform3D();
		t.set(viewpoint);
		ViewingPlatform vp = u.getViewingPlatform();
		vp.getViewPlatformTransform().setTransform(t);

	    // create an off Screen Canvas

	    offScreenCanvas3D = new OffScreenCanvas3D(config,true);

	    // set the offscreen to match the onscreen
	    Screen3D sOn = onScreenCanvas3D.getScreen3D();
	    Screen3D sOff = offScreenCanvas3D.getScreen3D();
	    sOff.setSize(sOn.getSize());
	    sOff.setPhysicalScreenWidth(sOn.getPhysicalScreenWidth());
	    sOff.setPhysicalScreenHeight(sOn.getPhysicalScreenHeight());

	    // attach the same view to the offscreen canvas
	    View v = u.getViewer().getView();
	    v.addCanvas3D(offScreenCanvas3D);

	    // tell onscreen about the offscreen so it knows to
	    // render to the offscreen at postswap
	    onScreenCanvas3D.setOffScreenCanvas(offScreenCanvas3D);

	    BranchGroup scene=creatSceneGraph();
		u.addBranchGraph(scene);
	}
	
	private BranchGroup creatSceneGraph(){
		Transform3D t3dInit = new Transform3D(); //initial state
		Matrix3d init = new Matrix3d(new double[]{
				0.8445428292466094, 0.5354270891164377, -0.00807724014943138,
	    		-0.30807315087674614, 0.4981587066359249, 0.8105114661198191,
	    		0.43799354250605826, -0.6820232659104275, 0.5856670739250017
	    		});
	    t3dInit.setRotation(init);
	    
		TransformGroup rotRoot = new TransformGroup();
		rotRoot.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE);
		rotRoot.setCapability(TransformGroup.ALLOW_TRANSFORM_READ);
		
		rotRoot.addChild(graph.cube.getElement());
		
		if(graph.shapes != null){
			for(int i=0; i<graph.shapes.size(); i++){
				rotRoot.addChild(graph.shapes.get(i).getElement());
			}
		}
		
		staRoot = new TransformGroup();
		TextTransformGroup[] ttg;
		ttg = graph.cube.axis[0].getStaticElement();
		for(int i=0; i<ttg.length; i++){
			ttgs.add(ttg[i]);
			staRoot.addChild(ttg[i]);
		}
		ttg = graph.cube.axis[1].getStaticElement();
		for(int i=0; i<ttg.length; i++){
			ttgs.add(ttg[i]);
			staRoot.addChild(ttg[i]);
		}
		ttg = graph.cube.axis[2].getStaticElement();
		for(int i=0; i<ttg.length; i++){
			ttgs.add(ttg[i]);
			staRoot.addChild(ttg[i]);
		}
		
		for(int i=0; i<ttgs.size(); i++){
			//ttgs.get(i).setTransform(t3dInit);
			ttgs.get(i).setTextRotate(init);
		}
		
		objRoot = new BranchGroup();
		BoundingSphere bounds = new BoundingSphere(new Point3d(0.0,0.0,0.0),0.0);
		Color3f bgColor = new Color3f(1f,1f,1f);
		Background bg = new Background(bgColor);
		bg.setApplicationBounds(bounds);
		
		if(graph.isLighted){
			AmbientLight lightA = new AmbientLight(new Color3f(0,0,0));
	        lightA.setInfluencingBounds(new BoundingSphere());
	        objRoot.addChild(lightA);
	 
	        DirectionalLight lightDR = new DirectionalLight(new Color3f(1f,0.5f,0.5f),new Vector3f(1f,-0.5f,-0.5f));
	        lightDR.setInfluencingBounds(new BoundingSphere());
	        objRoot.addChild(lightDR);
	        DirectionalLight lightDG = new DirectionalLight(new Color3f(0.5f,1f,0.5f),new Vector3f(0,1f,-0.5f));
	        lightDG.setInfluencingBounds(new BoundingSphere());
	        objRoot.addChild(lightDG);
	        /*DirectionalLight lightDB = new DirectionalLight(new Color3f(0.5f,0.5f,1f),new Vector3f(0,0,-1f));
	        lightDB.setInfluencingBounds(new BoundingSphere());
	        objRoot.addChild(lightDB);*/
		}
		
		TransformGroup transTg = new TransformGroup();
		transTg.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE);
		transTg.setCapability(TransformGroup.ALLOW_TRANSFORM_READ);
	    transTg.addChild(rotRoot);
	    transTg.addChild(staRoot);
		
	    MouseRotate myMouseRotate = new MouseRotate();
        myMouseRotate.setTransformGroup(rotRoot);
        myMouseRotate.setupCallback(this);
        myMouseRotate.setSchedulingBounds(new BoundingSphere());
        transTg.addChild(myMouseRotate);
        rotRoot.setTransform(t3dInit);
        
        MouseTranslate myMouseTranslate = new MouseTranslate();
        myMouseTranslate.setTransformGroup(transTg);
        myMouseTranslate.setupCallback(this);
        myMouseTranslate.setSchedulingBounds(new BoundingSphere());
        transTg.addChild(myMouseTranslate);
        
        MouseWheelZoom myMouseWheelZoom = new MouseWheelZoom();
        myMouseWheelZoom.setTransformGroup(transTg);
        myMouseWheelZoom.setupCallback(this);
        myMouseWheelZoom.setSchedulingBounds(new BoundingSphere());
        transTg.addChild(myMouseWheelZoom);
        
		objRoot.addChild(bg);
		objRoot.addChild(transTg);
		objRoot.compile();
		return objRoot;
	}

	//Static Mouse Rotate
	@Override
	public void transformChanged(int type, Transform3D transform) {
		//System.out.println("transformChanged");
		if(type == ROTATE){
			Matrix3d m=new Matrix3d();
			transform.get(m);
			for(int i=0; i<ttgs.size(); i++){
				ttgs.get(i).setTextRotate(m);
			}
		}
	}
	
	public void exportImage(String path)
	{
		//1
		/*BufferedImage bi=(BufferedImage)panel.createImage(panel.getWidth(),panel.getHeight());
		panel.paint(bi.getGraphics());*/
		
		//2
		/*offScreenCanvas3D.renderOffScreenBuffer();
		offScreenCanvas3D.waitForOffScreenRendering();
		//System.out.println( "Rendered to offscreen" );
		BufferedImage bi = offScreenCanvas3D.getOffScreenBuffer().getImage();*/
		
		//3
		BufferedImage bi = offScreenCanvas3D.print(true);
		
		try { 
			FileImageOutputStream directory = new FileImageOutputStream(new File(path)); 
			javax.imageio.ImageIO.write(bi,"png",directory);
			directory.close();
		} 
		catch (Exception e1) { 
			e1.printStackTrace(); 
		}
		/*try{
			Thread.sleep(1000);
			Robot robot = new Robot();
			BufferedImage scrImg = robot.createScreenCapture(new Rectangle(rect.x+10,rect.y+50,rect.width-20,rect.height-60));   
			FileImageOutputStream directory = new FileImageOutputStream(new File(path)); 
			javax.imageio.ImageIO.write(scrImg,"png",directory);
			directory.close();
		} 
		catch (Exception e1) { 
			e1.printStackTrace(); 
		}*/
	}
	
}
