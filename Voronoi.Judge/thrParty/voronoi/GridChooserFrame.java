package voronoi;

import java.awt.*;
import java.awt.event.*;
import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.JLabel;
import javax.swing.*;
import javax.swing.event.*;
import java.io.File;


/**
 * Serve per selezionare la griglia
 * 
 * @author Indrit Selimi 
 * @version 1.0
 */

public class GridChooserFrame extends JFrame {
  /**
   * Costruttore degli oggetti di classe  GridChooserFrame
   */
  public GridChooserFrame(Pannello unGame)
  {

    game = unGame;
    setSize(270, 300);
    setBackground(colore);
    setTitle("Grid Chooser Window");        
    setResizable(false);
    setLocation(690, 0);
    ImageIcon blueBall = new ImageIcon(GridChooserFrame.class.getResource("rb.gif"));
    ImageIcon redBall = new ImageIcon(GridChooserFrame.class.getResource("rbp.gif"));

    gridPanel = new JPanel();
    //gridPanel.setLayout(new GridLayout(8, 2));
    gridPanel.setBackground(colore);
    JLabel xPixelLabel = new JLabel("Horizontal pixel = ", SwingConstants.CENTER);
    xPixelLabel.setBackground(colore);
    xPixelLabel.setForeground(coloreT);
    xPixelLabel.setFont(new Font("Serif", Font.BOLD, 14));
    JLabel yPixelLabel = new JLabel("Vertical pixel = ", SwingConstants.CENTER);
    yPixelLabel.setBackground(colore);
    yPixelLabel.setForeground(coloreT);
    yPixelLabel.setFont(new Font("Serif", Font.BOLD, 14));

    sliderX = new JSlider( 4, 50, 20);

    sliderY = new JSlider(4, 50, 20);

    sliderX.setBackground(colore);
    //sliderX.setForeground(coloreT);
    sliderX.setMinorTickSpacing(4);
    sliderX.setMajorTickSpacing(4);
    sliderX.setPaintTicks(true); 
    sliderX.setPaintLabels(true);
    //sliderX.setSnapToTicks(true);
    sliderX.putClientProperty("JSlider.isFilled", Boolean.TRUE);

    sliderX.addChangeListener(new 
        ChangeListener()
        {  
          public void stateChanged(ChangeEvent event)
    {  
      //JSlider source = (JSlider)event.getSource();
      //pixelX = source.getValue();
      pixelX = sliderX.getValue();
      setGrigliaPixel();

      //verticalCombo.setSelectedItem(new Integer(game.dammiGriglia().getEnne()));
    }
    });


    sliderY.setBackground(colore);
    //sliderY.setForeground(coloreT);
    sliderY.setMinorTickSpacing(4);
    sliderY.setMajorTickSpacing(4);
    sliderY.setPaintTicks(true); 
    sliderY.setPaintLabels(true);
    //sliderY.setSnapToTicks(true);
    sliderY.putClientProperty("JSlider.isFilled", Boolean.TRUE);

    sliderY.addChangeListener(new 
        ChangeListener()
        {  
          public void stateChanged(ChangeEvent event)
    {  
      //JSlider source = (JSlider)event.getSource();
      //pixelY = source.getValue();
      pixelY = sliderY.getValue();
      setGrigliaPixel();
      //horizontalCombo.setSelectedItem(new Integer(game.dammiGriglia().getEmme()));
    }
    });


    gridPanel.add(xPixelLabel);
    gridPanel.add(sliderX);
    gridPanel.add(yPixelLabel);
    gridPanel.add(sliderY);



    JLabel righeLabel = new JLabel("Rows            = ", SwingConstants.LEFT);
    righeLabel.setBackground(colore);
    righeLabel.setForeground(coloreT);
    righeLabel.setFont(new Font("Serif", Font.BOLD, 14));
    JLabel colonneLabel = new JLabel("Columns       = ",SwingConstants.LEFT);
    colonneLabel.setBackground(colore);
    colonneLabel.setForeground(coloreT);
    colonneLabel.setFont(new Font("Serif", Font.BOLD, 14));

    horizontalCombo = new JComboBox();
    horizontalCombo.setPreferredSize(new Dimension(80, 22));
    horizontalCombo.setEditable(true);
    horizontalCombo.setBackground(colore);
    horizontalCombo.setForeground(coloreT);
    horizontalCombo.addItem(new Integer(10)); horizontalCombo.addItem(new Integer(20));horizontalCombo.addItem(new Integer(30));
    horizontalCombo.addItem(new Integer(40));horizontalCombo.addItem(new Integer(50));horizontalCombo.addItem(new Integer(60));
    horizontalCombo.addItem(new Integer(70));horizontalCombo.addItem(new Integer(80));horizontalCombo.addItem(new Integer(90));
    horizontalCombo.addItem(new Integer(100));

    horizontalCombo.addActionListener(new ActionListener()
        {
          public void actionPerformed(ActionEvent evt)
    {
      try
    {

      numRighe =  ((Integer)horizontalCombo.getSelectedItem()).intValue();

      setGrigliaRC();
      //sliderY.setValue(game.dammiGriglia().getElle());

    }
    catch(NumberFormatException oo)
    {
      //Non dimenticare!!!
    }
    }
    });


    verticalCombo = new JComboBox();
    verticalCombo.setEditable(true);
    verticalCombo.setBackground(colore);
    verticalCombo.setForeground(coloreT);
    verticalCombo.setPreferredSize(new Dimension(80, 22));
    verticalCombo.addItem(new Integer(10)); verticalCombo.addItem(new Integer(20));verticalCombo.addItem(new Integer(30));
    verticalCombo.addItem(new Integer(40));verticalCombo.addItem(new Integer(50));verticalCombo.addItem(new Integer(60));
    verticalCombo.addItem(new Integer(70));verticalCombo.addItem(new Integer(80));verticalCombo.addItem(new Integer(90));
    verticalCombo.addItem(new Integer(100));

    verticalCombo.addActionListener(new ActionListener()
        {
          public void actionPerformed(ActionEvent evt)
    {
      try
    {
      numColonne =  ((Integer)verticalCombo.getSelectedItem()).intValue();
      setGrigliaRC();
      //sliderX.setValue(game.dammiGriglia().getKappa());

    }
    catch(NumberFormatException oo)
    {
      //Non dimenticare!!!
    }
    }
    });

    gridPanel.add(righeLabel);
    gridPanel.add(horizontalCombo);
    gridPanel.add(colonneLabel);
    gridPanel.add(verticalCombo);

    grigliaCheck = new JCheckBox("Grid enable/disable", blueBall);
    grigliaCheck.setBackground(colore);
    grigliaCheck.setForeground(coloreT);
    grigliaCheck.setFont(new Font("Serif", Font.BOLD, 13));
    grigliaCheck.setSelectedIcon(redBall);
    class GrigliaListener implements ActionListener
    {
      public void actionPerformed(ActionEvent event)
      {
        if(grigliaCheck.isSelected())
        {
          //grigliaCheck.setIcon(redBall); 
          getGridFlag();
          game.setViewGriglia(true);
        }
        else{game.setViewGriglia(false);}

        game.revalidate();
        game.repaint();
        repaint();
      }
    }

    GrigliaListener ascoltaGriglia = new GrigliaListener();
    grigliaCheck.addActionListener(ascoltaGriglia);
    gridPanel.add(grigliaCheck);

    getContentPane().add(gridPanel, BorderLayout.CENTER);

  }



  public void setGrigliaDefault()
  {
    double zoom = game.getZoom();
    int dimX =  game.getDimensioneX();
    int dimY =  game.getDimensioneY();
    int dimQuadrattino = 20;
    griglia = new Griglia(dimQuadrattino, dimX, dimY, zoom);
    game.setGriglia(griglia);
    defaultFlag = true;
    pixelFlag = false;
    rcFlag = false;
    game.revalidate();
    game.repaint(); 
  }
  public void setGrigliaPixel()
  {
    double zoom = game.getZoom();
    int dimX =  game.getDimensioneX();
    int dimY =  game.getDimensioneY();
    griglia = new Griglia(pixelX, pixelY, dimX, dimY, zoom, "Pixel");
    game.setGriglia(griglia);
    defaultFlag = false;
    pixelFlag = true;
    rcFlag = false;
    game.revalidate();
    game.repaint(); 
  }

  public void setGrigliaRC()
  {
    double zoom = game.getZoom();
    int dimX =  game.getDimensioneX();
    int dimY =  game.getDimensioneY();
    pixelX = Math.round((float)(dimX / numColonne));
    pixelY = Math.round((float)(dimY / numRighe));
    griglia = new Griglia(pixelX, pixelY,  dimX, dimY, zoom, "Pixel");
    game.setGriglia(griglia);
    defaultFlag = false;
    pixelFlag = false;
    rcFlag = true;
    game.revalidate();
    game.repaint(); 
  }

  public void getGridFlag()
  {
    if(pixelFlag)
    {
      setGrigliaPixel();
    }
    else if(rcFlag)
    {
      setGrigliaRC();
    }
    else
    {
      setGrigliaDefault();
    }

  }
  public void setImage(ImageIcon img, ImageIcon img2)
  {
    JButton button = new JButton(img);
    button.setBackground(colore);
    button.setMargin(new Insets(3, 1, 1, 2));
    gridPanel.add(button);

    JButton button2 = new JButton(img2);
    button2.setBackground(colore);
    button2.setMargin(new Insets(2, 1, 1, 1));
    gridPanel.add(button2);
    button2.addActionListener(
        new ActionListener()
        {
          public void actionPerformed(ActionEvent evv)
    {
      setVisible(false);
    }
    });
    this.validate();

  }

  public void setCheck(boolean on)
  {
    if(on)
    {

      grigliaCheck.setSelected(true);
    }                        
    else
    {

      grigliaCheck.setSelected(false);            
    }
  }






  private static final  Color colore = new Color(233, 222, 205);
  private static final  Color coloreT = new Color(57, 24, 24);
  private Griglia griglia;
  private Pannello game;
  private int pixelX = 20;
  private int pixelY = 20;
  private int numRighe = 30;
  private int numColonne = 30;
  private boolean defaultFlag = false;
  private boolean pixelFlag = false;
  private boolean rcFlag = false;
  private JCheckBox grigliaCheck;
  private JSlider sliderX;
  private JSlider sliderY;
  private JComboBox horizontalCombo;
  private JComboBox verticalCombo;

  private JPanel gridPanel;



}
