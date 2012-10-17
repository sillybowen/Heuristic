package voronoi;

import javax.swing.JApplet;
import java.applet.Applet;
import java.awt.*;
import java.awt.event.*;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.Random;
import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextField;
import javax.swing.JLabel;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.text.*;
import javax.swing.border.EtchedBorder;
import javax.swing.border.TitledBorder;
import java.awt.geom.Point2D;
import java.awt.geom.Rectangle2D;
import java.awt.geom.Ellipse2D;
import javax.swing.event.MouseInputAdapter;
import javax.swing.border.*;
import javax.swing.JMenuItem;
import java.text.*;
import java.io.FileWriter;
import java.io.FileReader;
import java.io.IOException;
import java.io.File;
import java.awt.image.BufferedImage;
import javax.imageio.ImageIO;
import java.awt.geom.RoundRectangle2D;
import javax.print.*;
import javax.print.attribute.*;
import java.awt.print.*;
import java.beans.*;
import java.net.URL;
//import java.net.MalformedURLException;

import voronoi.utils.Constants;
import java.awt.Robot;

public class VoronoiGame extends JApplet {
    int move = 0;
  public void humanPutStone(int x, int y, String plyName) {
    System.out.println(plyName + " put a stone at (" + x + ", " + y + ")");
    if (move == 0) {
        areaBlu.setText("Blue's ("+plyName+") score");
        move++;
    }
    else  if (move == 1) {
      areaRed.setText("Red's ("+plyName+") score");
      move++;
  }
    // sitoCorrente = diagramma.trovaSitoDragged((new
    //       Point2D.Double(x / zoom, y / zoom)), zoom);

    Cella nuova = new Cella(x, y);
    aggiungiCella(nuova);

    setCampoBlue();
    setCampoRosso();

    game.revalidate();
    game.repaint(); 
    repaint();
    System.out.println("Red:"+game.areaTotaleRosso());
    System.out.println("Bllue:"+game.areaTotaleBlue());
  }
    public double RedArea()  {
        return game.AreaRed();
    }
    public double BlueArea()  {
        return game.AreaBlue();
    }



  public void init() {
    sisOperativo = System.getProperty("os.name");
    ProgressMonitor progressMonitor = new ProgressMonitor(VoronoiGame.this,
        "Loading data... please wait","", 0, 120);

    progressMonitor.setProgress(0);
    progressMonitor.setMillisToDecideToPopup(10);
    //final String separatorOfLines = System.getProperty("line.Separator");

    if(!isApplication)
      showStatus("Loding data from server... please wait");

    gridOkIcon = new ImageIcon(VoronoiGame.class.getResource("ok3.GIF"));
    gridExitIcon = new ImageIcon(VoronoiGame.class.getResource("exit.gif"));

    JButton salvaButton = new
      JButton(new ImageIcon(VoronoiGame.class.getResource("Save24.gif")));
    salvaButton.setBackground(colore);

    JButton clearButton = new
      JButton(new ImageIcon(VoronoiGame.class.getResource("New24.gif")));
    clearButton.setBackground(colore);

    JButton cancelButton = new
      JButton(new ImageIcon(VoronoiGame.class.getResource("Delete24.gif")));
    cancelButton.setBackground(colore);

    JButton testoButton = new
      JButton(new ImageIcon(VoronoiGame.class.getResource("Ok2.gif")));
    testoButton.setBackground(colore);

    class CustomMouseListener extends MouseInputAdapter {
      private Cursor tagliaCursor = null;
      private Cursor zoomInCursor = null;
      private Cursor zoomOutCursor = null;
      private Cursor crossCursor = null;
      private Cursor defaultCursor = null;
      private Cursor handCursor = null;

      private Cella p = null;

      public void mousePressed(MouseEvent evento) {
        if ((evento.getModifiers() == evento.BUTTON1_MASK)) {
          if (!isZoomIn && !isZoomOut) {
            zoom = game.getZoom();
            sitoCorrente = diagramma.trovaSito(new Point2D.Double(evento.getX()/zoom, evento.getY() / zoom));

            if (sitoCorrente == null && (!isDragged)) {
              if (isChecked) {
                griglia = game.dammiGriglia();
                int k = griglia.getKappa();
                int l = griglia.getElle();
                try {
                  Cella nuova = new Cella( 
                      griglia.roundToGriglia(evento.getX()/zoom, Math.round((float)(k/zoom))),  
                      griglia.roundToGriglia(evento.getY()/zoom, Math.round((float)(l/zoom))));
                  aggiungiCella(nuova);
                } catch(Exception ecc) {
                  JOptionPane.showMessageDialog(VoronoiGame.this,
                      "This algorithm is affected from numerical approximation errors."
                      + System.getProperty("line.separator") +
                      "The game now will be reset. Please be patient... we are working on!",
                      "About approximation problems.", 
                      JOptionPane.ERROR_MESSAGE);
                  VoronoiGame.this.reset();
                }
              } else {
                try {
                  Cella nuova = new Cella(evento.getX()/zoom, evento.getY() / zoom);
                  aggiungiCella(nuova);
                } catch(Exception ecc) {
                  JOptionPane.showMessageDialog(VoronoiGame.this,
                      "This algorithm is affected from numerical approximation errors. "
                      + System.getProperty("line.separator") +
                      "The game will be reset. Please be patient... we are working on!",
                      "About approximation problems.", 
                      JOptionPane.ERROR_MESSAGE);
                  VoronoiGame.this.reset();
                }
              }
            }

            setCampoBlue();
            setCampoRosso();

            game.revalidate();
            game.repaint(); 
            repaint();
          } else {
            if (isZoomIn) {
              game.setZoom(game.getZoom()* 1.25);
              if (gridFrame != null) {
                gridFrame.getGridFlag();
              }
              game.revalidate();
              game.repaint(); 
              repaint();
            }
            if (isZoomOut) {
              game.setZoom(game.getZoom() / 1.25);
              if (gridFrame != null) {
                gridFrame.getGridFlag();
              }
              game.revalidate();
              game.repaint(); 
              repaint();
            }
          }
        }
        if((sisOperativo.toLowerCase()).startsWith("lin"))
        {
          if((evento.isPopupTrigger()) && (evento.getModifiers() == evento.BUTTON3_MASK))
          {
            popup.show(evento.getComponent(), evento.getX(), evento.getY());
          }
        }
      }

      public void mouseClicked(MouseEvent evento)
      { 
        if(!isZoomIn && !isZoomOut)
        {
          zoom = game.getZoom();
          sitoCorrente = diagramma.trovaSitoDragged((new Point2D.Double(evento.getX()/zoom,
                  evento.getY() / zoom)), zoom);
          // ------------------------------
          // System.out.println("Clicked stone: x= " + evento.getX() / zoom +
          //     " y = " + evento.getY() / zoom);
          // ----------------------------
          if((sitoCorrente != null) && (evento.getClickCount() >= 2))
          {
            diagramma.cancellaCella(sitoCorrente);
            if(!bOneRound)
              bColore = sitoCorrente.getColore();
          }

          setCampoBlue();
          setCampoRosso();

          game.revalidate();
          game.repaint(); 
          repaint();
        }
      }

      public void mouseMoved(MouseEvent evento)
      {
        zoom = game.getZoom();

        coordinate.setText("(" + Math.round((float)(evento.getX()/zoom)) + 
            ", " + Math.round((float)(evento.getY() / zoom))+ ")");

        if(diagramma.trovaSito(new Point2D.Double(evento.getX()/zoom,
                evento.getY() / zoom)) != null)
        {
          if(crossCursor == null)
            crossCursor = Cursor.getPredefinedCursor(Cursor.CROSSHAIR_CURSOR);
          setCursor(crossCursor);
        }
        else if((diagramma.trovaSitoDragged(new Point2D.Double(evento.getX()/zoom,
                  evento.getY() / zoom), zoom) != null) && isDragged)
        {
          if(defaultCursor == null)
            defaultCursor = Cursor.getDefaultCursor();
          setCursor(defaultCursor);
        }
        else if(isZoomIn)
        {
          if(zoomInCursor == null)
          {
            Toolkit tk = Toolkit.getDefaultToolkit();
            zoomInCursor = tk.createCustomCursor(tk.getImage(VoronoiGame.class.getResource("ZoomIn16.gif")), new Point(10,10), "ZoomIn");
          }
          setCursor(zoomInCursor);
        }
        else if(isZoomOut)
        {
          if(zoomOutCursor == null)
          {
            Toolkit tk = Toolkit.getDefaultToolkit();
            zoomOutCursor = tk.createCustomCursor(tk.getImage(VoronoiGame.class.getResource("ZoomOut16.gif")), new Point(10,10),"ZoomOut");
          }
          setCursor(zoomOutCursor);
        }
        else
        {
          if(handCursor == null)
            handCursor = Cursor.getPredefinedCursor(Cursor.HAND_CURSOR);
          setCursor(handCursor);
        }
      }

      public void mouseDragged(MouseEvent even)
      {
        zoom = game.getZoom();
        coordinate.setText("(" + Math.round((float)(even.getX()/zoom)) + ", "
            + Math.round((float)(even.getY() / zoom))+ ")");

        if(even.getModifiers() == even.BUTTON1_MASK)
        {
          int indiceCella = 0;
          if(p == null)
          {
            p = diagramma.trovaSitoDragged((new Point2D.Double(even.getX()/zoom ,
                    even.getY()/zoom)), zoom);
            if(p == null)
            {
              isDragged = false;
              game.setDragged(false);

              item0.setForeground(Color.black);
              item0.setText("Dragg Off");
              return;
            }
            if(isDragged)
            {

              retta = new Rectangle2D.Double(even.getX() - diam/2, even.getY() - diam/2, diam, diam);
              game.setRettangolo(retta); 

            }  
          }
          else
          {
            boolean col = p.getColore();
            diagramma.cancellaCella(p);
            p = new Cella(even.getX()/zoom, even.getY()/zoom, col);
            if(isDragged)
              game.setRettangolo(new Rectangle2D.Double(even.getX()- diam/2, even.getY()-diam/2, diam, diam));
            Cella c = diagramma.trovaSito(new Point2D.Double(even.getX()/zoom, even.getY()/zoom));
            if(c == null)
              diagramma.aggiungiCellaDragged(p);
            areaCellaDragged.setText("(" + calcolaAreaCella(p) + ")");
            testoAreaCella.setVisible(true);
            areaCellaDragged.setVisible(true);

            setCampoBlue();
            setCampoRosso();
            game.revalidate();
            game.repaint();
            repaint();
          }
        }

        if(even.getModifiers() == even.BUTTON3_MASK)
        {
          if(tagliaCursor == null)
          {
            Toolkit tk = Toolkit.getDefaultToolkit();
            tagliaCursor = tk.createCustomCursor(tk.getImage(VoronoiGame.class.getResource("cut.gif")), new Point(10,10), "taglia");
          }
          setCursor(tagliaCursor);    

          if(game.getP() == null)
          {
            game.setP(new Point2D.Double(even.getX(), even.getY()));
          }
          else
          {
            game.setQ(new Point2D.Double(even.getX(), even.getY()));
          }
          if(!game.getAreaCancellabile())
            game.setAreaCancellabile(true);     

          game.revalidate();
          game.repaint();
          repaint();
        }
      }


      public void mouseExited(MouseEvent evento)
      {
        if(evento.getComponent() == game)
        {
          setCursor(Cursor.getDefaultCursor());
        }
      }


      public void mouseReleased(MouseEvent evento)
      {
        double zoom = game.getZoom();
        testoAreaCella.setVisible(false);
        areaCellaDragged.setVisible(false);
        p = null;

        game.setAreaCancellabile(false);
        if((game.getP() == null) && (evento.isPopupTrigger()) && (evento.getModifiers() == evento.BUTTON3_MASK))
        {
          popup.show(evento.getComponent(), evento.getX(), evento.getY());
          if(!game.getViewGriglia() && nonPrimaVolta)
          {
            item6.setForeground(coloreT);
            item6.setText("Grid Disabled");
          }
          else if(nonPrimaVolta)
          {
            item6.setForeground(Color.red);
            item6.setText("Grid Enabled");

          }
          nonPrimaVolta = true;
        }
        if((game.getP() != null) && (evento.getModifiers() == evento.BUTTON3_MASK))
        {
          RoundRectangle2D rettangolo = game.getRect();
          diagramma.cancellaArea(rettangolo, zoom);
          game.setAreaCancellabile(false);
          setCampoBlue();
          setCampoRosso();
        }

        game.setP(null);
        game.setQ(null); //necessario, altrimenti si crea un rettangolo nell'istante 

        game.revalidate();
        game.repaint();
        repaint();

      }

    }

    class ClearListener implements ActionListener
    {

      public void actionPerformed(ActionEvent evento)
      {

        diagramma.clear();
        bColore = false;
        blu.setSelected(false);
        blu.setIcon(bluIcon);
        setCampoBlue();
        setCampoRosso();
        game.revalidate();
        game.repaint();
        repaint();
      }
    }

    class RandomListener implements ActionListener
    {

      public void actionPerformed(ActionEvent evento)
      {

        generator = new Random();
        for(int j = 1; j <= 16; j++)
        {

          int coordX =  generator.nextInt(dim_X);
          int coordY = generator.nextInt(dim_Y);

          if(isChecked)
          {
            griglia = game.dammiGriglia();
            double zoom = game.getZoom();
            int k = griglia.getKappa();
            int l = griglia.getElle();
            try
            {
              Cella nuova = new Cella( 
                  griglia.roundToGriglia(coordX, Math.round((float)(k/zoom))),  
                  griglia.roundToGriglia(coordY, Math.round((float)(l/zoom))));                                          
              aggiungiCella(nuova);
            }
            catch(Exception ecc)
            {

              JOptionPane.showMessageDialog(VoronoiGame.this,"This algorithm is affected from numerical approximation errors. " + System.getProperty("line.separator") +
                  "The game will be now reset. Please be patient... we are working on!",
                  "About approximation problems.", 
                  JOptionPane.ERROR_MESSAGE );

              VoronoiGame.this.reset();
            }
          }
          else
          {
            try
            {
              Cella nuova = new Cella(coordX, coordY);
              aggiungiCella(nuova);
            }
            catch(Exception ecc)
            {

              JOptionPane.showMessageDialog(VoronoiGame.this,"This algorithm is affected from numerical approximation errors. " + System.getProperty("line.separator") + 
                  "The game now will be reset. Please be patient... we are working on!",
                  "About approximation problems.", 
                  JOptionPane.ERROR_MESSAGE );
              VoronoiGame.this.reset();
            }
          }
        }


        setCampoBlue();
        setCampoRosso();

        game.revalidate();
        game.repaint();
        repaint();
      }
    }

    class TestoListener implements ActionListener
    {
      public void actionPerformed(ActionEvent evento)
      {
        setPoint();               

        setCampoBlue();
        setCampoRosso();
        game.revalidate();
        game.repaint();
        repaint();
      }
    }
    /**
     * Questa classe interna resposabile della verifica
     * di testo in ingresso.
     */
    class VerificatoreTesto extends InputVerifier
    {
      public boolean verify(JComponent componente)
      {
        String testo = ((JTextComponent)componente).getText();
        return isValid(testo);
      }
    }



    class RounderListener implements ActionListener
    {
      public void actionPerformed(ActionEvent event)
      {
        if(roundGriglia.isSelected())
        {
          setChecked(true);
          if(game.dammiGriglia() == null)
          {
            JOptionPane.showMessageDialog(bar,
                "<HTML><BODY text = \"#422C2C\"> <BLOCKQUOTE><H3>First you have to initialize a grid object!"
                + "<HR>Please choose the grid first.</H3></BLOCKQUOTE></BODY></HTML>",                             
                "About first snap to grid", JOptionPane.INFORMATION_MESSAGE, 
                new ImageIcon(VoronoiGame.class.getResource("duke.gif")));
            roundGriglia.setSelected(false);
            setChecked(false);
          }
        }
        else{setChecked(false);}
        game.revalidate();
        game.repaint();
        repaint();
      }
    }

    class CancelTestoListener implements ActionListener
    {
      public void actionPerformed(ActionEvent evento)
      {
        try
        {
          double coordX = Double.parseDouble(ascissaField.getText());
          double coordY = Double.parseDouble(ordinataField.getText());
          sitoCorrente = diagramma.trovaSito(new Point2D.Double(coordX,coordY));
          diagramma.cancellaCella(sitoCorrente);
          if((sitoCorrente != null) && (!bOneRound))
            bColore = sitoCorrente.getColore();
        }  
        catch (NumberFormatException e)
        {
          JOptionPane.showMessageDialog(VoronoiGame.this,"Only numbers are allowed! " + System.getProperty("line.separator")  + 
              "Please control that other symbols are not presented.",                       
              "About deleting values from keyboard.", 
              JOptionPane.WARNING_MESSAGE);
        }

        setCampoBlue();
        setCampoRosso();
        game.revalidate();
        game.repaint();
        repaint();
      }
    }

    class AscoltaEditZoom implements ActionListener
    {
      public void actionPerformed(ActionEvent e)
      {
        double zoom_factor = Double.parseDouble(JOptionPane.showInputDialog(VoronoiGame.this, "Insert the zooming factor in  %",
              "Zooming Input Dialog", JOptionPane.QUESTION_MESSAGE));
        zoom_factor /= 100;
        game.setZoom(game.getZoom() * zoom_factor);
        if(gridFrame == null)
        {
          gridFrame = new GridChooserFrame(game);
          gridFrame.getGridFlag();

        }
        else 
        {
          gridFrame.getGridFlag();
        }

        game.repaint(); 
        repaint();
      }
    }


    class AscoltaSaveItem implements ActionListener
    {
      public void actionPerformed(ActionEvent evt)
      {
        chooser.setCurrentDirectory(new File("."));
        chooser.setSelectedFile(new File("Game.txt"));
        if(fileSave == null)
        {
          int result = chooser.showSaveDialog(VoronoiGame.this);
          if(result == JFileChooser.APPROVE_OPTION)
          {
            fileSave = chooser.getSelectedFile();
            try
            {
              FileWriter file = new FileWriter(fileSave);
              diagramma.salvaDati(file);
            }
            catch(IOException eccezione)
            {
              System.exit(-1);
            }
          }
        }
        else
        {
          try
          {
            file = new FileWriter(fileSave);
            diagramma.salvaDati(file);
          }
          catch(IOException eccezione)
          {

          }
        }


      }
    }



    class AscoltaFileSave implements ActionListener
    {
      public void actionPerformed(ActionEvent evt)
      {
        //chooser.setCurrentDirectory(new File("C:/VoronoiGame"));
        chooser.setSelectedFile(new File("Game.txt"));
        // show file chooser dialog
        int result = chooser.showSaveDialog(VoronoiGame.this);
        if(result == JFileChooser.APPROVE_OPTION)
        {
          fileSave = chooser.getSelectedFile();
          try
          {
            FileWriter file = new FileWriter(fileSave);
            diagramma.salvaDati(file);
          }
          catch(IOException eccezione)
          {
            System.exit(-1);
          }
        }
      }
    }

    class AscoltaSaveAsNet implements ActionListener
    {
      public void actionPerformed(ActionEvent evt)
      {

        if(utente == null)
        {
          JOptionPane.showMessageDialog(VoronoiGame.this,"Please register first at " +
              "Electronic Registration in Help menu!","About user's registration", 
              JOptionPane.ERROR_MESSAGE);
          return;
        }
        if(cliente == null)
          cliente = new Client(utente, password, nomeFile, email, diagramma);
        else
          cliente.setIstanza(utente, password, nomeFile, email);                

        if(fileFrameSave == null)
        {

          if(listModel == null)
          {
            cliente.dammiListaFiles();
            listModel = new DefaultListModel();
            Enumeration datiEnum = (cliente.riceviFiles()).elements();
            while(datiEnum.hasMoreElements())
            {
              listModel.addElement((String)datiEnum.nextElement());
            }
          }

          fileFrameSave = new FileChooserFrame("Save",listModel,cliente, game);
          fileFrameSave.setVisible(true);

        }
        else
        {
          fileFrameSave.setVisible(true);
        }

        //nomeFile = fileFrame.getNomeFile();             
        //cliente.setIstanza(utente, password, nomeFile, email);
        //cliente.salvaDati();

      }
    }



    class AscoltaSaveNet implements ActionListener
    {
      public void actionPerformed(ActionEvent evt)
      {
        if(utente == null)
        {
          JOptionPane.showMessageDialog(VoronoiGame.this,"Please register first at " +
              "Electronic Registration in the Help menu!","About user's registration", 
              JOptionPane.ERROR_MESSAGE);
          return;
        }



        if(fileFrameSave == null)
        {

          if(cliente == null)
          {
            cliente = new Client(utente, password, nomeFile, email, diagramma);
          }
          else
            cliente.setIstanza(utente, password, nomeFile, email);

          if(listModel == null)
          {
            cliente.dammiListaFiles();
            listModel = new DefaultListModel();                     
            Enumeration datiEnum = (cliente.riceviFiles()).elements();
            while(datiEnum.hasMoreElements())
            {
              listModel.addElement((String)datiEnum.nextElement());
            }
          }

          fileFrameSave = new FileChooserFrame("Save", listModel,cliente, game);
          fileFrameSave.setVisible(true);

        }
        else
        {
          cliente.setNomeFile(fileFrameSave.getNomeFile());                 
          cliente.salvaDati();
        }


      }
    }

    class AscoltaLoadNet implements ActionListener
    {
      public void actionPerformed(ActionEvent evt)
      {
        if(utente == null)
        {
          JOptionPane.showMessageDialog(VoronoiGame.this,"Please register first at " +
              "Electronic Registration in the Help menu!","About user's registration", 
              JOptionPane.ERROR_MESSAGE);
          return;
        }

        if(cliente != null)
          cliente.setIstanza(utente, password, nomeFile, email);
        else
          cliente = new Client(utente, password, nomeFile, email, diagramma);                


        if(fileFrameLoad == null)
        {

          if(listModel == null)
          {
            cliente.dammiListaFiles();
            listModel = new DefaultListModel();
            Enumeration datiEnum = (cliente.riceviFiles()).elements();
            while(datiEnum.hasMoreElements())
            {
              listModel.addElement((String)datiEnum.nextElement());
            }

          }

          fileFrameLoad = new FileChooserFrame("Load", listModel, cliente, game);
          fileFrameLoad.setVisible(true);

        }
        else
        {
          fileFrameLoad.setVisible(true);
        }


        //cliente.caricaDati();
        //game.repaint();
        //repaint();
        //validate();


      }
    }


    class AscoltaImageSave implements ActionListener
    {
      public AscoltaImageSave(String BN_C)
      {
        BNC = BN_C;
      }

      public void actionPerformed(ActionEvent evt)
      {
        //chooser.setCurrentDirectory(new File("../Games"));
        chooser.setSelectedFile(new File("Image.jpg"));

        // show file chooser dialog
        int result = chooser.showSaveDialog(VoronoiGame.this);
        if(result == JFileChooser.APPROVE_OPTION)
        {
          BufferedImage image;
          fileImageSave = chooser.getSelectedFile();
          try
          {
            if(BNC.equals("C"))
            {
              image = game.getJPEG(BufferedImage.TYPE_INT_RGB);
            }
            else
            {
              image = game.getJPEG(BufferedImage.TYPE_BYTE_GRAY);
            }

            ImageIO.write(image, "JPEG", fileImageSave); 

          }
          catch(IOException eccezione)
          {
            System.exit(-1);
          }
        }
      }
      private String BNC;
    }

    class AscoltaImageSaveNet implements ActionListener
    {
      private String BNC;
      public AscoltaImageSaveNet(String BN_C)
      {
        BNC = BN_C;
      }
      public void actionPerformed(ActionEvent evt)
      {
        if(utente == null)
        {
          JOptionPane.showMessageDialog(VoronoiGame.this,"Please register first at " +
              "Electronic Registration in the Help menu!","About user's registration", 
              JOptionPane.ERROR_MESSAGE);
          return;
        }
        if(cliente == null)
          cliente = new Client(utente, password, nomeFile, email, diagramma);
        else
          cliente.setIstanza(utente, password, nomeFile, email);
        BufferedImage image;
        if(BNC.equals("C"))
        {
          image = game.getJPEG(BufferedImage.TYPE_INT_RGB);
        }
        else
        {
          image = game.getJPEG(BufferedImage.TYPE_BYTE_GRAY);
        }
        cliente.salvaImmagine(image);
        cliente.salvaImmagine(image);
        cliente.spedisciMail();

      }
    }



    class AscoltaFileLoad implements ActionListener
    {
      public void actionPerformed(ActionEvent evt)
      {

        int result = chooser.showOpenDialog(VoronoiGame.this);
        if(result == JFileChooser.APPROVE_OPTION)
        {
          File unFile = chooser.getSelectedFile();
          try
          {
            FileReader file = new FileReader(unFile);
            diagramma.caricaDati(file);
          }
          catch(IOException eccezione)
          {
            System.exit(-1);                         
          }
          game.revalidate();
          game.repaint(); 
          repaint();

        }
      }
    }

    class AscoltaResize implements ActionListener
    {
      public void actionPerformed(ActionEvent evento)
      {
        if(dim_X <= dim_Y)
          game.setZoom(Constants.WIDTH_OF_SQUARE / (double)dim_X);
        else
          game.setZoom(Constants.WIDTH_OF_SQUARE / (double)dim_Y);

        /*
           if(gridFrame == null)
           {
           gridFrame = new GridChooserFrame(game);
           gridFrame.getGridFlag();

           }
           */
        if(gridFrame != null)
        {
          gridFrame.getGridFlag();
        }

        game.repaint();
        game.revalidate();
        repaint();
        /*
           try
           {
           URL u = new URL(getCodeBase(), "gamecourse.htm");
           getAppletContext().showDocument(u);
           }
           catch(MalformedURLException o){};       
           */




      }
    }
    class FilePreviewer extends JComponent implements PropertyChangeListener 
    {
      ImageIcon thumbnail = null;
      public FilePreviewer(JFileChooser fc)
      {
        setPreferredSize(new Dimension(100, 50));
        fc.addPropertyChangeListener(this);
        setBorder(new BevelBorder(BevelBorder.LOWERED));
      }

      public void loadImage(File f) 
      {
        if (f == null) 
        {
          thumbnail = null;
        }
        else 
        {
          ImageIcon tmpIcon = new ImageIcon(f.getPath());
          if(tmpIcon.getIconWidth() > 90) 
          {
            thumbnail = new ImageIcon
              (
               tmpIcon.getImage().getScaledInstance(90, -1, Image.SCALE_DEFAULT));
          } 
          else 
          {
            thumbnail = tmpIcon;
          }
        }
      }

      public void propertyChange(PropertyChangeEvent e) 
      {
        String prop = e.getPropertyName();
        if(prop == JFileChooser.SELECTED_FILE_CHANGED_PROPERTY) 
        {
          if(isShowing()) 
          {
            loadImage((File) e.getNewValue());
            repaint();
          }
        }
      }

      public void paint(Graphics g) 
      {
        super.paint(g);
        if(thumbnail != null) 
        {
          int x = getWidth()/2 - thumbnail.getIconWidth()/2;
          int y = getHeight()/2 - thumbnail.getIconHeight()/2;
          if(y < 0) 
          {
            y = 0;
          }

          if(x < 5) 
          {
            x = 5;
          }
          thumbnail.paintIcon(this, g, x, y);
        }
      }
    }



    if(isApplication)
    {
      chooser = new JFileChooser();
      ExtensionFileFilter filter = new ExtensionFileFilter();
      filter.addExtension("txt");
      filter.setDescription("VoronoiGame Files(*.txt)");
      ExtensionFileFilter filt = new ExtensionFileFilter();
      filt.addExtension("jpg");
      filt.setDescription("Picture Files(*.jpg)");
      chooser.setFileFilter(filter);
      chooser.setFileFilter(filt);
      chooser.setAccessory(new FilePreviewer(chooser));
    }



    JMenuBar menuBar = new JMenuBar();
    menuBar.setBackground(colore);
    setJMenuBar(menuBar);
    JMenu fileMenu = new JMenu("File");
    fileMenu.setBackground(colore);
    fileMenu.setForeground(coloreT);
    fileMenu.setMnemonic('F');
    menuBar.add(fileMenu);
    JMenuItem salva = new JMenuItem("Save Game", 'S');
    salva.setBackground(colore);
    salva.setForeground(coloreT);
    salva.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_S, InputEvent.CTRL_DOWN_MASK));
    fileMenu.add(salva);
    if(isApplication)
      salva.addActionListener(new AscoltaSaveItem());
    else
      salva.addActionListener(new AscoltaSaveNet());

    JMenuItem salvaAs = new JMenuItem("Save Game As", 'A');
    salvaAs.setBackground(colore);
    salvaAs.setForeground(coloreT);
    //salvaAs.setMnemonic(12);
    salvaAs.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_A, InputEvent.CTRL_DOWN_MASK));
    fileMenu.add(salvaAs);
    if(isApplication)
      salvaAs.addActionListener(new AscoltaFileSave());
    else
      salvaAs.addActionListener(new AscoltaSaveAsNet());

    progressMonitor.setProgress(10);
    progressMonitor.setNote("Creating File menu...");
    JMenu salvaImageAs = new JMenu("Save Image");
    salvaImageAs.setBackground(colore);
    salvaImageAs.setForeground(coloreT);
    salvaImageAs.setMnemonic('I');
    fileMenu.add(salvaImageAs);
    ButtonGroup gruppo = new ButtonGroup(); 
    JMenuItem salvaBN = new JRadioButtonMenuItem("Save image in Gray scale");        
    salvaBN.setBackground(colore);
    salvaBN.setForeground(coloreT);        


    //salvaBN.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_I + KeyEvent.VK_G , InputEvent.CTRL_DOWN_MASK));

    JMenuItem salvaC = new JRadioButtonMenuItem("Save image in RGB scale");
    salvaC.setBackground(colore);
    salvaC.setForeground(coloreT);
    //salvaC.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_ + KeyEvent.VK_G , InputEvent.CTRL_DOWN_MASK));


    gruppo.add(salvaC);
    gruppo.add(salvaBN);
    salvaImageAs.add(salvaC);
    salvaImageAs.add(salvaBN);
    if(isApplication)
    {
      salvaC.addActionListener(new AscoltaImageSave("C"));
      salvaBN.addActionListener(new AscoltaImageSave("BN"));
    }
    else
    {
      salvaC.addActionListener(new AscoltaImageSaveNet("C"));
      salvaBN.addActionListener(new AscoltaImageSaveNet("BN"));
    }


    JMenuItem stampaImg = new JMenuItem("Print Image", 'P');
    stampaImg.setBackground(colore);
    stampaImg.setForeground(coloreT);
    stampaImg.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_P, InputEvent.CTRL_DOWN_MASK));
    attributes = new HashPrintRequestAttributeSet();
    if(isApplication)
    {
      stampaImg.addActionListener(new
          ActionListener()
          {
            public void actionPerformed(ActionEvent event)
      {
        try
      {
        PrinterJob job = PrinterJob.getPrinterJob();
        job.setPrintable(game);
        if (job.printDialog(attributes))
      {  
        job.print(attributes);
      }
      }
      catch (PrinterException exception)
      {
        JOptionPane.showMessageDialog(VoronoiGame.this, exception);
      }
      }
      });
      fileMenu.add(stampaImg);
    }



    JMenuItem exitMenuItem = new JMenuItem("Exit Game", 'E');
    exitMenuItem.setBackground(colore);
    exitMenuItem.setForeground(coloreT);
    exitMenuItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_E, InputEvent.CTRL_DOWN_MASK));
    if(isApplication)
    {
      exitMenuItem.addActionListener(new
          ActionListener()
          {
            public void actionPerformed(ActionEvent event)
      {
        System.exit(0);
      }});
    }

    JMenuItem carica = new JMenuItem("Load Game", 'L');
    carica.setBackground(colore);
    carica.setForeground(coloreT);
    carica.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_O, InputEvent.CTRL_DOWN_MASK));
    fileMenu.add(carica);
    if(isApplication)
      carica.addActionListener(new AscoltaFileLoad());
    else
      carica.addActionListener(new AscoltaLoadNet());

    JMenuItem cancella = new JMenuItem("Reset Game", 'R');
    cancella.setBackground(colore);
    cancella.setForeground(coloreT);
    cancella.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_N, InputEvent.CTRL_DOWN_MASK));
    fileMenu.add(cancella);
    if(isApplication)
      fileMenu.add(exitMenuItem);

    progressMonitor.setProgress(20);
    progressMonitor.setNote("Creating Edit menu...");
    JMenu editMenu = new JMenu("Edit");
    editMenu.setBackground(colore);
    editMenu.setForeground(coloreT);
    editMenu.setMnemonic('E');
    menuBar.add(editMenu);
    JMenuItem zoomEditItem = new JMenuItem("Zoom", 'Z');
    zoomEditItem.setBackground(colore);
    zoomEditItem.setForeground(coloreT);
    editMenu.add(zoomEditItem);
    JMenuItem resizeItem = new JMenuItem("Resize", 'R');
    resizeItem.setBackground(colore);
    resizeItem.setForeground(coloreT);
    editMenu.add(resizeItem);

    ButtonGroup gruppoLook = new ButtonGroup();
    JRadioButtonMenuItem lookJ = new JRadioButtonMenuItem("Java Look & Feel");
    lookJ.setBackground(colore);
    lookJ.setForeground(coloreT);
    lookJ.addActionListener(new ActionListener()
        {
          public void actionPerformed(ActionEvent ett)
    {
      try 
    {
      UIManager.setLookAndFeel("javax.swing.plaf.metal.MetalLookAndFeel");
      SwingUtilities.updateComponentTreeUI(VoronoiGame.this);

    }
    catch (Exception e) { }
    }
    });

    JRadioButtonMenuItem lookXP = new JRadioButtonMenuItem("Windows Look & Feel");
    lookXP.setBackground(colore);
    lookXP.setForeground(coloreT);

    lookXP.addActionListener(new ActionListener()
        {
          public void actionPerformed(ActionEvent ett)
    {
      try 
    {
      UIManager.setLookAndFeel("com.sun.java.swing.plaf.windows.WindowsLookAndFeel");
      SwingUtilities.updateComponentTreeUI(VoronoiGame.this);
    }
    // catch (ClassNotFoundException e) { }
    //catch (InstantiationException e) { }
    //catch (IllegalAccessException e) { }
    //catch (UnsupportedLookAndFeelException e) { }
    catch (Exception e) { }

    }
    });
    /*
       JRadioButtonMenuItem lookLinux = new JRadioButtonMenuItem("Linux Look & Feel");
       lookLinux.setBackground(colore);
       lookLinux.addActionListener(new ActionListener()
       {
       public void actionPerformed(ActionEvent ett)
       {
       try 
       {
       UIManager.setLookAndFeel("com.sun.java.swing.plaf.motif.MotifLookAndFeel");
       SwingUtilities.updateComponentTreeUI(VoronoiGame.this);                   
       }
       catch (Exception e) { }

       }
       });
       */

    JRadioButtonMenuItem lookPlat = new JRadioButtonMenuItem("Platform Look & Feel");
    lookPlat.setBackground(colore);
    lookPlat.setForeground(coloreT);
    lookPlat.addActionListener(new ActionListener()
        {
          public void actionPerformed(ActionEvent ett)
    {
      try 
    {
      UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
      SwingUtilities.updateComponentTreeUI(VoronoiGame.this);                    
    }
    catch (Exception e) { }

    }
    });



    gruppoLook.add(lookJ);
    gruppoLook.add(lookXP);
    //gruppoLook.add(lookLinux);
    gruppoLook.add(lookPlat);

    JMenu changeLook = new JMenu("Look & Feel");
    changeLook.setBackground(colore);
    changeLook.setForeground(coloreT);
    changeLook.setMnemonic('L');

    changeLook.add(lookJ);
    changeLook.add(lookXP);
    //changeLook.add(lookLinux);
    changeLook.add(lookPlat);
    editMenu.add(changeLook);

    progressMonitor.setProgress(30);
    progressMonitor.setNote("Creating Options menu...");
    JMenu optionMenu = new JMenu("Options");
    optionMenu.setBackground(colore);
    optionMenu.setForeground(coloreT);
    optionMenu.setMnemonic('O');
    menuBar.add(optionMenu);
    progressMonitor.setProgress(40);
    JMenuItem scegliDimensione = new JMenuItem("Set game's form factor", 'S');
    scegliDimensione.setBackground(colore);
    scegliDimensione.setForeground(coloreT);
    optionMenu.add(scegliDimensione);

    showPositionItem = new JCheckBoxMenuItem("Show exact sites position");
    showPositionItem.setBackground(colore);
    showPositionItem.setForeground(coloreT);
    showPositionItem.setSelected(false);
    showPositionItem.addActionListener(new ActionListener()
        {
          public void actionPerformed(ActionEvent evt)
    {
      if(showPositionItem.isSelected())
    {
      game.setShowPosition(true);
    }
    else {game.setShowPosition(false);}

    //game.revalidate();
    game.repaint();
    repaint();
    }
    });


    visualizzaAreaCelle = new JCheckBoxMenuItem("Show single cell area");
    visualizzaAreaCelle.setBackground(colore);
    visualizzaAreaCelle.setForeground(coloreT);
    visualizzaAreaCelle.setSelected(false);
    visualizzaAreaCelle.addActionListener(new ActionListener()
        {
          public void actionPerformed(ActionEvent evt)
    {
      if(visualizzaAreaCelle.isSelected())
    {
      game.setShowArea(true);
    }
    else {game.setShowArea(false);}

    //game.revalidate();
    game.repaint();
    repaint();
    }
    });



    JMenuItem puntiItem = new JMenuItem("Choose point dimensions", 'C');
    puntiItem.setBackground(colore);
    puntiItem.setForeground(coloreT);
    optionMenu.add(puntiItem);

    final JCheckBoxMenuItem antiAliasingItem = new  JCheckBoxMenuItem("Set Antialiasing");
    antiAliasingItem.setBackground(colore);
    antiAliasingItem.setForeground(coloreT);
    antiAliasingItem.setSelected(true);

    oneRoundItem = new  JCheckBoxMenuItem("One-Round Modality");
    oneRoundItem.setBackground(colore);
    oneRoundItem.setForeground(coloreT);

    oneRoundListener = new  ActionListener()
    {
      public void actionPerformed(ActionEvent event)
      {
        if(token == 0) 
        {
          oneRoundItem.setSelected(false); 
          return;
        }

        if(!bOneRound)
        {
          bOneRound = true;
          bColore = blu.isSelected(); 
          oneRoundItem.setSelected(true); 
          item7.setForeground(Color.red);
          item7.setText("OneRound On");
          LABEL_ONEROUND.setVisible(true);
        }
        else
        {
          bOneRound = false;
          bColore = !bColore;
          oneRoundItem.setSelected(false); 
          item7.setForeground(Color.black);
          item7.setText("OneRound Off");
          LABEL_ONEROUND.setVisible(false);
        }
      }
    };


    oneRoundItem.addActionListener(oneRoundListener);

    antiAliasingItem.addActionListener(new ActionListener()
        {
          public void actionPerformed(ActionEvent evt)
    {
      if(antiAliasingItem.isSelected())
    {
      game.setAliasing(true);
      game.repaint();
      repaint();
    }
    else 
    {
      game.setAliasing(false);
      game.repaint();
      repaint();
    }
    }
    });

    optionMenu.add(oneRoundItem);
    optionMenu.add(antiAliasingItem);       
    optionMenu.add(showPositionItem);
    optionMenu.add(visualizzaAreaCelle);


    frameCursore = new JFrame("Drawing points...");
    frameCursore.setSize(260, 170);
    frameCursore.setBackground(colore);
    frameCursore.setResizable(false);
    JPanel panelCursore = new JPanel();
    panelCursore.setBackground(colore);
    JSlider slider = new JSlider(1, 10, 5);
    slider.setBackground(colore);
    slider.setMinorTickSpacing(1);
    slider.setMajorTickSpacing(1);
    slider.setPaintTicks(true); 
    slider.setPaintLabels(true);
    slider.setSnapToTicks(true);
    slider.putClientProperty("JSlider.isFilled", Boolean.TRUE);
    JLabel labelCursore = new JLabel("Please choose point drawing dimensions:");
    labelCursore.setForeground(coloreT);
    labelCursore.setFont(new Font("Serif", Font.BOLD, 14));

    JButton cursorButton = new JButton(new ImageIcon(VoronoiGame.class.getResource("tick.gif")));
    cursorButton.setBackground(colore);
    cursorButton.addActionListener(new ActionListener()
        {
          public void actionPerformed(ActionEvent evt)
    {
      frameCursore.setVisible(false);
    }
    });

    panelCursore.add(labelCursore, BorderLayout.NORTH);
    panelCursore.add(slider, BorderLayout.CENTER);
    panelCursore.add(cursorButton, BorderLayout.SOUTH);
    frameCursore.getContentPane().add(panelCursore, BorderLayout.CENTER);
    slider.addChangeListener(new 
        ChangeListener()
        {
          public void stateChanged(ChangeEvent event)
    {
      JSlider source = (JSlider)event.getSource();
      game.setDiametro(source.getValue());
      diam =  source.getValue() * 2;

      ArrayList listaCelle = diagramma.getVCelle();
      int lunghezzaLista = listaCelle.size();
      Cella ultimaCella = (Cella)listaCelle.get(lunghezzaLista - 1);

      retta = new Rectangle2D.Double(ultimaCella.getKernel().getX() * zoom - diam/2,
        ultimaCella.getKernel().getY() * zoom - diam/2, diam, diam);
      game.setRettangolo(retta); 

      game.repaint();
      repaint();
    }
    });

    puntiItem.addActionListener(new 
        ActionListener()
        {
          public void actionPerformed(ActionEvent eve)
    {
      frameCursore.setVisible(true);
    }
    });

    //Il panello per l'impostazione della dimensione
    dimPanel = new JPanel();        
    dimPanel.setLayout(new GridLayout(3, 1));
    JLabel xDimLabel = new JLabel("Horizontal dimension = ");        
    //xDimLabel.setForeground(coloreT);        
    JLabel yDimLabel = new JLabel("Vertical dimension     = ");        
    //yDimLabel.setForeground(coloreT);  
    dimPanel.add(xDimLabel);
    horizontalCombo = new JComboBox();
    horizontalCombo.setEditable(true);
    horizontalCombo.addItem(new Integer(100)); horizontalCombo.addItem(new Integer(200));horizontalCombo.addItem(new Integer(300));
    horizontalCombo.addItem(new Integer(400));horizontalCombo.addItem(new Integer(500));horizontalCombo.addItem(new Integer(Constants.WIDTH_OF_SQUARE));
    horizontalCombo.addItem(new Integer(700));horizontalCombo.addItem(new Integer(800));horizontalCombo.addItem(new Integer(900));
    horizontalCombo.addItem(new Integer(1000));
    dimPanel.add(horizontalCombo);
    verticalCombo = new JComboBox();
    verticalCombo.setEditable(true);
    verticalCombo.addItem(new Integer(100)); verticalCombo.addItem(new Integer(200));verticalCombo.addItem(new Integer(300));
    verticalCombo.addItem(new Integer(400));verticalCombo.addItem(new Integer(500));verticalCombo.addItem(new Integer(Constants.WIDTH_OF_SQUARE));
    verticalCombo.addItem(new Integer(700));verticalCombo.addItem(new Integer(800));verticalCombo.addItem(new Integer(900));
    verticalCombo.addItem(new Integer(1000));
    dimPanel.add(yDimLabel);
    dimPanel.add(verticalCombo);
    scegliDimensione.addActionListener(new ActionListener()
        {
          public void actionPerformed(ActionEvent evt)
    {

      if(JOptionPane.showConfirmDialog(VoronoiGame.this,dimPanel, "Insert the game dimensions",JOptionPane.YES_NO_OPTION )== 0)
    {
      try
    {
      dim_XTemp = dim_X;
      dim_YTemp = dim_Y;

      dim_X =  ((Integer)horizontalCombo.getSelectedItem()).intValue();
      dim_Y =  ((Integer)verticalCombo.getSelectedItem()).intValue();
      checkNuovo();
      if(gridFrame != null)
    {
      gridFrame.getGridFlag();
    }
    }
    catch(NumberFormatException oo)
    {
      /*
         JOptionPane.showMessageDialog(dimPanel,"Only numbers are allowed!"+ System.getProperty("line.separator") +
         "There was a problem with the input values."
         + "please control that other symbols are not presented.",
         "About redimensioning Voronoi game.", 
         JOptionPane.WARNING_MESSAGE);
         */
    }
    }


    }
    });
    progressMonitor.setProgress(50);
    progressMonitor.setNote("Creating Grid menus...");
    JMenu gridMenu = new JMenu("Grid");
    gridMenu.setBackground(colore);
    gridMenu.setForeground(coloreT);
    gridMenu.setMnemonic('G');
    menuBar.add(gridMenu);
    JMenuItem qualsiasi = new JMenuItem("Choose grid type", 'C');
    qualsiasi.setBackground(colore);
    qualsiasi.setForeground(coloreT);
    gridMenu.add(qualsiasi);
    qualsiasi.addActionListener(new 
        ActionListener()
        {
          public void actionPerformed(ActionEvent eve)
    {
      if(gridFrame == null)
    {
      gridFrame = new GridChooserFrame(game);
      gridFrame.setImage(gridOkIcon, gridExitIcon);
      gridFrame.setLocationRelativeTo(destro);
    }

    gridFrame.setVisible(true);
    //game.revalidate();
    game.repaint(); 
    repaint();

    }
    });


    roundGriglia = new JCheckBoxMenuItem("Snap to grid");
    roundGriglia.setBackground(colore);
    roundGriglia.setForeground(coloreT);
    gridMenu.add(roundGriglia);
    progressMonitor.setProgress(60);
    progressMonitor.setNote("Creating Help menu...");
    JMenu helpMenu = new JMenu("Help");
    helpMenu.setBackground(colore);
    helpMenu.setForeground(coloreT);
    helpMenu.setMnemonic('H');
    menuBar.add(helpMenu);

    JMenuItem aiuto =  new JMenuItem("Contents",'C');
    aiuto.setBackground(colore);
    aiuto.setForeground(coloreT);
    aiuto.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_H, InputEvent.CTRL_DOWN_MASK));
    helpMenu.add(aiuto);
    htmlFrame = new JFrame();
    htmlFrame.setTitle("Game Functionalities");
    //htmlFrame.setLocation(Constants.WIDTH_OF_SQUARE, 100);
    htmlFrame.setSize(370, Constants.WIDTH_OF_SQUARE);
    JEditorPane content = new JEditorPane();
    content.setEditable(false);
    URL urlHtml = VoronoiGame.class.getResource("Contents.html");
    try
    {
      content.setPage(urlHtml);
    }
    catch(IOException e){}
    htmlFrame.getContentPane().add(new JScrollPane(content));
    aiuto.addActionListener(
        new ActionListener()
        {
          public void actionPerformed(ActionEvent evv)
    {
      htmlFrame.setVisible(true); 

    }
    });


    final JMenuItem registrazione = new JMenuItem("Electronic Registration", 'E');
    registrazione.setBackground(colore);
    registrazione.setForeground(coloreT);
    registrazione.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_E, InputEvent.CTRL_DOWN_MASK));
    if(!isApplication)
      helpMenu.add(registrazione);

    frameRegistration = new JFrame("Registration window");
    frameRegistration.setSize(430, 200);
    frameRegistration.setResizable(false);
    frameRegistration.setBackground(colore);
    JPanel panelRegistration = new JPanel(); 
    panelRegistration.setBackground(colore);
    JLabel utenteLabel = new JLabel("Account ID", SwingConstants.CENTER);
    utenteLabel.setForeground(coloreT);
    areaUtente = new JTextField("your_id", 10);        
    JLabel passLabel = new JLabel("Password", SwingConstants.CENTER);
    passLabel.setForeground(coloreT);
    areaPassword = new JPasswordField("pass", 10);        

    JLabel mailLabel = new JLabel("Your email:");
    mailLabel.setForeground(coloreT);
    areaMail = new JTextField("indritselimilavoro@yahoo.it", 14);
    panelRegistration.add(utenteLabel);
    panelRegistration.add(areaUtente);
    panelRegistration.add(passLabel);
    panelRegistration.add(areaPassword);


    panelRegistration.add(mailLabel);
    panelRegistration.add(areaMail);
    progressMonitor.setProgress(70);

    ImageIcon regButtonIcon = new ImageIcon(VoronoiGame.class.getResource("web.gif"));
    JButton buttonRegistrazione = new JButton("Submit", regButtonIcon);
    buttonRegistrazione.setMargin(new Insets(0, 0, 0, 0));
    buttonRegistrazione.setBackground(colore);
    buttonRegistrazione.setForeground(coloreT);
    buttonRegistrazione.addActionListener(
        new ActionListener()
        {
          public void actionPerformed(ActionEvent evt)
    {
      utente = areaUtente.getText();
      password = areaPassword.getPassword();                
      email = areaMail.getText();
      frameRegistration.setVisible(false);
      registrazione.setEnabled(false);

    }
    });
    panelRegistration.add(buttonRegistrazione);
    frameRegistration.getContentPane().add(panelRegistration);
    registrazione.addActionListener(new ActionListener()
        {
          public void actionPerformed(ActionEvent evt)
    {
      frameRegistration.setVisible(true);
    }
    });
    frameRegistration.pack();
    frameRegistration.setLocation(10, 200);
    if(isApplication)
    {
      JMenuItem licenza = new JMenuItem("License", 'L');
      licenza.setBackground(colore);
      licenza.setForeground(coloreT);
      licenza.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_L, InputEvent.CTRL_DOWN_MASK));
      licenzaFrame = new JFrame();
      licenzaFrame.setTitle("License");
      licenzaFrame.setSize(370, Constants.WIDTH_OF_SQUARE);
      JEditorPane gpl = new JEditorPane();
      gpl.setEditable(false);
      URL urlGpl = VoronoiGame.class.getResource("gpl.txt");
      //htmlFrame.setLocation(Constants.WIDTH_OF_SQUARE, 100);
      try
      {
        gpl.setPage(urlGpl);
      }
      catch(IOException e){}
      licenzaFrame.getContentPane().add(new JScrollPane(gpl));
      licenza.addActionListener(new
          ActionListener()
          {
            public void actionPerformed(ActionEvent event)
      {
        licenzaFrame.setVisible(true);
      }
      });
      helpMenu.add(licenza);
    }

    JMenuItem about = new JMenuItem("About", 'A');
    about.setBackground(colore);
    about.setForeground(coloreT);
    about.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_A, InputEvent.CTRL_DOWN_MASK));
    helpMenu.add(about);
    frame = new JFrame();

    about.addActionListener(new
        ActionListener()
        {
          public void actionPerformed(ActionEvent event)
    {
      if (dialog == null) 
      dialog = new About(frame);
    dialog.setVisible(true); 
    }
    });

    ascissaField = new JTextField("300", 5);
    ordinataField = new JTextField("300", 5);
    ascissaField.setFont(new Font("Dialog", Font.BOLD + Font.ITALIC, 16));
    ordinataField.setFont(new Font("Dialog", Font.BOLD + Font.ITALIC, 16));

    JLabel xLabel = new JLabel("x = ", SwingConstants.RIGHT);
    JLabel yLabel = new JLabel("y = ", SwingConstants.RIGHT);
    xLabel.setForeground(coloreT);
    yLabel.setForeground(coloreT);
    InputVerifier verificatore = new VerificatoreTesto();
    ascissaField.setInputVerifier(verificatore);
    ordinataField.setInputVerifier(verificatore);
    JButton randomButton = new JButton("Rand");        
    randomButton.setBackground(colore);
    randomButton.setForeground(coloreT);
    randomButton.setToolTipText("Insert 16 points at random");
    diagramma = new Diagramma(dim_X, dim_Y);
    game = new Pannello(diagramma);
    // Set initial zoom level
    game.setZoom(game.getZoom() * Constants.INIT_GAME_ZOOM_LEVEL);


    scroll = new JScrollPane(game);
    getContentPane().add(scroll, BorderLayout.CENTER);

    progressMonitor.setProgress(80);
    progressMonitor.setNote("Creating tool bar...");
    // Questo panello serve solo per posizionare i bottoni
    bar = new JToolBar("Basics tools are here...");
    bar.setPreferredSize(new Dimension(684, 28));
    //bar.setLayout(new BorderLayout());
    //bar.setLayout(new GridLayout(1,10));
    bar.setBackground(colore);
    Insets n = new Insets(0, -2, 0, 0);
    Insets m = new Insets(0, 0, 0, 0);
    //Border bordoLinea = BorderFactory.createLineBorder(Color.blue);
    //LineBorder bordoLinea = new LineBorder(Color.blue, 1, true);
    //bar.setBorder(bordoLinea);
    clearButton.setMargin(n);
    clearButton.setToolTipText("Push to clear all data structures");
    bar.add(clearButton);

    JButton loadButton = new JButton(new ImageIcon(VoronoiGame.class.getResource("Open24.gif")));
    loadButton.setToolTipText("Push to load a game previosly saved.");
    if(isApplication)
      loadButton.addActionListener(new AscoltaFileLoad());
    else
      loadButton.addActionListener(new AscoltaLoadNet());
    loadButton.setBackground(colore);
    salvaButton.setToolTipText("Push to save the current game.");
    salvaButton.setMargin(m);
    bar.add(salvaButton);
    bar.add(loadButton);
    if(isApplication)
      salvaButton.addActionListener(new AscoltaSaveItem());
    else
      salvaButton.addActionListener(new AscoltaSaveNet());

    ImageIcon ZI = new ImageIcon(VoronoiGame.class.getResource("ZoomIn24.gif"));
    bar.add(creaPulsante(ZI,"Push to zoom in", 1.125));

    ImageIcon ZO = new ImageIcon(VoronoiGame.class.getResource("ZoomOut24.gif"));
    bar.add(creaPulsante(ZO,"Push to zoom out", 0.8));
    bar.add(randomButton);
    bar.addSeparator(new Dimension(10, 0));
    ///ButtonGroup gruppoOneRound = new ButtonGroup(); 
    final ImageIcon redIcon = new ImageIcon(VoronoiGame.class.getResource("red.gif"));
    bluIcon = new ImageIcon(VoronoiGame.class.getResource("blu.gif"));
    blu = new JRadioButton(bluIcon);
    blu.setBackground(colore);
    blu.setSelected(false);
    blu.setToolTipText("Select color for One-Round");        
    blu.addActionListener(new  ActionListener()
        {
          public void actionPerformed(ActionEvent event)
    {
      if(blu.isSelected())
    {
      if(bOneRound)
      bColore = true;
    blu.setIcon(redIcon);
    }
    else
    {
      if(bOneRound)
      bColore = false;
    blu.setIcon(bluIcon);                    
    }
    }
    });

    spinner = new JSpinner(new SpinnerNumberModel(4, 0, 10000, 1) );
    spinner.setForeground(coloreT);
    spinner.setFont(new Font("SansSerif", Font.BOLD, 10));
    spinner.addChangeListener(
        new ChangeListener()
        {
          public void stateChanged(ChangeEvent event)
    {
      token = ((Integer)spinner.getValue()).intValue();
    }
    });

    spinner.setMaximumSize(new Dimension(50, 30));
    spinner.setToolTipText("Choose the number of sites for the One-Round");
    bar.add(blu);
    bar.add(spinner);
    bar.addSeparator(new Dimension(10, 0));

    bar.add(xLabel);
    ascissaField.setToolTipText("The x coordinate.");
    ascissaField.setMaximumSize(new Dimension(60,25));
    bar.add(ascissaField);
    bar.addSeparator(new Dimension(10, 0));
    ordinataField.setMaximumSize(new Dimension(60,25));
    ordinataField.setToolTipText("The y coordinate.");
    bar.add(yLabel);
    bar.add(ordinataField);
    testoButton.setToolTipText("Push to insert a new point.");
    bar.add(testoButton);
    cancelButton.setToolTipText("Push to delete the point.");
    bar.add(cancelButton);
    bar.addSeparator(new Dimension(5, 0));
    LABEL_ONEROUND.setFont(new Font("SansSerif", Font.BOLD, 12));
    LABEL_ONEROUND.setForeground(new Color(195, 169, 61));
    bar.add(LABEL_ONEROUND);
    LABEL_ONEROUND.setVisible(false);
    getContentPane().add(bar, BorderLayout.NORTH);

    progressMonitor.setProgress(90);
    progressMonitor.setNote("Creating the Control panel...");

    //Questo secondo panello va posizionato a destra
    destro = new JPanel();
    destro.setLayout(new GridLayout(20,1));
    destro.setBorder(new TitledBorder((new EtchedBorder()), "Control"));
    destro.setBackground(colore);
    destro.setPreferredSize(new Dimension(Constants.RIGHT_CONTROL_PANEL_WIDTH, dim_Y));

    areaRosso = new JTextField(8);
    areaRosso.setEditable(false);

    areaRosso.setToolTipText("Shows red player area");
    areaBlue = new JTextField(8);
    areaBlue.setEditable(false);
    if((sisOperativo.toLowerCase()).startsWith("win"))
    {
      areaRosso.setFont(new Font("SansSerif", Font.BOLD,18));
      areaBlue.setFont(new Font("SansSerif", Font.BOLD,18));
    }
    else
    {
      areaRosso.setFont(new Font("SansSerif",Font.BOLD,16));
      areaBlue.setFont(new Font("SansSerif",Font.BOLD,16));
    }

    areaBlue.setToolTipText("Shows blue player area");
    areaRed = new JLabel("Red's Score");
    areaRed.setForeground(new Color(250, 72, 72));
    areaBlu = new JLabel("Blu's Score");
    areaBlu.setForeground(new Color(31, 75, 153));
    coordinate = new JLabel("(300,300)", SwingConstants.CENTER);
    coordinate.setForeground(coloreT);
    if((sisOperativo.toLowerCase()).startsWith("win"))
      coordinate.setFont(new Font("Serif", Font.BOLD, 16));
    else
      coordinate.setFont(new Font("Serif", Font.BOLD, 14));
    JLabel testoCursore = new JLabel("Cursor in:", SwingConstants.CENTER);
    testoCursore.setForeground(coloreT);
    testoCursore.setFont(new Font("Serif", Font.BOLD, 16));

    destro.add(areaRed);
    destro.add(areaRosso);
    destro.add(areaBlu);
    destro.add(areaBlue);        
    destro.add(testoCursore);
    destro.add(coordinate);


    testoAreaCella = new JLabel("Cell's area:", SwingConstants.CENTER);
    testoAreaCella.setForeground(coloreT);
    testoAreaCella.setFont(new Font("Serif", Font.BOLD, 13));
    testoAreaCella.setVisible(false);
    destro.add(testoAreaCella);
    areaCellaDragged = new JLabel("", SwingConstants.CENTER);
    areaCellaDragged.setForeground(coloreT);
    if((sisOperativo.toLowerCase()).startsWith("win"))
      areaCellaDragged.setFont(new Font("Serif", Font.BOLD, 14));
    else
      areaCellaDragged.setFont(new Font("Serif", Font.BOLD, 12));
    areaCellaDragged.setVisible(false);
    destro.add(areaCellaDragged);




    getContentPane().add(destro, BorderLayout.EAST);

    progressMonitor.setProgress(100);
    progressMonitor.setNote("Creating Pop Up menu...");

    popup = new JPopupMenu();

    ImageIcon dragPopUP = new ImageIcon(VoronoiGame.class.getResource("mouse16.gif"));

    item0 = new JMenuItem("Drag On/Off", dragPopUP);
    item0.setBackground(colore);
    item0.setForeground(coloreT);

    ImageIcon resetPopUP = new ImageIcon(VoronoiGame.class.getResource("New16.gif"));
    item1 = new JMenuItem("Reset", resetPopUP);
    item1.setBackground(colore);
    item1.setForeground(coloreT);

    ImageIcon Plus = new ImageIcon(VoronoiGame.class.getResource("ZoomIn16.gif"));

    item2 = new JMenuItem("ZoomIn On/Off", Plus);
    item2.setBackground(colore);
    item2.setForeground(coloreT);

    ImageIcon Minus = new ImageIcon(VoronoiGame.class.getResource("ZoomOut16.gif"));

    item3 = new JMenuItem("ZoomOut On/Off",  Minus);
    item3.setBackground(colore);
    item3.setForeground(coloreT);

    ImageIcon savePopUP = new ImageIcon(VoronoiGame.class.getResource("Save16.gif"));
    item4 = new JMenuItem("Save Game",savePopUP);
    item4.setBackground(colore);
    item4.setForeground(coloreT);
    if(isApplication)
      item4.addActionListener(new AscoltaSaveItem());
    else
      item4.addActionListener(new AscoltaSaveNet());
    ImageIcon loadPopUP = new ImageIcon(VoronoiGame.class.getResource("Open16.gif"));
    item5 = new JMenuItem("Load Game", loadPopUP);
    item5.setBackground(colore);
    item5.setForeground(coloreT);
    if(isApplication)
      item5.addActionListener(new AscoltaFileLoad());
    else
      item5.addActionListener(new AscoltaLoadNet());

    ImageIcon grigliaPopUP = new ImageIcon(VoronoiGame.class.getResource("griglia.gif"));

    item6 = new JMenuItem("Grid Enable/Disable", grigliaPopUP);
    item6.setBackground(colore);
    item6.setForeground(coloreT);
    item6.addActionListener(new  ActionListener()
        {
          public void actionPerformed(ActionEvent event)
    {
      if(gridFrame == null)
    {
      gridFrame = new GridChooserFrame(game);
      gridFrame.setImage(gridOkIcon, gridExitIcon);
    }

    if(!game.getViewGriglia())
    {
      item6.setForeground(Color.red);
      item6.setText("Grid Enabled");                    
      gridFrame.getGridFlag();                    
      game.setViewGriglia(true);
      gridFrame.setCheck(true);
    }
    else
    {
      game.setViewGriglia(false);
      gridFrame.setCheck(false);
      item6.setForeground(coloreT);
      item6.setText("Grid Disabled");
    }

    game.revalidate();
    game.repaint();
    repaint();
    }
    });

    ImageIcon oneRoundIcon = new ImageIcon(VoronoiGame.class.getResource("bean16.gif"));
    item7 = new JMenuItem("One-Round On/Off", oneRoundIcon);
    item7.setBackground(colore);
    item7.setForeground(coloreT);
    item7.addActionListener(oneRoundListener);

    progressMonitor.setProgress(110);
    progressMonitor.setNote("Installing listeners...");

    CustomMouseListener listenerMouse = new CustomMouseListener();
    game.addMouseListener(listenerMouse);
    game.addMouseMotionListener(listenerMouse);

    ClearListener listenerClear = new ClearListener();
    clearButton.addActionListener(listenerClear);

    RandomListener listenerRandom  = new RandomListener();
    randomButton.addActionListener(listenerRandom);

    TestoListener listenerTesto = new TestoListener();
    testoButton.addActionListener(listenerTesto);

    CancelTestoListener cancelListener = new CancelTestoListener();
    cancelButton.addActionListener(cancelListener);       

    RounderListener ascoltaRounder = new RounderListener();
    roundGriglia.addActionListener(ascoltaRounder);

    AscoltaEditZoom  ascoltaEditZoom = new  AscoltaEditZoom();
    zoomEditItem.addActionListener(ascoltaEditZoom);

    resizeItem.addActionListener(new AscoltaResize());

    item0.addActionListener(new ActionListener()
        {
          public void actionPerformed(ActionEvent evt)
    {
      if(isDragged)
    {
      isDragged = false;
      game.setDragged(false);
      item0.setForeground(coloreT);
      item0.setText("Dragg Off");
      game.repaint();
      repaint();
    }
    else
    {
      ArrayList listaCelle = diagramma.getVCelle();
      if(!listaCelle.isEmpty())
    {
      isDragged = true;
      item0.setForeground(Color.red);
      item0.setText("Dragg On");

      int lunghezzaLista = listaCelle.size();
      Cella ultimaCella = (Cella)listaCelle.get(lunghezzaLista - 1);

      retta = new Rectangle2D.Double(ultimaCella.getKernel().getX() * game.getZoom() - diam/2,
          ultimaCella.getKernel().getY() * game.getZoom() - diam/2, diam, diam);
      game.setRettangolo(retta); 
      game.setDragged(true);
      game.repaint();
      repaint();
    }
    } 
    }
    });
    popup.add(item0);
    item1.addActionListener(listenerClear);
    popup.add(item2);
    item2.addActionListener(new ActionListener()
        {
          public void actionPerformed(ActionEvent evento)
    {
      if(!isZoomIn){isZoomIn = true; isZoomOut = false;item3.setForeground(coloreT);item3.setText("ZoomOut Off");item2.setForeground(Color.red);item2.setText("ZoomIn On");} 
      else{isZoomIn = false; isZoomOut = false;item2.setForeground(coloreT);item2.setText("ZoomIn Off");}
    }
    });
    popup.add(item3); 
    item3.addActionListener(new ActionListener()
        {
          public void actionPerformed(ActionEvent evento)
    {
      if(!isZoomOut){isZoomOut = true; isZoomIn =false;item2.setForeground(coloreT);item2.setText("ZoomIn Off");item3.setForeground(Color.red);item3.setText("ZoomOut On");}
      else{isZoomOut = false; isZoomIn = false;item3.setForeground(coloreT);item3.setText("ZoomOut Off");}
    }
    });

    popup.add(item6);
    popup.add(item7);
    popup.add(item4);
    popup.add(item5);
    popup.add(item1);

    cancella.addActionListener(listenerClear);
    progressMonitor.setProgress(120);
    progressMonitor.setNote("Download completed!");
    progressMonitor.close();

    game.setAliasing(true);
  }

  public void stop()
  {
    if(cliente != null)
    {
      cliente.holdOff();
    }
  }
  /*
     public void destroy()
     {
     if(cliente != null)
     {
     cliente.holdOff();
     }
     }
     */


  public void checkNuovo()
  {
    if((dim_XTemp != dim_X) || (dim_YTemp != dim_Y))
    {

      diagramma.setDiagramSize(dim_X, dim_Y);
      if(dim_X <= dim_Y){game.setZoom(1000 / (double)dim_X);} else{game.setZoom(1000 /  (double)dim_Y);}


      game.revalidate();
      game.repaint();
      repaint();

    }
  }




  private JButton creaPulsante(ImageIcon icon,String testo, final double zoom_factor)
  {
    JButton pulsante = new JButton(icon);
    pulsante.setBackground(colore);
    pulsante.setToolTipText(testo);
    class AscoltaPulsante implements ActionListener
    {
      public void actionPerformed(ActionEvent evento)
      {
        game.setZoom(game.getZoom() * zoom_factor);
        if(gridFrame == null)
        {
          gridFrame = new GridChooserFrame(game);
          gridFrame.setImage(gridOkIcon, gridExitIcon);
          gridFrame.getGridFlag();
        }
        else 
        {
          gridFrame.getGridFlag();
        }



        game.revalidate();
        game.repaint();
        repaint();

      }
    }

    AscoltaPulsante listener = new AscoltaPulsante();
    pulsante.addActionListener(listener);
    return pulsante;
  }

  public void setCampoBlue()
  {
    areaBlue.setText(game.areaTotaleBlue());
  }

  public void setCampoRosso()
  {
    areaRosso.setText(game.areaTotaleRosso());
  }

  /**
   * Questo metodo aggiunge un nuovo sito sul 
   * diagramma in base all'input da tastiera.
   */
  private void setPoint() 
  {
    try
    {

      double coordX
        = Double.parseDouble(ascissaField.getText());
      double coordY
        = Double.parseDouble(ordinataField.getText());

      if(isChecked)
      {
        griglia = game.dammiGriglia();
        double zoom = game.getZoom();
        int k = griglia.getKappa();
        int l = griglia.getElle();
        try
        {
          Cella nuova = new Cella( 
              griglia.roundToGriglia(coordX, Math.round((float)(k/zoom))),  
              griglia.roundToGriglia(coordY, Math.round((float)(l/zoom))));
          aggiungiCella(nuova);

        }
        catch(Exception ecc)
        {
          JOptionPane.showMessageDialog(VoronoiGame.this,"This algorithm is affected from numerical approximation errors. " + System.getProperty("line.separator") + 
              "The game will be now reset. Please be patient... we are working on!",
              "About approximation problems.", 
              JOptionPane.ERROR_MESSAGE);                   
          this.reset();
        }
      }
      else
      {
        try
        {
          Cella nuova = new Cella(coordX, coordY);
          aggiungiCella(nuova);
        }
        catch(Exception ecc)
        {

          JOptionPane.showMessageDialog(VoronoiGame.this,"This algorithm is affected from numerical approximation errors." + System.getProperty("line.separator")+
              "The game now will be reset. Please be patient... we are working on!",
              "About approximation problems.", 
              JOptionPane.ERROR_MESSAGE);

          this.reset();
        }
      }


    }
    catch (NumberFormatException e)
    {

      JOptionPane.showMessageDialog(VoronoiGame.this,"Only numbers are allowed! " + System.getProperty("line.separator")  + 
          "The correct format is for example 434.896." + System.getProperty("line.separator")  + "Please control that other symbols are not presented.",
          "About inserted values from keyboard.", 
          JOptionPane.WARNING_MESSAGE);
    }
  }

  private boolean isValid(String s)
  {
    try
    {
      Double.parseDouble(s);
      return true;
    }
    catch(NumberFormatException exc)
    {
      return false;
    }
  }

  public void setChecked(boolean check)
  {
    isChecked = check;
  }
  /*
     public boolean puntiUguali(Point2D.Double pointUno, Point2D.Double pointDue)
     {
     return (pointUno.distance(pointDue) <= DISTANZA_MIN/2 - 0.1);
     }
     */


  public String calcolaAreaCella(Cella sito)
  {
    Cella sitoCorrente = sito;

    ArrayList confini = sitoCorrente.getVfrontiere();
    ArrayList lista = game.ordinaPunti(confini, sitoCorrente.getKernel());

    DecimalFormat myFormatter = new DecimalFormat("#");
    return myFormatter.format(game.calcolaArea(lista));
  }
  public void reset()
  {
    diagramma.clear();
    setCampoBlue();
    setCampoRosso();                 

    game.revalidate();
    game.repaint();
    repaint();
  }

  public void setDimensX(String unInt)
  {
    dim_X = Integer.parseInt(unInt);
  }

  public void setDimensY(String unInt)
  {
    dim_Y = Integer.parseInt(unInt);
  }

  public void aggiungiCella(Cella c)
  {
    Cella cellaNuova = c;
    cellaNuova.setColore(bColore);
    diagramma.aggiungiCella(cellaNuova);
    if(bOneRound)
    {
      if(token == 1)
      {
        bOneRound = false;
        bColore = !bColore;
        oneRoundItem.setSelected(false); 
        item7.setForeground(Color.black);
        item7.setText("OneRound Off");
        LABEL_ONEROUND.setVisible(false);
      }

      token--;
      spinner.setValue(new Integer(token));
    }
    else
      bColore = !bColore;
  }

  public String getAppletInfo() {
    return "Voronoi Game Judge Applet";
  }

  // Don't call, use humanPutStone instead
  public static void artificialMouseClick(int x, int y, double zoomLevel) {
    try {
      Robot robot = new Robot();
   
    //    coordinate.setText("(" + Math.round((float)(evento.getX()/zoom)) + 
      //      ", " + Math.round((float)(evento.getY() / zoom))+ ")");
  
      double xwhole, ywhole;
      xwhole = x + 8;
      ywhole = y + 151;
      // ywhole = y;
      System.out.println("Zoom level= " + zoomLevel);
      robot.mouseMove((int)(xwhole * zoomLevel), (int)(ywhole * zoomLevel));
      // Simulate a mouse click
      robot.mousePress(InputEvent.BUTTON1_MASK);
      robot.mouseRelease(InputEvent.BUTTON1_MASK);
    } catch (AWTException awte) {
      awte.printStackTrace();
    }
  }

  public static VoronoiGame startGame() {
    isApplication = true;
    VoronoiGame voro = new VoronoiGame();
    frameT = new JFrame();
    frameT.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    frameT.getContentPane().add(voro);
    // Set initial window size
    frameT.setSize(Constants.INIT_WINDOW_WIDTH, Constants.INIT_WINDOW_LENGTH);
    frameT.setTitle("Voronoi Game");
    Toolkit strum = Toolkit.getDefaultToolkit();
    Image imgIc = strum.getImage(VoronoiGame.class.getResource("Unipg.GIF"));
    frameT.setIconImage(imgIc);
    voro.init();
    frameT.setVisible(true);

    // voro.humanPutStone(250, 100, "aaa");
    // voro.humanPutStone(750, 100, "ccc");
    return voro;
  }

  public static void main(String[] args) {
    isApplication = true;
    JApplet voro = new VoronoiGame();
    frameT = new JFrame();
    frameT.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    frameT.getContentPane().add(voro);
    // Set initial window size
    frameT.setSize(Constants.INIT_WINDOW_WIDTH, Constants.INIT_WINDOW_LENGTH);
    frameT.setTitle("Voronoi Game");
    Toolkit strum = Toolkit.getDefaultToolkit();
    Image imgIc = strum.getImage(VoronoiGame.class.getResource("Unipg.GIF"));
    frameT.setIconImage(imgIc);
    voro.init();
    frameT.setVisible(true);
  }

  private int dim_X = Constants.WIDTH_OF_SQUARE; //Qua esiste un problema!!Se non sono final non si
  private int dim_Y = Constants.WIDTH_OF_SQUARE; //inizializza l'applet. Bisogna dare come parametri!


  private static Diagramma diagramma;
  private static Pannello game;
  private static Cella sitoCorrente; //Sperimantale!!


  private boolean isDragged = false;
  private JScrollPane scroll;
  private JPanel destro;

  private JCheckBoxMenuItem roundGriglia;
  private JCheckBoxMenuItem visualizzaAreaCelle;
  private JCheckBoxMenuItem showPositionItem;

  private JLabel areaCellaDragged;
  private JLabel testoAreaCella;
  private JTextField areaRosso;
  private JTextField areaBlue;
  private JLabel coordinate;
  private Random generator;
  private boolean isChecked = false;// Serve per lo Snap to Griglia
  private JTextField ascissaField;
  private JTextField ordinataField;
  private double zoom;
  private static final double DISTANZA_MIN = 2;
  private JPopupMenu popup; // Sperimentale!
  private JMenuItem item0,item1, item2, item3, item4, item5, item6, item7;
  private JFrame frame;
  private JFrame frameCursore;


  private static final  Color colore = new Color(233, 222, 205);
  //private static final  Color coloreT = new Color(105, 42, 42);
  private static final  Color coloreT = new Color(57, 24, 24);
  private Griglia griglia;
  private GridChooserFrame gridFrame;

  private boolean isZoomIn = false;
  private boolean isZoomOut = false;
  private About dialog;
  private JPanel dimPanel;
  private JPanel gridPanel;
  private JTextField xDim;
  private JTextField yDim;

  private int dim_XTemp = 0;
  private int dim_YTemp = 0;

  private JFileChooser chooser;
  private File fileSave;
  private File fileImageSave;
  private FileWriter file;

  private static boolean isApplication = false; // Sperimantale

  private JComboBox horizontalCombo;
  private JComboBox verticalCombo;

  private Client cliente; //Ogetto che serve per referenziare un'istanza Client

  private String utente;
  private char[] password;
  private String email;
  private String nomeFile = "Game.txt";


  private JFrame frameRegistration;
  private JTextField areaUtente;
  private JPasswordField areaPassword;
  private JTextField areaFile;
  private JTextField areaMail;

  private PrintRequestAttributeSet attributes; //Molto Sperimentale!!!
  private boolean nonPrimaVolta = false; //Molto, molto sperimentale!!!

  private int diam = 8; //Serve per aggiornare il rettangolo di dragged

  private JLabel areaRed;
  private JLabel areaBlu;
  private Rectangle2D.Double retta;
  private ImageIcon gridOkIcon, gridExitIcon;
  private JToolBar bar;

  private FileChooserFrame fileFrameSave;
  private FileChooserFrame fileFrameLoad;
  private DefaultListModel listModel;
  private  JFrame htmlFrame;
  private static JFrame frameT;
  private String sisOperativo; //Molto sperimentale
  private JFrame licenzaFrame;
  private boolean bColore = false;
  private int token = 4;
  private JRadioButton blu = null;
  private boolean bOneRound = false; // Sperimentale
  private JSpinner spinner = null;
  private final JLabel LABEL_ONEROUND = new JLabel("One-Round");
  private ActionListener oneRoundListener  = null;
  private JCheckBoxMenuItem oneRoundItem = null;
  private ImageIcon bluIcon = null;
}
