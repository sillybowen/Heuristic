package voronoi;

import java.net.*;
import java.io.*;
import java.util.*;
import java.awt.*;
import java.awt.geom.Point2D;
import java.awt.image.BufferedImage;
import javax.imageio.ImageIO;

public class Client {
  public Client(String nomeUtente, char[] pass,  String nomeFile, String email, Diagramma unDiagramma)
  {
    try
    {
      //int indice = email.indexOf('@');
      //this.nomeFile = (nomeFile.concat(email.substring(0, indice))) + ".txt";
      this.nomeFile = nomeFile;
      this.email = email;
      this.voronoi = unDiagramma;
      this.utente = nomeUtente;
      this.password = new String(pass);
    }
    catch(NullPointerException ecc)
    {
      //Questo va gestito!!!!!   
    }

    try
    {
      InetAddress indirizzo = InetAddress.getLocalHost();//Questo va usato solo per le prove
      //InetAddress indirizzo = InetAddress.getByName("www.MyServer.it"); // Questo quello giusto
      Socket canale = new Socket(indirizzo, VORONOI_PORT);
      in = new BufferedReader(new InputStreamReader(canale.getInputStream()));
      outStream = canale.getOutputStream();
      out = new PrintWriter (outStream, true);//Fa l'auto flush

    }
    catch(IOException ecc)
    {

    }
  }


  public void salvaDati() 
  {
    if((utente != null) && (password != null) && (nomeFile != null))
    {
      if(email == null)
        out.println("SaveAsCommand" + "|" + utente + "|" + password + "|" + nomeFile);
      else
        out.println("SaveAsCommand" + "|" + utente + "|" + password + "|" + nomeFile + "|"+ email);

      Iterator listaCelle = voronoi.getCelle();
      while(listaCelle.hasNext())
      {
        Cella cella = (Cella)(listaCelle.next());
        Point2D.Double kernel = cella.getKernel();
        double x = kernel.getX();
        double y = kernel.getY();
        boolean colore = cella.getColore();
        out.println(x + "|" + y + "|" + Boolean.toString(colore));
      }
      out.println("Fine");
    }

  }

  public void caricaDati()
  {
    if((utente != null) && (password != null) && (nomeFile != null))
    {
      if(email == null)
        out.println("LoadCommand" + "|" + utente + "|" + password + "|" + nomeFile);
      else
        out.println("LoadCommand" + "|" + utente + "|" + password + "|" + nomeFile + "|"+ email);
      //out.flush();
    }

    String riga;
    try
    {
      while(!(((riga = in.readLine())).equals("Fine")))
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
          voronoi.aggiungiCella(cella);
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
    catch(IOException ecce)
    {
      //Va gestito!!!
    }

  }


  public void salvaImmagine(BufferedImage image)
  {
    if((utente != null) && (password != null) && (nomeFile != null) && (email != null))
    {
      out.println("SalvaImmagine" + "|" + utente + "|" + password + "|" + nomeFile + "|"+ email);
    }

    try
    {
      ImageIO.write(image, "JPEG", ImageIO.createImageOutputStream(outStream));

      //ImageIO.write(image, "JPEG", outStream);
    }        
    catch(IOException ee){  }
    out.println("Sal" + "|" + utente + "|" + password + "|" + nomeFile + "|"+ email);
  }
  public void spedisciMail()
  {
    out.println("SpedisciMail" + "|" + utente + "|" + password + "|" + nomeFile + "|"+ email);
  }




  public void cancellaFile()
  {
    out.println("CancellaFile" + "|" + utente + "|" + password + "|" + nomeFile);
  }

  public void holdOff()
  {
    out.println("END_TRASMISSION" + "|" + utente + "|" + password + "|" + nomeFile);
    //out.flush();
  }
  public void setNomeFile(String nome)
  {
    nomeFile = nome;
  }

  public void setIstanza(String unUtente, char[] unPassword, String unFile, String unEmail)
  {
    utente = unUtente;
    password = new String(unPassword);
    nomeFile = unFile;
    email = unEmail;
  }

  public Vector riceviFiles()
  {
    Vector listaFiles = new Vector();
    String riga;
    try
    {
      while(!(((riga = in.readLine())).equals("Fine")))
      {                 
        listaFiles.add(riga);                      
      }
    }
    catch(IOException ecce)
    {
      //Va gestito!!!
    }
    return listaFiles;
  }

  public void dammiListaFiles()
  {
    if((utente != null) && (password != null) && (nomeFile != null))
    {
      if(email == null)
        out.println("DammiFile" + "|" + utente + "|" + password + "|" + nomeFile);
      else
        out.println("DammiFile" + "|" + utente + "|" + password + "|" + nomeFile + "|"+ email);


      //out.flush();
    }
  }




  private Diagramma voronoi;
  private static final int VORONOI_PORT = 1212;
  private String utente; 
  private String password;
  private String nomeFile;
  private String email;
  private PrintWriter out;
  private BufferedReader in;
  private OutputStream outStream;


}
