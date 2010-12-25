package mU.graph;

import java.awt.Color;
import java.awt.Rectangle;


import javax.swing.*;

import mU.Symbol;
import mU.Var;
// import mU.parser.mexpr.MExpr;
// import mU.parser.mexpr.MSymbol;
import mU.graph.data.Graph;
import mU.graph.data.graph2d.Graph2D;
import mU.graph.data.graph3d.Graph3D;
import mU.graph.drawer.BasicDrawer;
import mU.graph.drawer.Drawer2D;
import mU.graph.drawer.Drawer3D;
import mU.graph.graphics.Graphics2D;
import mU.graph.graphics.Graphics3D;

public class GraphDrawer{
	
	private static final Symbol Graphics2D = Symbol.system("Graphics2D");
	private static final Symbol Graphics3D = Symbol.system("Graphics3D");
	
	private JPanel panel=null;
	private BasicDrawer drawer;
	private Graph graph;
	private Rectangle r1=new Rectangle(100,100,400,300);
	private Rectangle r2=new Rectangle(100,100,500,500);
	
	public GraphDrawer(Var expr,String path,boolean paintFrame){
		
		if(expr.tupleQ(Graphics2D)){
			Graphics2D graphics = new Graphics2D(expr);
			graph = graphics.getGraph();
			drawer = new Drawer2D((Graph2D)graph,r1);
			panel = drawer.exportPanel();
			if (paintFrame) {
				JFrame frame = new JFrame();
			    frame.setBackground(Color.WHITE);
				frame.setBounds(r1);
				frame.getContentPane().add(panel);
				frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
				frame.setVisible(true);
				if (path != null)
					drawer.exportImage(path);
//				System.exit(0);
			}
		}
		else if(expr.tupleQ(Graphics3D)){
			Graphics3D graphics = new Graphics3D(expr);
			graph = graphics.getGraph();
			drawer = new Drawer3D((Graph3D)graph,r2);
			panel = drawer.exportPanel();
			if (paintFrame) {
				JFrame frame = new JFrame();
			    frame.setBackground(Color.WHITE);
				frame.setBounds(r2);
				frame.getContentPane().add(drawer.exportPanel());
				frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
				frame.setVisible(true);
				System.out.println("frame appear");
				if (path != null)
					drawer.exportImage(path);
				System.out.println("screen shot got");
//				System.exit(0);
			}
		}
		else{
			return;
		}
	}
	
	

	public void setBounds(Rectangle r){
		this.r1=r;
	}
	
	public JPanel exportPanel(){
		return panel;
	}
}
