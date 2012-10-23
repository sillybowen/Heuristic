package voronoi;

import java.awt.*;
import java.util.*;
import java.awt.geom.Point2D;
import java.awt.geom.Rectangle2D;
import java.awt.geom.RoundRectangle2D;
import javax.swing.JPanel;
import java.awt.geom.Line2D;
import java.awt.geom.Ellipse2D;
import java.awt.geom.GeneralPath;
import java.text.NumberFormat;
import javax.swing.JLabel;
import java.text.*;
import java.awt.image.BufferedImage;
import java.awt.print.*;
import javax.print.*;

import java.util.concurrent.atomic.AtomicInteger;

public class Pannello extends JPanel implements Printable {
  // Reference to Owning VoronoiGame() class
  private final VoronoiGame parVoroniGame_;
  /**
   * Costruisce una panello disegnando un diagramma di voronoi.
   * @param diagramma  il Diagramma di Voronoi da disegnare
   */
  public Pannello(Diagramma  unDiagramma, VoronoiGame parVoroniGame) {
    parVoroniGame_ = parVoroniGame;
    voronoi = unDiagramma;
  }


  /** Questo metodo serve per lo scrollPane.
    Se non ci fosse lo scrollPane non funzionerebbe!
    @return la dimensione del diagramma.
    */
  public Dimension getPreferredSize()
  {
    if(voronoi == null) return new Dimension(600, 600);
    Dimension dim = voronoi.getSize();
    return new Dimension((int)(dim.getWidth()* zoom), (int)(dim.getHeight()* zoom));
  }

  public int getDimensioneX()
  {
    Dimension dimX = voronoi.getSize();
    return (int)(dimX.getWidth());
  }

  public int getDimensioneY()
  {
    Dimension dimY = voronoi.getSize();
    return (int)(dimY.getHeight());
  }


  public double getZoom()
  {
    return zoom;
  }

  public void setZoom(double factor)
  {
    zoom = factor;
  }

  public void setGriglia(Griglia unaGriglia)
  {
    griglia = unaGriglia;
  }


  public Griglia dammiGriglia()
  {
    return griglia;
  }

  public void setViewGriglia(boolean on)
  {
    viewGriglia = on;
  }

  public boolean getViewGriglia()
  {
    return  viewGriglia;
  }


  public double calcolaArea(ArrayList lista)
  {
    int i,j;
    double area = 0;
    int N = lista.size();
    if(lista.isEmpty())
    {
      return 0;
    }
    for (i=0; i < N; i++)
    {
      j = (i + 1) % N;
      area += ( (((Point2D.Double)(lista.get(i))).getX() * ((Point2D.Double)(lista.get(j))).getY()) 
          - (((Point2D.Double)(lista.get(i))).getY() * ((Point2D.Double)(lista.get(j))).getX()) );
    }

    area = Math.abs(area / 2.0);
    return(area);
  }

  public double AreaRed() {
    ArrayList confini;        
    Cella sitoCorrente;
    double areaTotaleRosso = 0;
    int indice = 0;
    if(voronoi.getVCelle().isEmpty()) {
      return 0;
    }
    Iterator listaSiti = voronoi.getVCelle().iterator();
    while (listaSiti.hasNext()) {
      ArrayList lista;
      sitoCorrente = (Cella)listaSiti.next();             
      confini = sitoCorrente.getVfrontiere();
      lista = ordinaPunti(confini, sitoCorrente.getKernel());

      if(sitoCorrente.getColore())
        areaTotaleRosso += calcolaArea(lista); 
    }

    return areaTotaleRosso;
  }

  public String areaTotaleRosso() {
    ArrayList confini;        
    Cella sitoCorrente;
    double areaTotaleRosso = 0;
    int indice = 0;
    if(voronoi.getVCelle().isEmpty())
    {
      return "0";
    }
    Iterator listaSiti = voronoi.getVCelle().iterator();
    while(listaSiti.hasNext())
    {
      ArrayList lista;
      sitoCorrente = (Cella)listaSiti.next();             
      confini = sitoCorrente.getVfrontiere();
      lista = ordinaPunti(confini, sitoCorrente.getKernel());

      if(sitoCorrente.getColore())
        areaTotaleRosso += calcolaArea(lista); 
    }

    DecimalFormat myFormatter = new DecimalFormat("#");
    return myFormatter.format(areaTotaleRosso);
  }

  public double AreaBlue() {
    ArrayList confini;        
    Cella sitoCorrente;
    double areaTotaleBlue = 0;
    int indice = 0;
    if(voronoi.getVCelle().isEmpty())
    {
      return 0;
    }
    Iterator listaSiti = voronoi.getVCelle().iterator();
    while(listaSiti.hasNext())
    {
      ArrayList lista;
      sitoCorrente = (Cella)listaSiti.next();             
      confini = sitoCorrente.getVfrontiere();
      lista = ordinaPunti(confini, sitoCorrente.getKernel());

      if(!sitoCorrente.getColore())
        areaTotaleBlue += calcolaArea(lista);
    }
    return areaTotaleBlue;
  }

  public String areaTotaleBlue()
  {
    ArrayList confini;        
    Cella sitoCorrente;
    double areaTotaleBlue = 0;
    int indice = 0;
    if(voronoi.getVCelle().isEmpty())
    {
      return "0";
    }
    Iterator listaSiti = voronoi.getVCelle().iterator();
    while(listaSiti.hasNext())
    {
      ArrayList lista;
      sitoCorrente = (Cella)listaSiti.next();             
      confini = sitoCorrente.getVfrontiere();
      lista = ordinaPunti(confini, sitoCorrente.getKernel());

      if(!sitoCorrente.getColore())
        areaTotaleBlue += calcolaArea(lista);
    }
    DecimalFormat myFormatter = new DecimalFormat("#");
    return myFormatter.format(areaTotaleBlue);
  }

  // Metodi per il disegno...
  public void paintComponent(Graphics g)
  {
    super.paintComponent(g);
    g2 = (Graphics2D)g;
    setBackground(new Color(194, 201, 191));
    if(voronoi == null) return;

    if(isAliasing)
    {
      g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
      g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
    }

    // System.out.println("Calling paintComponent: " + componentsPaint + " th time!");
    if (componentsPaint.incrementAndGet() < 3) {  // Init game painting
      try {
        parVoroniGame_.getIsPaintingSemaphore().acquire();
      } catch (InterruptedException ie) {
        ie.printStackTrace();
      }
      disegnaCelle(g2);
      parVoroniGame_.getIsPaintingSemaphore().release();
    } else {  // Puting stone painting
      disegnaCelle(g2);
      parVoroniGame_.getIsPaintingSemaphore().release();
    }


    if(viewGriglia)  griglia.disegna(g2);
    if(isCancellabile) disegnaAreaCancellabile(g2);        
    if(isDragged) disegnaQuadrettiDragged(g2);
    if(showArea) disegnaAreaCelle(g2);
    if(showPosition) disegnaCoordinate(g2);
    disegnaArena(g2);
  }

  public void disegnaCelle(Graphics2D gg) {
    Graphics2D g3 = gg;
    ArrayList frontiere;
    Frontiera frontiera;
    //Cella cellaFine;
    //int indice = 0;
    int x, y = 0;

    Iterator listaSiti = voronoi.getCelle();
    while(listaSiti.hasNext()) {
      ArrayList lista;
      cellaFine = (Cella)listaSiti.next();
      if(cellaFine.getColore())
        g3.setPaint(new Color(250, 72, 72));
      else
        g3.setPaint(new Color(31, 75, 153));


      GeneralPath s = new GeneralPath();
      frontiere = cellaFine.getVfrontiere();
      lista = ordinaPunti(frontiere, cellaFine.getKernel());

      if(!(lista.isEmpty()))
      {
        s.moveTo((float)(((Point2D.Double)(lista.get(0))).getX() * zoom), 
            (float)(((Point2D.Double)(lista.get(0))).getY() * zoom));
        for(int i = 1; i < lista.size(); i++)
        {
          s.lineTo((float)(((Point2D.Double)(lista.get(i))).getX()*zoom), 
              (float)(((Point2D.Double)(lista.get(i))).getY() * zoom) );

        }
        s.closePath();

        g3.fill(s);
        g3.setPaint(Color.black);
        g3.draw(s);
      }

      x = (int) (cellaFine.getKernel().getX()*zoom);
      y = (int) (cellaFine.getKernel().getY()*zoom);
      Ellipse2D.Double circle = new Ellipse2D.Double((x-diametro/2), (y-diametro/2), diametro, diametro);
      g3.setPaint(Color.yellow);
      g3.setStroke(new BasicStroke(1.0F));
      g3.fill(circle);
    }
  }

  public void  disegnaArena(Graphics2D gg)
  {
    Graphics2D g3 = gg;
    Point2D.Double p1 = new Point2D.Double(0, 0);
    Point2D.Double p2 = new Point2D.Double(getDimensioneX()*zoom, 0);
    Point2D.Double p3 = new Point2D.Double(getDimensioneX()*zoom, getDimensioneY()*zoom);
    Point2D.Double p4 = new Point2D.Double(0, getDimensioneY()*zoom);

    Line2D.Double lineaNord = new Line2D.Double(p1, p2);
    Line2D.Double lineaEst = new Line2D.Double(p2, p3);
    Line2D.Double lineaSud = new Line2D.Double(p3, p4);
    Line2D.Double lineaOvest = new Line2D.Double(p4, p1);

    g3.setPaint(new Color(45, 65, 36));
    g3.setStroke(new BasicStroke(2.0F));
    g3.draw(lineaNord);
    g3.draw(lineaEst);
    g3.draw(lineaSud);
    g3.draw(lineaOvest);
  }



  /*
   * 
   public void disegnaScheletro(Graphics2D gg)
   {
   Graphics2D g3 = gg;
   ArrayList frontiere;
   Frontiera frontiera;
   Cella cellaFine;
   int indice=0;

   Iterator listProvince = voronoi.getVCelle().iterator();
   while(listProvince.hasNext())
   {

   cellaFine = (Cella)listProvince.next();
   frontiere = cellaFine.getVfrontiere();         

   GeneralPath s = new GeneralPath();

   Iterator listFrontiere = frontiere.iterator();
   if(listFrontiere.hasNext())
   {
   frontiera = (Frontiera)listFrontiere.next();
   puntoUno = frontiera.getPuntoUno();
   puntoDue = frontiera.getPuntoDue();

   s.moveTo((float)(puntoUno.getX() * zoom), (float)(puntoUno.getY() * zoom));
   s.lineTo((float)(puntoDue.getX() * zoom), (float)(puntoDue.getY() * zoom));

   }
   while(listFrontiere.hasNext())
   {
   frontiera = (Frontiera)listFrontiere.next();
   puntoUno = frontiera.getPuntoUno();
   puntoDue = frontiera.getPuntoDue();

   s.moveTo((float)(puntoUno.getX() * zoom), (float)(puntoUno.getY() * zoom));
   s.lineTo((float)(puntoDue.getX() * zoom), (float)(puntoDue.getY() * zoom));


   }
   g3.setPaint(Color.black);
   g3.setStroke(new BasicStroke(1.0F));
   g3.draw(s);

   }
   }
   */

  public void  disegnaAreaCelle(Graphics2D gg)
  { 
    Graphics2D g3 = gg;
    double factor = getZoom();
    ArrayList confini;        
    Cella sitoCorrente;

    Double areaCella;
    ArrayList listaArea = new ArrayList();
    ArrayList listaKernel = new ArrayList();

    Iterator listaSiti = voronoi.getVCelle().iterator();
    while(listaSiti.hasNext())
    {
      ArrayList lista;
      sitoCorrente = (Cella)listaSiti.next();  
      listaKernel.add(sitoCorrente.getKernel());

      confini = sitoCorrente.getVfrontiere();
      lista = ordinaPunti(confini, sitoCorrente.getKernel());

      DecimalFormat myFormatter = new DecimalFormat("#");
      listaArea.add((String)(myFormatter.format(calcolaArea(lista))));
    }


    g3.setPaint(Color.white);
    for(int i = 0; i <  listaKernel.size(); i++)
    {
      int a = (int) (((Point2D.Double)(listaKernel.get(i))).getX() * factor);
      int b = (int) (((Point2D.Double)(listaKernel.get(i))).getY() * factor);

      g3.drawString("(" + listaArea.get(i) + ")", (a + 10)  ,(b + 10));

    }

  }

  public void  disegnaCoordinate(Graphics2D gg)
  { 
    Graphics2D g3 = gg;
    double factor = getZoom();
    Cella sitoCorrente;



    ArrayList listaKernel = new ArrayList();
    Iterator listaSiti = voronoi.getVCelle().iterator();
    while(listaSiti.hasNext())
    {
      ArrayList lista;
      sitoCorrente = (Cella)listaSiti.next();  

      double coX = sitoCorrente.getKernel().getX();
      double coY = sitoCorrente.getKernel().getY();

      int a = (int) (coX * factor);
      int b = (int) (coY * factor);

      NumberFormat formatter = NumberFormat.getNumberInstance();
      formatter.setMaximumFractionDigits(2);
      formatter.setMinimumFractionDigits(2);
      g3.setPaint(Color.white);
      g3.drawString(("("+"x =" + " " + formatter.format(coX)  + "; " +
            "y =" +" "+ formatter.format(coY) +")"), a - 50 ,(b - 10));

    }

  }

  public void disegnaAreaCancellabile(Graphics2D gg)
  {
    Graphics2D g3 = gg;
    rect = new RoundRectangle2D.Double(0,0,0,0, 30, 30);
    if((p != null) && (q != null))
      rect.setFrameFromDiagonal(p, q);
    else return;
    g3.setComposite(AlphaComposite.getInstance(AlphaComposite.SRC_OVER, (float)0.3));
    g3.setPaint(new Color(1, 6, 207));
    g3.setStroke(new BasicStroke(1.0F));
    g3.draw(rect);
    g3.setPaint(new Color(216, 232, 249));
    g3.fill(rect);
  }
  public void disegnaQuadrettiDragged(Graphics2D gg)
  {
    Graphics2D g3 = gg;
    g3.setPaint(Color.white);
    g3.draw(rettangolo);
  }   

  public ArrayList ordinaPunti(ArrayList v, Point2D.Double sito)
  {
    kernel = sito;
    Frontiera front;
    ArrayList  listaPunti = new ArrayList ();
    Iterator listaElementi = v.iterator();
    while(listaElementi.hasNext())
    {
      front= (Frontiera)listaElementi.next();
      puntoUno = front.getPuntoUno();
      puntoDue = front.getPuntoDue();

      if(!(listaPunti.contains(puntoUno)))
      {
        listaPunti.add(puntoUno);
      } 
      if(!(listaPunti.contains(puntoDue)))
      {
        listaPunti.add(puntoDue);
      }

      Comparator comp = new ConfrontaAngoli();
      Collections.sort(listaPunti, comp);
    }
    return listaPunti;
  }

  class ConfrontaAngoli implements Comparator
  {
    public int compare(Object a, Object b)
    {
      Point2D.Double puntoA =  (Point2D.Double)a;
      Point2D.Double puntoB =  (Point2D.Double)b;

      if(getAngle(puntoA, kernel) < getAngle(puntoB, kernel)){return -1;}
      if(sonoUguali(getAngle(puntoA, kernel), getAngle(puntoB, kernel))){return 0;}
      return 1;
    }
  }

  private double getAngle(Point2D.Double p1, Point2D.Double p2)
  {
    if(!(puntiUguali(p1, p2))) 
    {
      double b = (p2.getX() - p1.getX());
      double a = (p1.getY() - p2.getY());
      return (Math.atan2(a, b) + Math.PI);
    }
    else return 0;
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

  public boolean sonoUguali(double numUno, double numDue)
  {
    if((numUno != 0) && (numDue != 0))
    {
      return ((Math.abs(numUno - numDue) / 
            Math.max(Math.abs(numUno), Math.abs(numDue))) <= EPS);
    }
    else
    {
      return (Math.abs(numUno - numDue) <= EPS);
    }
  }
  public void setShowArea(boolean vero)
  {
    showArea = vero;

  }

  public boolean getShowArea()
  {
    return showArea;

  }
  public void setShowPosition(boolean vero)
  {
    showPosition = vero;
  }

  public boolean getShowPosition(boolean vero)
  {
    return showPosition;
  }
  public void setAreaCancellabile(boolean on)
  {
    isCancellabile = on;
  }
  public boolean getAreaCancellabile()
  {
    return isCancellabile;
  }
  public void setP(Point2D punto)
  {
    p = punto;
  }

  public void setQ(Point2D punto)
  {
    q = punto;
  }
  public Point2D getP()
  {
    return p;
  }
  public RoundRectangle2D.Double getRect()
  {
    return rect;
  }
  public void setRect(RoundRectangle2D.Double rec)
  {
    rect = rec;
  }


  public BufferedImage getJPEG(int unIntero)
  {
    BufferedImage image = new BufferedImage((int)(Math.round(getDimensioneX() * zoom)), (int)(Math.round(getDimensioneY() * zoom)), unIntero);
    Graphics2D gImage = image.createGraphics();
    disegnaCelle(gImage);
    if(viewGriglia)  griglia.disegna(gImage);
    if(showArea) disegnaAreaCelle(gImage);
    if(showPosition) disegnaCoordinate(gImage);

    return image;
  }
  public int print(Graphics g, PageFormat pf, int page) throws PrinterException
  {
    if (page >= 1) return Printable.NO_SUCH_PAGE;

    Graphics2D g3 = (Graphics2D)g;
    g3.translate(pf.getImageableX(), pf.getImageableY());
    disegnaCelle(g3);
    if(viewGriglia)  griglia.disegna(g3);
    if(showArea) disegnaAreaCelle(g3);
    if(showPosition) disegnaCoordinate(g3);

    return Printable.PAGE_EXISTS;
  }


  public void setDiametro(int unNumero)
  {
    diametro = unNumero;
  }

  public void setRettangolo(Rectangle2D.Double unRettDrag)
  {
    rettangolo = unRettDrag;
  }
  public Rectangle2D.Double getRettangolo()
  {
    return rettangolo;
  }
  public void setDragged(boolean on)
  {
    isDragged = on;
  }
  public boolean getDragged()
  {
    return isDragged;
  }

  public void setAliasing(boolean on)
  {
    isAliasing = on;
  }
  public boolean getAliasing()
  {
    return isAliasing;
  }


  // Alternating semaphore release
  private static AtomicInteger componentsPaint = new AtomicInteger();

  private Point2D.Double kernel; //Sperimentale!!!
  private Graphics2D g2;
  private Diagramma voronoi;  


  private Point2D puntoUno;
  private Point2D puntoDue;
  private double zoom = 1.0;

  private Griglia griglia; //Sperimentale!!
  private boolean viewGriglia = false;

  private ArrayList listaKernel;
  private ArrayList listaArea;


  private boolean showArea = false;
  private boolean showPosition = false;
  private static final double EPS = 1E-14;

  private Point2D p; //Molto Sperimentale!!
  private Point2D q; //Molto Sperimentale
  private boolean isCancellabile = false;//Molto Sperimentale
  private RoundRectangle2D.Double rect;
  private int diametro = 5;

  private Rectangle2D.Double rettangolo;
  private boolean isDragged = false;
  private boolean isAliasing = false;

  private  Cella cellaFine;
}
