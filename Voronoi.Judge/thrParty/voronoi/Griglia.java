package voronoi;

import javax.swing.JOptionPane;
import java.io.IOException;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.Line2D;
import java.awt.geom.Point2D;
import java.awt.BasicStroke;
import java.awt.Color;


/**
Questo classe implementa una griglia rettangolare
con dimensioni scelte dall'utente.
*/
public class Griglia {
  public Griglia(int numeroPixel, int dimX,  int dimY, double unoZoom)
  {
    zoom = unoZoom;
    this.dimensioneX = (int)(Math.round(dimX * zoom));
    this.dimensioneY = (int)(Math.round(dimY * zoom));

    k = (int)(Math.round(numeroPixel * zoom));
    l = k;
    n =  Math.round(dimensioneX / (float)k); //numero colonne
    m = Math.round(dimensioneY / (float)l); //numero righe
  }

  public Griglia(int xPixel, int  yPixel, int dimX,  int dimY,  double unoZoom, String pixel)
  {
    zoom = unoZoom;
    this.dimensioneX = (int)(Math.round(dimX * zoom));
    this.dimensioneY = (int)(Math.round(dimY * zoom));
    k = (int)(Math.round(xPixel * zoom));
    l = (int)(Math.round(yPixel * zoom));
    n = Math.round(dimensioneX / (float)k); //numero colonne
    m = Math.round(dimensioneY / (float)l); //numero righe
  }
  /*
     public Griglia(int numeroRighe, int numeroColonne, int dimX,  int dimY,  double unoZoom)
     {
     zoom = unoZoom;
     this.dimensioneX = (int)(Math.round(dimX * zoom));
     this.dimensioneY = (int)(Math.round(dimY * zoom));

     k = (int)(Math.round((dimensioneX) / (float)numeroColonne)); 
     l = (int)(Math.round((dimensioneY) / (float)numeroRighe)); 
     n = numeroColonne; //numero colonne
     m = numeroRighe; //numero righe
     }
     */   
  public void disegna(Graphics2D g2)
  {
    g2.setStroke(new BasicStroke(1.0F));
    Color colorGriglia = new Color(99, 99, 99);
    g2.setPaint(colorGriglia);
    // disegna la griglia
    for (int i = 0; i < m; i++)
    {

      Point2D.Double from = new Point2D.Double(0, i * l);
      Point2D.Double to = new Point2D.Double(dimensioneX, i * l);

      Line2D.Double line = new Line2D.Double(from, to);
      //Disegna le linee
      g2.draw(line);
    }
    Point2D.Double dal = new Point2D.Double(0, dimensioneY);
    Point2D.Double al = new Point2D.Double(dimensioneX, dimensioneY);
    Line2D.Double linea = new Line2D.Double(dal, al);
    g2.draw(linea);
    for (int j = 0; j <  n; j++)
    {

      //Coordinata x dell'estremo griglia
      Point2D.Double from = new Point2D.Double(j * k, 0);
      Point2D.Double to = new Point2D.Double(j * k, dimensioneY);
      Line2D.Double line = new Line2D.Double(from, to);
      //Disegna le linee
      g2.draw(line);
    }
    dal = new Point2D.Double(dimensioneX, 0);
    al = new Point2D.Double(dimensioneX, dimensioneY);
    linea = new Line2D.Double(dal, al);
    g2.draw(linea);


  }

  public int roundToGriglia(double coord, int KL)
  {
    int coordRounded = KL * Math.round((float)(coord / KL));
    return coordRounded;
  }

  public int getKappa()
  {
    return k;
  }
  public int getElle()
  {
    return l;
  }

  public int getEnne()
  {
    return n;
  }
  public int getEmme()
  {
    return m;
  }
  public int getDimensioneX()
  {
    return dimensioneX;
  }
  public int getDimensioneY()
  {
    return dimensioneY;
  }


  private   int k = 12;
  private   int l = 12;
  private  int n = 30;
  private  int m = 30;
  private double zoom = 1;

  private int dimensioneX;
  private int dimensioneY;


}
