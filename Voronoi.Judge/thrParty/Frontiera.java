import java.awt.*;
import java.util.*;
import java.util.Enumeration;
import java.awt.geom.Point2D;
import java.awt.geom.Line2D;

/**
 *@author Indrit Selimi.
 */

public class Frontiera                                                                                                                                                                                                                                                                                                                           
{
    /**
    * Crea una frontiera che formata dall'intersezione di due celle.
    * I due punti sono delimitatori della frontiera.
    */
    public Frontiera(Cella cellaUno, Cella cellaDue, Point2D.Double limit1, Point2D.Double limit2)
    {
        this.cella1 = cellaUno;                                                                           
        this.cella2 = cellaDue;
        p1 = limit1;
        p2 = limit2;
    } 
    
    public Point2D.Double trovaIntersezione(Cella cella)
    {
        double a1, b1, c1;
        
        double a2, b2, c2;
        
        double a3, b3, c3;
    
        double x1, y1, x2, y2;
        
        double dx, dy;  
        
        x1 = p1.getX();
        x2 = p2.getX();
        y1 = p1.getY();
        y2 = p2.getY();
        dx = x2-x1;
        dy = y2-y1;

        if(Math.abs(dx) > Math.abs(dy))
        {
            a1 = -dy/dx;
            b1 = 1;
            c1 = -a1*x1 - y1; 
        }
        else
        {
            a1 = 1;
            b1 = -dx/dy;
            c1 = -b1*y1 - x1;
        }
      
        x1 = cella1.getKernel().getX();  
        x2 = cella.getKernel().getX();
        y1 = cella1.getKernel().getY();  
        y2 = cella.getKernel().getY();
        dx = x2-x1;  
        dy = y2-y1;

        if(Math.abs(dx) > Math.abs(dy))
        {
            a2 = -dy/dx;
            b2 = 1;
            c2 = -a2*x1 - y1; 
        }
        else
        {
            a2 = 1;
            b2 = -dx/dy;
            c2 = -b2*y1 - x1; 
        }
        
        
        

       
        double xm = (cella1.getKernel().getX() + cella.getKernel().getX()) / 2;
        double ym = (cella1.getKernel().getY() + cella.getKernel().getY()) / 2;

        a3 = b2;
        b3 = -a2;       
        c3 = -xm*a3 - ym*b3;
      
        
        
        if(sonoUguali(a1*b3, a3*b1))
        {
            return null;
        }
        
        double denom = a1*b3 - b1*a3;
        
        double y0 = (c1*a3 - c3*a1) / denom;
        
        double x0 = (c3*b1 - c1*b3) / denom;
        
        double xmax = Math.max(p1.getX(), p2.getX());
        double xmin = Math.min(p1.getX(), p2.getX());
        double ymax = Math.max(p1.getY(), p2.getY());
        double ymin = Math.min(p1.getY(), p2.getY());
       
        if((isMaggiorUguale(x0, xmin)) && (isMinorUguale(x0, xmax)) && 
                   (isMaggiorUguale(y0, ymin)) && (isMinorUguale(y0, ymax)))
        {
            return new Point2D.Double(x0, y0);
        }
        else
            return null;
            
    } 
 
    public Cella getVicinoDi(Cella cella)
    {
        if(cella == cella1)
            return cella2;
        if(cella == cella2)
            return cella1;
        return null;
    }
   
  

    public boolean isEdge()
    {
        return (cella2 == null);
    }
    
    public Point2D.Double getPuntoUno()
    {
        return p1;
    }
    
    public Point2D.Double getPuntoDue()
    {
        return p2;
    }
    
    public void setPuntoUno(Point2D.Double punto)
    {
        this.p1 = punto;
    }
    
    public void setPuntoDue(Point2D.Double punto)
    {
        this.p2 = punto;
    }
    
    public void setCellaUno(Cella sito)
    {
        cella1 = sito;
    }
    
    public void setCellaDue(Cella sito)
    {
        cella2 = sito;
    }
    
    public boolean isMinorUguale(double u, double d)
    {
        if((u != 0) && (d != 0))
        {
            return ((Math.abs(u-d) / Math.max(Math.abs(u), Math.abs(d)) <= EPSILON) || (u < d));           
        }
        else
        {
           return ((Math.abs(u-d) <= EPSILON) || (u < d));
        }
    }
            
    
    public boolean isMaggiorUguale(double u, double d)
    {
        if((u != 0) && (d != 0))
        {
            return (((Math.abs(u-d) / Math.max(Math.abs(u), Math.abs(d))) <= EPSILON) || (u > d));
        }   
        else
        {
           return ((Math.abs(u-d) <= EPSILON) || (u > d));
        }
    }
    
     public boolean sonoUguali(double num, double numero)
     {
          if((num != 0) && (numero != 0))
          {
              return ((Math.abs(num - numero) / 
                        Math.max(Math.abs(num), Math.abs(numero))) <= EPSILON);
          }
          else
          {
              return (Math.abs(num - numero) <= EPSILON);
          }
      }
        
    private Point2D.Double p1, p2;
    private Cella cella1, cella2;
    private static final double EPSILON = 1E-9;
}
