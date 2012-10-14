import java.util.*;
import java.awt.*;
import java.awt.geom.Point2D;
import java.io.Serializable;
import java.io.ObjectOutputStream;
import java.io.ObjectInputStream;
import java.io.IOException;



/**
 *@author Indrit Selimi
*/

public class Cella implements Cloneable 
{
    /**
     * Un altro costruttore.
     * @param x la coordinata x del sito.
     * @param y la coordinata y del sito.
     */
    public  Cella(double x, double y)
    {
        frontiere = new ArrayList();
        sito = new Point2D.Double(x, y);
    }
    
     public  Cella(double x, double y, boolean col)
    {
        frontiere = new ArrayList();
        sito = new Point2D.Double(x, y);
        this.colore = col;
    }
    public Cella(Point2D.Double unPunto)
    {
        frontiere = new ArrayList();
        sito = unPunto;
    }
    
     public Cella(Point2D.Double unPunto, boolean col)
    {
        frontiere = new ArrayList();
        sito = unPunto;
        this.colore = col;
    }
        
    public ArrayList getVfrontiere()
    {
        return  frontiere;
    }
 
    public Iterator getFrontiere()
    {
        return frontiere.iterator();
    }
   
    public Point2D.Double getKernel()
    {
        return sito;
    }
    
     public boolean puntiUguali(Point2D.Double pointUno, Point2D.Double pointDue)
     {
          double numUnoX = pointUno.getX();
          double numUnoY = pointUno.getY();
          double numDueX = pointDue.getX();
          double numDueY = pointDue.getY();
          boolean ics, yps;
          
          if((numUnoX != 0) && (numDueX != 0))
          {
             ics = ((Math.abs(numUnoX - numDueX) / 
                        Math.max(Math.abs(numUnoX), Math.abs(numDueX))) <= EPS);
          }
          else
          {
              ics = (Math.abs(numUnoX - numDueX) <= EPS);
          }
          
          if((numUnoY != 0) && (numDueY != 0))
          {
             yps = ((Math.abs(numUnoY - numDueY) / 
                        Math.max(Math.abs(numUnoY), Math.abs(numDueY))) <= EPS);
          }
          else
          {
              yps = (Math.abs(numUnoY - numDueY) <= EPS);
          }
          
          return (ics && yps);
          
          
     }
     
     public void setColore(boolean rossoblu)
     {
         colore = rossoblu;
     }
     
     public boolean getColore()
     {
         return colore;
     }
     /*
    private void writeObject(ObjectOutputStream out) throws IOException //Questo va gestito!!
    {
        out.defaultWriteObject();
        
        out.writeDouble(sito.getX());
        out.writeDouble(sito.getY());
    }
    
    private void readObject(ObjectInputStream in) throws IOException,ClassNotFoundException //Questo va gestito!!
    {
       in.defaultReadObject();
        
       double x = in.readDouble();
       double y = in.readDouble();
       sito = new Point2D.Double(x, y);
       
    }
    */
       
        
     
   /*
    public boolean equals(Object altraCella)
    {
        if(altraCella == null) return false;
        if (getClass() != altraCella.getClass()) return false;
        Cella altra = (Cella)altraCella;
        
        return(puntiUguali(sito, altra.sito));
    }
    */
     
     public Object clone()
     {
         Cella clonata  = null;
         try
         {
             clonata = (Cella)super.clone();
             clonata.setColore(this.colore);
         }
         catch(CloneNotSupportedException e){e.printStackTrace();}
         
         
         return clonata;
     }
    
    
    private ArrayList frontiere;
    private Point2D.Double sito;
    private static final double EPS = 1E-13;
    private boolean colore = false;
}
