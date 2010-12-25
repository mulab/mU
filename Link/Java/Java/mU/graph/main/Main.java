package mU.graph.main;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;

import shell.model.Document;
import shell.model.Graphics2D;
import utils.FileOperations;

import mU.Kernel;
import mU.Parser;
import mU.Var;
import mU.graph.GraphDrawer;

// import mU.parser.mexpr.MExpr;
// import mU.parser.mexprparser.MExprParser;


public class Main {

	public static void main(String[] args){ 
		try{
			if(args.length != 2){
				args = new String[2];
				args[0] = "D:\\Projects\\GeometryBoard\\In.txt";
				// "D:\\Projects\\plot\\InputForm.txt";
				args[1] = "D:\\Projects\\GeometryBoard\\Out.png"; 
				// "D:\\Projects\\plot\\Graph.png";
			}
			BufferedReader fin = new BufferedReader(new FileReader(args[0])); 
			// MExprParser mep = new MExprParser();
			// MExpr expr = mep.generate(fin);
			Kernel kernel = new Kernel();
			Parser parser = new Parser(kernel);
			String line, buf = "";
			while ((line = fin.readLine()) != null)
				buf += line;
			Var expr = parser.parse(buf).toVar();
			// System.err.println(expr);
			new GraphDrawer(expr,args[1],true);
			System.exit(0);
			/*Graphics2D graphics2D = new Graphics2D(shell, expr);
			Document document = graphics2D.getDocument();
			BufferedWriter fout = new BufferedWriter(new FileWriter(args[1])); 
			document.writeToFile(fout);
			fout.close();
			*/
		}
		catch(Exception e){
			e.printStackTrace();
		}
   } 
}
