package mU.graph.drawer;


import java.awt.*;
import java.awt.geom.*;
import java.awt.image.BufferedImage;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.ByteArrayOutputStream;

import javax.imageio.stream.FileImageOutputStream;
import javax.swing.JPanel;

import mU.graph.data.graph2d.*;
import mU.graph.data.graph2d.axis.Axis;


public class Drawer2D extends JPanel implements BasicDrawer{
	
	private static final long serialVersionUID = 3040929363127087370L;
	
	private static int rxworddy = 12;
	private static int ryworddy = 5;
	private static int ryworddx = -7;
	private static int rxworddx = -3;
	
	private static int xlabeldx = -8;
	private static int xlabeldy = -5;
	private static int ylabeldx = 8;
	private static int ylabeldy = 12;
	
	private static double graphnamex = 0.52;
	private static double graphnamey = 0.15;
	
	private static int psize = 6;
	private static int wordsize = 10;
	private static BasicStroke curvestroke = new BasicStroke(1f);
	private static BasicStroke axisstroke = new BasicStroke(1f);
	private static Font rulfont = new Font("Tahoma",Font.PLAIN,12);
	private static Font namefont = new Font("Tahoma",Font.BOLD,15);
	
	private Graph2D graph;
	private Rectangle rect;
	private int w,h;
	private double graphmove;
	private boolean graphmoved = false;
	
	public Drawer2D(Graph2D graph,Rectangle r){
		super();
		rect = r;
		setBackground(Color.WHITE);
		//setBorder(BorderFactory.createBevelBorder(BevelBorder.LOWERED));
		setup(graph,r);
	}
	
	/*public Drawer2D(byte[] buffer,Rectangle r){
		try{
			ByteArrayOutputStream baos = new ByteArrayOutputStream();
			sun.misc.BASE64Decoder decoder = new sun.misc.BASE64Decoder();
			decoder.decodeBuffer(new ByteArrayInputStream(buffer), baos);
			ByteArrayInputStream bais = new ByteArrayInputStream(baos.toByteArray());
			ObjectInputStream ois = new ObjectInputStream(bais);
		    graph=(Graph2D)ois.readObject();
		    ois.close();
		}
		catch(Exception e){
			e.printStackTrace();
		}
		setup(graph,r);
	}
	*/
	
	public void paintComponent(Graphics g) {
	    Graphics2D g2 = (Graphics2D)g;
	    
	    //�����
	    g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);   
	    
	    w = getWidth();
		h = getHeight();
	    
		int longestword = 0;
		if(graph.ay != null){
			for(int i=0;i<graph.ay.rulers.valuedruler.size();i++){
				if(graph.ay.rulers.valuedruler.get(i).getString().length()>longestword)
					longestword=graph.ay.rulers.valuedruler.get(i).getString().length();
			}
		}
		if(longestword > 2){
			int extra = longestword-2;
			graphmoved = true;
			graphmove = (double)extra*wordsize/w;
			//System.out.println("graphmove is "+graphmove);
		}
		else{
			graphmove = 0;
			graphmoved = false;
		}
	    
	    if(graph.polys != null){
	    	for(int i=0;i<graph.polys.size();i++)
	    		drawPolygon(g2,graph.polys.get(i));
	    }
	    
	    if(graph.curves != null){
	    	g2.setStroke(curvestroke);
		    for(int i=0;i<graph.curves.size();i++){
		    	drawCurve(g2,graph.curves.get(i));
		    }
	    }
	    
	    if(graph.lines != null){
	    	g2.setStroke(curvestroke);
	    	for(int i=0;i<graph.lines.size();i++)
	    		drawLine(g2,graph.lines.get(i));
	    }
	    
	    if(graph.colorlines != null){
	    	g2.setStroke(curvestroke);
	    	for(int i=0;i<graph.colorlines.size();i++)
	    		drawColorfulLine(g2,graph.colorlines.get(i));
	    }
	    
	    if(graph.plists != null){
	    	for(int i=0;i<graph.plists.size();i++){
	    		drawPointList(g2,graph.plists.get(i));
		    }
	    }
	    
	    g2.setStroke(axisstroke);
	    if(graph.ax != null)
	    	drawAxeX(g2,graph.ax);
	    if(graph.ay != null)
	    	drawAxeY(g2,graph.ay);
	    if(graph.name != null)
	    	drawGraphName(g2,graph.name);
	}
	
	/*public boolean saveGraph(String name){
		if (graph==null) return false;
		graph.name=name;
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
		return this;
	}
	
	public void exportImage(String path){
		BufferedImage bi=(BufferedImage)this.createImage(this.getWidth(),this.getHeight());
		this.paint(bi.getGraphics());
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
			javax.imageio.ImageIO.write(scrImg,"jpg",directory);
			directory.close();
		} 
		catch (Exception e1) { 
			e1.printStackTrace(); 
		}*/
	}
	
	private void setup(Graph2D graph,Rectangle r){
		this.graph=graph;
		setBounds(r);
	}
	
	private void drawPointList(Graphics2D g2,PointListData plist){
		if(plist.color!=null) g2.setColor(plist.color);
		else g2.setColor(Color.BLUE);
		System.out.println("point num is "+plist.vert.length);
		for(int i=0;i<plist.vert.length;i++){
			System.out.println("vert is "+plist.vert[i].x);
			Ellipse2D.Double point=new Ellipse2D.Double(convertX(plist.vert[i].x)-psize/2,convertY(plist.vert[i].y)-psize/2,psize,psize);
			g2.fill(point);
		}
	}
	
	private void drawCurve(Graphics2D g2,CurveData line){
		if(line.color!=null) g2.setColor(line.color);
		else g2.setColor(Color.BLUE);
		
		Path2D.Double path=new Path2D.Double();
		path.moveTo(convertX(line.vert.get(0).x),convertY(line.vert.get(0).y));
		for(int i=0;i<line.ctrl.size();i++){
			path.quadTo(convertX(line.ctrl.get(i).x),convertY(line.ctrl.get(i).y),convertX(line.vert.get(i+1).x),convertY(line.vert.get(i+1).y));
		}
		g2.draw(path);
	}

	private void drawPolygon(Graphics2D g2,mU.graph.data.graph2d.PolygonData poly){
		g2.setColor(poly.color);
		int[] x=new int[poly.vert.length];
		int[] y=new int[poly.vert.length];
		for(int i=0;i<poly.vert.length;i++){
			x[i]=convertX(poly.vert[i].x);
			y[i]=convertY(poly.vert[i].y);
		}
		java.awt.Polygon p=new java.awt.Polygon(x,y,x.length);
		g2.fill(p);
		g2.draw(p); //fill the edge
	}
	
	private void drawLine(Graphics2D g2,LineData line){
		if(line.color!=null) g2.setColor(line.color);
		else g2.setColor(Color.BLUE);
		
		for(int i=0; i<line.vert.length-1; i++){
			g2.drawLine(convertX(line.vert[i].x),convertY(line.vert[i].y),convertX(line.vert[i+1].x),convertY(line.vert[i+1].y));
		}
	}
	
	private void drawColorfulLine(Graphics2D g2,ColorfulLineData line){
		for(int i=0; i<line.vert.length-1; i++){
			g2.setColor(line.color[i]);
			g2.drawLine(convertX(line.vert[i].x),convertY(line.vert[i].y),convertX(line.vert[i+1].x),convertY(line.vert[i+1].y));
		}
	}
	
	private void drawAxeX(Graphics2D g2,Axis ax){
		g2.setColor(Color.BLACK);
		g2.setFont(rulfont); 
		g2.drawLine(convertX(ax.start),convertY(ax.height),convertX(ax.end),convertY(ax.height));
		for(int i=0;i<ax.rulers.valuedruler.size();i++){
			if(ax.rulers.valuedruler.get(i).isVisible()){
				g2.drawLine(convertX(ax.rulers.valuedruler.get(i).getPos()),convertY(ax.height),convertX(ax.rulers.valuedruler.get(i).getPos()),convertY(ax.height)-(int)(h*Axis.rullen));
				g2.drawString(ax.rulers.valuedruler.get(i).getString(),convertX(ax.rulers.valuedruler.get(i).getPos())+(int)(ax.rulers.valuedruler.get(i).getString().length()*rxworddx),convertY(ax.height)+rxworddy);
			}
		}
		for(int i=0;i<ax.rulers.subruler.size();i++){
			g2.drawLine(convertX(ax.rulers.subruler.get(i)),convertY(ax.height),convertX(ax.rulers.subruler.get(i)),convertY(ax.height)-(int)(h*Axis.subrullen));
		}
		if(ax.name != null){
			g2.drawString(ax.name,convertX(ax.end)+xlabeldx,convertY(ax.height)+xlabeldy);
		}
	}
	
	private void drawAxeY(Graphics2D g2,Axis ay){
		g2.setColor(Color.BLACK);
		g2.setFont(rulfont); 
		g2.drawLine(convertX(ay.height),convertY(ay.start),convertX(ay.height),convertY(ay.end));
		for(int i=0;i<ay.rulers.valuedruler.size();i++){
			if(ay.rulers.valuedruler.get(i).isVisible()){
				g2.drawLine(convertX(ay.height),convertY(ay.rulers.valuedruler.get(i).getPos()),convertX(ay.height)+(int)(w*Axis.rullen),convertY(ay.rulers.valuedruler.get(i).getPos()));
				g2.drawString(ay.rulers.valuedruler.get(i).getString(),convertX(ay.height)+ay.rulers.valuedruler.get(i).getString().length()*ryworddx,convertY(ay.rulers.valuedruler.get(i).getPos())+ryworddy);
			}
		}
		for(int i=0;i<ay.rulers.subruler.size();i++){
			g2.drawLine(convertX(ay.height),convertY(ay.rulers.subruler.get(i)),convertX(ay.height)+(int)(w*Axis.subrullen),convertY(ay.rulers.subruler.get(i)));
		}
		if(ay.name != null){
			g2.drawString(ay.name,convertX(ay.height)+ylabeldx,convertY(ay.end)+ylabeldy);
		}
	}
	
	private void drawGraphName(Graphics2D g2,String name){
		g2.setColor(Color.BLACK);
		g2.setFont(namefont); 
		
		g2.drawString(name,(int)(w*graphnamex),(int)(h*graphnamey));
	}
	
	private int convertX(double x){
		if(!graphmoved){
			return (int)(w*x);
		}
		else{
			return (int)(graphmove*w+w*x*(1-graphmove));
		}
	}
	
	private int convertY(double y){
		return (int)(h-y*h);
	}
}
