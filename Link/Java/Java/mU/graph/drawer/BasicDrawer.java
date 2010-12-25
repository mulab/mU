package mU.graph.drawer;

import java.awt.Graphics;

import javax.swing.JPanel;

public interface BasicDrawer{

	// public abstract boolean saveGraph(String name);
	public abstract JPanel exportPanel();
	public abstract void exportImage(String path);
	// public abstract void paint(Graphics g);
}
