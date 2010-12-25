package mU.graph;

import javax.swing.JPanel;

import mU.Var;

public class GraphicsPanel {

	private GraphDrawer drawer;

	public GraphicsPanel(Var expr) {
		try {
			drawer = new GraphDrawer(expr, null, false);
		} catch (Exception e) {
			System.out.println(e);
		}
	}

//	public GraphicsPanel(String filename) {
//		try {
//			File file = new File(filename);
//			BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(file)));
//			MExprParser mep = new MExprParser();
//			MExpr expr = mep.generate(reader);
//			drawer = new GraphDrawer(expr, null, false);
//		} catch (Exception e) {
//			System.out.println(e);
//		}
//	}
//
	public JPanel exportPanel() {
		return drawer.exportPanel();
	}

}
