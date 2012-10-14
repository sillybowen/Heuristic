package voronoi;

import java.util.*;
import java.awt.*;
import java.awt.geom.Point2D;
import java.awt.geom.Rectangle2D;
import java.io.PrintWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.BufferedReader;
import java.awt.geom.RoundRectangle2D;
import java.lang.Exception;

/**
 * Il codice implementa le strutture dati per il Diagramma di Voronoi.
 * @author Indrit Selimi.
 * Il diagramma costituito da un insieme di celle.
 */
public class Diagramma {

  /**
   * Costruisce un diagramma di Voronoi.
   * @param sizeX la larghezza del diagramma.
   * @param sizeY l'altezza del diagramma.
   */
  public Diagramma(int sizeX, int sizeY)
  {
    maxX = sizeX;
    maxY = sizeY;
    celle = new ArrayList();
  }

  public void setDiagramSize(int x, int y)
  {
    this.clear();
    maxX = x;
    maxY = y;
  }

  public void aggiungiCellaRossa(Cella cella)
  {
    cella.setColore(true); /// colore rosso
    aggiungiCella(cella);
  }

  public void aggiungiCellaBlu(Cella cella)
  {
    cella.setColore(false); /// colore rosso
    aggiungiCella(cella);
  }


  /**
   * Aggiunge una cella nella struttura dati.
   * @param cella la cella da inserire.
   */

  public void aggiungiCella(Cella cella)
  {
    // variabili locali inizio...
    Iterator listaCelle;
    Iterator listaFrontiere;
    yyy = 0;

    ArrayList celleFatte = new ArrayList(); 
    Iterator listaCelleFatte;

    Iterator listaFront;

    Cella cella1, cella2, cellaFatta;
    Frontiera front, front1, front2, front3, front4, newFront;
    Point2D.Double punto1, punto2, punto3, punto, limit;

    boolean loop = false; 
    boolean doneFlag = false;
    // fine variabili locali

    // inizializzazione variabili
    front1 = front2 = front3 = front4 = front = null;
    punto1 = punto2 = punto3 = null;
    cella1 = cella2 = null;

    // se la distanza minore di quella minima ritorna...
    if(trovaSito(cella.getKernel()) != null)
    {
      return;
    }

    // aggiunge un nuovo elemento(sito)

    // cella.setColore((celle.size() % 2 == 0));
    celle.add(cella);
    //celleFatte.add(cella);

    //aggiunge i bordi dell quadrato  aggiungendo quattro frontiere alla prima cella
    if(celle.size() == 1)
    {
      Point2D.Double p1 = new Point2D.Double(0, 0);
      Point2D.Double p2 = new Point2D.Double(maxX, 0);
      Point2D.Double p3 = new Point2D.Double(maxX, maxY);
      Point2D.Double p4 = new Point2D.Double(0, maxY);

      this.aggiungiFrontiera(cella,new Frontiera(cella, null, p1, p4));
      this.aggiungiFrontiera(cella,new Frontiera(cella, null, p4, p3));
      this.aggiungiFrontiera(cella,new Frontiera(cella, null, p3, p2));
      this.aggiungiFrontiera(cella,new Frontiera(cella, null, p2, p1));

      return; 
    }


    listaCelle = celle.iterator(); //reinizializzazione della listaCelle 
    while(listaCelle.hasNext())
    {
      // la cella sucessiva...cella1
      cella1 = (Cella)listaCelle.next();
      doneFlag = false;


      listaCelleFatte = celleFatte.iterator(); //inizzializzazione di listaCelleFatte
      while(listaCelleFatte.hasNext()) 
      {
        cellaFatta = (Cella)listaCelleFatte.next();
        if(cella1 == cellaFatta)
        {
          doneFlag = true;
          break;
        }

      }

      if(doneFlag) continue;

      //Per le altre celle  continua con le istruzioni:
      front1 = front2 = null;
      listaFrontiere = cella1.getFrontiere();
      while(listaFrontiere.hasNext())
      {
        front = (Frontiera)listaFrontiere.next();
        punto = front.trovaIntersezione(cella);

        if(punto == null) continue;

        //if(punto != null)
        //{
        if(front1 == null)
        {
          front1 = front;
          punto1 = punto;
        }
        else
        {
          front2 = front;
          punto2 = punto;
          break;
        }
        //}
      } 


      // Serve per scorrere nella listaCelle
      if((front1 == null) || (front2 == null))
      {
        continue;
      }

      // la nuova frontiera fra cella e cella1
      newFront = new Frontiera(cella, cella1, punto1, punto2);
      // per ogni frontiera trovata si aggiunge la nuova frontiera e il nuovo vicino
      aggiungiFrontiera(cella, newFront);
      aggiungiFrontiera(cella1, newFront); 


      if(cella1.getKernel().distance(front1.getPuntoUno()) < 
          cella.getKernel().distance(front1.getPuntoUno()))
      {
        limit = front1.getPuntoDue();
        front1.setPuntoDue(punto1);
      }
      else
      {
        limit = front1.getPuntoUno();
        front1.setPuntoUno(punto1);
      }

      if(front1.isEdge())
      {
        aggiungiFrontiera(cella,new Frontiera(cella, null, punto1, limit));
      }

      ArrayList oldFront = new ArrayList();
      listaFrontiere = cella1.getFrontiere();
      while(listaFrontiere.hasNext())
      {
        front = (Frontiera)listaFrontiere.next();

        if(front == front1) 
          continue;
        if(front == front2) 
          continue;
        if(front == newFront) 
          continue;

        // Scorre sui punti frontiera della cella e verifica
        // se sono stati aggiornati, eventualmente gli elimina.
        if(front.getPuntoUno().distance(cella.getKernel()) <
            front.getPuntoUno().distance(cella1.getKernel()))
        {
          oldFront.add(front);
          if(front.isEdge())
          {
            aggiungiFrontiera(cella,front);
            front.setCellaUno(cella);
          }


        }
      }


      listaFrontiere = oldFront.iterator();
      while(listaFrontiere.hasNext())
      {
        this.cancellaFrontiera(cella1,(Frontiera)listaFrontiere.next());
      }


      cella2 = cella1; //conserva il riferimento alla prima cella visionata
      celleFatte.add(cella1);

      //Trova uno delle frontiere della cella sucessiva che
      //non sia per quella comune alla cella precedente(cella1)
      while((!(front1.isEdge())) && (!loop))
      {
        cella1 = front1.getVicinoDi(cella1);

        listaFrontiere = cella1.getFrontiere();
        while(listaFrontiere.hasNext())
        {
          front3 = (Frontiera)listaFrontiere.next();
          punto3 = front3.trovaIntersezione(cella);

          if((punto3 != null) && (front3 != front1))
            break;
        }


        if(front3 == front2)
        {
          punto3 = punto2;
          loop = true;
        } 

        newFront = new Frontiera(cella, cella1, punto1, punto3);
        this.aggiungiFrontiera(cella, newFront);
        this.aggiungiFrontiera(cella1, newFront);

        if(cella1.getKernel().distance(front3.getPuntoUno()) < 
            cella.getKernel().distance(front3.getPuntoUno()))
        {
          limit = front3.getPuntoDue();
          front3.setPuntoDue(punto3);
        }
        else
        {
          limit = front3.getPuntoUno();
          front3.setPuntoUno(punto3);
        }



        if(front3.isEdge())
          this.aggiungiFrontiera(cella, new Frontiera(cella, null, punto3, limit));

        oldFront.clear(); //Serve per reinizilizzare l'oldFront
        listaFrontiere = cella1.getFrontiere();
        while(listaFrontiere.hasNext())
        {
          front = (Frontiera)listaFrontiere.next();
          if(front == front1) 
            continue;
          if(front == front3) 
            continue;
          if(front == newFront) 
            continue;

          // Controllo sui punti consecutivi dello scheletro della
          // cella successiva(analogamente a prima). Se un punto pi
          // vicino alla nuova cella(quella entrante o corrente) che
          // alla cella alla quale appartenevano  vuol dire che non le 
          // appartengono pi!
          if(front.getPuntoUno().distance(cella.getKernel())
              < front.getPuntoUno().distance(cella1.getKernel()))
          {
            oldFront.add(front);

            if(front.isEdge())
            {
              this.aggiungiFrontiera(cella, front);
              front.setCellaUno(cella);
            }

          }
        }


        listaFrontiere = oldFront.iterator();
        while(listaFrontiere.hasNext())
        {
          this.cancellaFrontiera(cella1, (Frontiera)listaFrontiere.next());
        }



        front1 = front3;
        punto1 = punto3;
        celleFatte.add(cella1);

      } 



      cella1 = cella2;
      if(!loop)
      {
        if(cella1.getKernel().distance(front2.getPuntoUno()) <
            cella.getKernel().distance(front2.getPuntoUno()))
        {
          limit = front2.getPuntoDue();
          front2.setPuntoDue(punto2);
        }
        else
        {
          limit = front2.getPuntoUno();
          front2.setPuntoUno(punto2);
        }



        if(front2.isEdge())
          this.aggiungiFrontiera(cella, new Frontiera(cella, null, punto2, limit));
      }
      if(!loop)
      {

        while((!front2.isEdge()))
        {
          cella1 = front2.getVicinoDi(cella1);       
          listaFrontiere = cella1.getFrontiere();
          while(listaFrontiere.hasNext())
          {
            front3 = (Frontiera)listaFrontiere.next();
            punto3 = front3.trovaIntersezione(cella);

            if((punto3 != null) && (front3 != front2))
              break;
          }



          if(front3 == front1)
          {
            throw new NullPointerException();
          }
          //   System.out.println("Problema, Hai beccato lo" +
          //     "stesso punto di rottura di prima" + "Ciclo infinito!!");

          yyy++;
          if(yyy > 20)
          {
            throw new NullPointerException();
          }

          newFront = new Frontiera(cella, cella1, punto2, punto3);
          this.aggiungiFrontiera(cella, newFront);
          this.aggiungiFrontiera(cella1, newFront);

          if(cella1.getKernel().distance(front3.getPuntoUno()) < 
              cella.getKernel().distance(front3.getPuntoUno()))
          {
            limit = front3.getPuntoDue();
            front3.setPuntoDue(punto3);
          }
          else
          {
            limit = front3.getPuntoUno();
            front3.setPuntoUno(punto3);
          }


          if(front3.isEdge())
            aggiungiFrontiera(cella, new Frontiera(cella, null, punto3, limit));

          oldFront.clear();
          listaFrontiere = cella1.getFrontiere();
          while(listaFrontiere.hasNext())
          {

            front = (Frontiera)listaFrontiere.next();
            if(front == front2) 
              continue;
            if(front == front3) 
              continue;
            if(front == newFront) 
              continue;


            if(front.getPuntoUno().distance(cella.getKernel()) < 
                front.getPuntoUno().distance(cella1.getKernel()))
            {
              oldFront.add(front);

              if(front.isEdge())
              {
                this.aggiungiFrontiera(cella, front);
                front.setCellaUno(cella);
              }

            }
          }


          listaFront = oldFront.iterator();
          while(listaFront.hasNext())
          {
            this.cancellaFrontiera(cella1, (Frontiera)listaFront.next());
          }

          celleFatte.add(cella1);
          front2 = front3;
          punto2 = punto3;

        } 
      }

      oldFront.clear();
      boolean edge1 = false, edge2 = false, edge3 = false, edge4 = false;

      Iterator listaFrontiereCella = cella.getFrontiere();
      while(listaFrontiereCella.hasNext())
      {            
        front = (Frontiera) listaFrontiereCella.next();
        if(isUguale(front.getPuntoUno().getX(), maxX) && 
            isUguale(front.getPuntoDue().getX(), maxX))
        {
          if(edge2)
          {
            punto1 = front2.getPuntoUno();
            punto2 = front2.getPuntoUno();

            if(punto1.getY() < front2.getPuntoDue().getY())
              punto1 = front2.getPuntoDue();

            if(punto2.getY() > front2.getPuntoDue().getY()) 
              punto2 = front2.getPuntoDue();

            if(punto1.getY() < front.getPuntoUno().getY()) 
              punto1 = front.getPuntoUno();

            if(punto2.getY() > front.getPuntoUno().getY()) 
              punto2 = front.getPuntoUno();

            if(punto1.getY() < front.getPuntoDue().getY())
              punto1 = front.getPuntoDue();

            if(punto2.getY() > front.getPuntoDue().getY()) 
              punto2 = front.getPuntoDue();


            front.setPuntoUno(punto1);
            front.setPuntoDue(punto2);

            oldFront.add(front2);
            front2 = front;
          }
          else
          {
            edge2 = true;
            front2 = front;
          }
        } 

        if(isUguale(front.getPuntoUno().getX(), 0) &&
            isUguale(front.getPuntoDue().getX(), 0))
        {
          if(edge4)
          {
            punto1 = front4.getPuntoUno();
            punto2 = front4.getPuntoUno();


            if(punto1.getY() < front4.getPuntoDue().getY())
              punto1 = front4.getPuntoDue();

            if(punto2.getY() > front4.getPuntoDue().getY()) 
              punto2 = front4.getPuntoDue();

            if(punto1.getY() < front.getPuntoUno().getY())
              punto1 = front.getPuntoUno();

            if(punto2.getY() > front.getPuntoUno().getY()) 
              punto2 = front.getPuntoUno();

            if(punto1.getY() < front.getPuntoDue().getY())
              punto1 = front.getPuntoDue();

            if(punto2.getY() > front.getPuntoDue().getY())
              punto2 = front.getPuntoDue();


            front.setPuntoUno(punto1);
            front.setPuntoDue(punto2);

            oldFront.add(front4);
            front4=front;
          }
          else
          {
            edge4 = true;
            front4 = front;
          }
        }

        if(isUguale(front.getPuntoUno().getY(),0)
            && isUguale(front.getPuntoDue().getY(), 0))
        {
          if(edge1)
          {
            punto1 = front1.getPuntoUno();
            punto2 = front1.getPuntoUno();


            if(punto1.getX() > front1.getPuntoDue().getX())
              punto1 = front1.getPuntoDue();

            if(punto2.getX() < front1.getPuntoDue().getX()) 
              punto2 = front1.getPuntoDue();

            if(punto1.getX() > front.getPuntoUno().getX()) 
              punto1 = front.getPuntoUno();

            if(punto2.getX() < front.getPuntoUno().getX()) 
              punto2 = front.getPuntoUno();

            if(punto1.getX() > front.getPuntoDue().getX())
              punto1 = front.getPuntoDue();

            if(punto2.getX() < front.getPuntoDue().getX()) 
              punto2 = front.getPuntoDue();


            front.setPuntoUno(punto1);
            front.setPuntoDue(punto2);

            oldFront.add(front1);
            front1=front;
          }
          else
          {
            edge1 = true;
            front1 = front;
          }
        } 

        if(isUguale(front.getPuntoUno().getY(), maxY) && 
            isUguale(front.getPuntoDue().getY(), maxY))
        {
          if(edge3)
          {
            punto1 = front3.getPuntoUno();
            punto2 = front3.getPuntoUno();


            if(punto1.getX() > front3.getPuntoDue().getX())
              punto1 = front3.getPuntoDue();

            if(punto2.getX() < front3.getPuntoDue().getX())
              punto2 = front3.getPuntoDue();

            if(punto1.getX() > front.getPuntoUno().getX())
              punto1 = front.getPuntoUno();

            if(punto2.getX() < front.getPuntoUno().getX()) 
              punto2 = front.getPuntoUno();

            if(punto1.getX() > front.getPuntoDue().getX()) 
              punto1 = front.getPuntoDue();

            if(punto2.getX() < front.getPuntoDue().getX())
              punto2 = front.getPuntoDue();


            front.setPuntoUno(punto1);
            front.setPuntoDue(punto2);

            oldFront.add(front3);
            front3=front;
          }
          else
          {
            edge3 = true;
            front3 = front;
          }
        } 

      } 



      listaFront = oldFront.iterator();
      while(listaFront.hasNext())
      {
        front = (Frontiera) listaFront.next();
        this.cancellaFrontiera(cella, front);
      }

    } 

  }

  public boolean aggiungiFrontiera(Cella sito, Frontiera frontier)
  {
    Cella p = frontier.getVicinoDi(sito);

    return sito.getVfrontiere().add(frontier);
  }

  public boolean cancellaFrontiera(Cella sito, Frontiera frontier)
  {
    Cella p = frontier.getVicinoDi(sito);

    return sito.getVfrontiere().remove(frontier);
  }

  public void cancellaCella(Cella sito)
  {
    if(sito == null) return;

    if(celle.contains(sito))
    {
      celle.remove(sito);
      ArrayList trasporto = (ArrayList)celle.clone();
      this.clear();
      Iterator listaTrasporto = trasporto.iterator();
      while(listaTrasporto.hasNext())
      {
        Cella elementoCorrente = (Cella)listaTrasporto.next();
        elementoCorrente.getVfrontiere().clear();
        aggiungiCella(elementoCorrente);
      }

    }
  }


  /*
     public int cancellaCella(Cella sito)
     {
     int indice = 0;
     if(sito!=null)
     {
     Point2D.Double punto;
     if(celle.contains(sito))
     {
     indice = celle.indexOf(sito);
     celle.remove(sito);
     ArrayList trasporto = (ArrayList)celle.clone();
     this.clear();
     Iterator listaTrasporto = trasporto.iterator();
     while(listaTrasporto.hasNext())
     {
     Cella elementoCorrente = (Cella)listaTrasporto.next();
     elementoCorrente.getVfrontiere().clear();
     aggiungiCella(elementoCorrente);
     }
     }
     }
     return indice;
     }
     */

  public void clear()
  {
    celle.clear();
  }

  public Iterator getCelle()
  {
    return celle.iterator();
  }

  public ArrayList getVCelle()
  {
    return celle;
  }


  public Cella trovaSito(Point2D.Double unPunto)
  {
    for(Iterator listaSiti = this.getCelle(); listaSiti.hasNext();)
    {
      Cella sitoCorrente = (Cella)listaSiti.next();
      // Qua esiste un'inconsistenza con i pixel, non sempre!
      if(sitoCorrente.getKernel().distance(unPunto) < (DISTANZA_MIN / 2 - 0.1))            
      {
        return sitoCorrente;
      }
    }
    return null;
  }

  public Cella trovaSitoDragged(Point2D.Double unPunto, double zom)
  {
    Cella tempSito = null;
    for(Iterator listaSiti = this.getCelle(); listaSiti.hasNext();)
    {
      Cella sitoCorrente = (Cella)listaSiti.next();
      if(sitoCorrente.getKernel().distance(unPunto) < (DISTANZA_MIN * 30 / (zom * 0.6))) // Qua esiste un problema di tunning           
      {
        if(tempSito == null)
        {
          tempSito = sitoCorrente;
        }
        else
        {
          if(sitoCorrente.getKernel().distance(unPunto) < tempSito.getKernel().distance(unPunto))
          {
            tempSito = sitoCorrente;
          }
        }
      }              
    }
    return tempSito;
  }

  public Dimension getSize()
  {
    return new Dimension((int)maxX, (int)maxY);
  }

  public boolean isUguale(double num, int numIntero)
  {
    double numero = (double)numIntero;
    if((num != 0) && (numero != 0))
    {
      return ((Math.abs(num - numero) / 
            Math.max(Math.abs(num), Math.abs(numero))) <= EPSI);
    }
    else
    {
      return (Math.abs(num - numero) <= EPSI);
    }
  }


  public void aggiungiCellaDragged(Cella sito)
  {
    if(sito == null) return;

    ArrayList trasportoBis = (ArrayList)getVCelle().clone();
    this.clear();
    DISTANZA_MIN = 1;
    trasportoBis.add(sito);
    Iterator listaTrasporto = trasportoBis.iterator();
    while(listaTrasporto.hasNext())
    {
      Cella elementoCorrente = (Cella)listaTrasporto.next();
      elementoCorrente.getVfrontiere().clear();
      aggiungiCella(elementoCorrente);
    }
    DISTANZA_MIN = 5;
  }







  /*
     public void aggiungiCellaDragged(Cella sito,  int indice)
     {
     if(sito != null)
     {
     int indiceAggiungi = indice;
  //ArrayList trasportoBis = (ArrayList)((getVCelle()).clone());
  ArrayList trasportoBis = new ArrayList();
  for(int i = 0; i < celle.size(); i++)
  {
  Cella testCella = (Cella)celle.get(i);
  Cella cellaClone = (Cella)testCella.clone();
  System.out.println("Cella Clone " + cellaClone.getColore());
  trasportoBis.add(cellaClone);
  }

  this.clear();
  DISTANZA_MIN = 1;
  trasportoBis.add(indiceAggiungi, sito);
  Iterator listaTrasport = trasportoBis.iterator();
  while(listaTrasport.hasNext())
  {
  Cella elementoCorrente = (Cella)listaTrasport.next();
  ////elementoCorrente.getVfrontiere().clear();
  Point2D.Double point = elementoCorrente.getKernel();
  boolean col = elementoCorrente.getColore();
  System.out.println("Cella trasposrto " + col);
  Cella nuova = new Cella(point);
  nuova.setColore(col);
  aggiungiCella(elementoCorrente);
  }
  DISTANZA_MIN = 5;
     }
     }
     */ 


  //Metodi per il salvataggio dei punti
  public void salvaDati(FileWriter file) throws IOException                                                   
  {
    PrintWriter out = new PrintWriter(file);
    Iterator listaCelle = celle.iterator();
    while(listaCelle.hasNext())
    {
      Cella cella = (Cella)(listaCelle.next());
      Point2D.Double kernel = cella.getKernel();
      double x = kernel.getX();
      double y = kernel.getY();
      boolean colore = cella.getColore();
      out.println(x + "|" + y + "|" + Boolean.toString(colore));
    }
    file.close();
  }

  public void caricaDati(FileReader file) throws IOException 
  {
    //this.clear(); Vedere se seve cosi o diversamente!!!
    BufferedReader input = new BufferedReader(file);
    String riga;
    while((riga = input.readLine()) != null)
    {
      StringTokenizer divisore = new StringTokenizer(riga, "|");
      try
      {
        double x = Double.parseDouble(divisore.nextToken());
        double y = Double.parseDouble(divisore.nextToken());
        boolean colore = (Boolean.valueOf(divisore.nextToken())).booleanValue();
        Point2D.Double p = new Point2D.Double(x, y);
        Cella cella = new Cella (p);
        cella.setColore(colore);
        this.aggiungiCella(cella);
      }
      catch(NumberFormatException ec)
      {
        //questo va gestito!!!
      }
      catch(NoSuchElementException ecc)
      {
        //questo va gestito!!!
      }
    }
  }

  public void cancellaArea(RoundRectangle2D unRettangolo, double zoom)
  {
    Cella cella = null;
    RoundRectangle2D rett = unRettangolo;
    Iterator myEnum = celle.iterator();
    Point2D.Double punto;
    ArrayList siti = new ArrayList();

    while(myEnum.hasNext())
    {
      cella = (Cella)myEnum.next();
      double x = (cella.getKernel()).getX() * zoom;
      double y = (cella.getKernel()).getY() * zoom;

      if(rett.contains(x, y))
      {
        siti.add(cella);                   
      }

    }
    for(int k = 0; k < siti.size(); k++)
    {
      celle.remove((Cella)siti.get(k));
    }

    ArrayList trasporto = (ArrayList)celle.clone();
    this.clear();
    Iterator listaTrasporto = trasporto.iterator();
    while(listaTrasporto.hasNext())
    {
      Cella elementoCorrente = (Cella)listaTrasporto.next();
      punto = elementoCorrente.getKernel();
      cella = new Cella(punto);
      cella.setColore(elementoCorrente.getColore());
      aggiungiCella(cella);
    }

  }

  public void help()
  {
    celle.remove(celle.size() - 1);
    ArrayList trasporto = (ArrayList)celle.clone();
    this.clear();
    Iterator listaTrasporto = trasporto.iterator();
    while(listaTrasporto.hasNext())
    {
      Cella elementoCorrente = (Cella)listaTrasporto.next();
      elementoCorrente.getVfrontiere().clear();
      aggiungiCella(elementoCorrente);

    }
  }


  private ArrayList celle;
  private static  int DISTANZA_MIN = 5; // Distanza minima tra due siti
  private int maxX, maxY;
  private static final double EPSI = 1E-14;
  private int yyy;


}
