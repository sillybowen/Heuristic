/**
   @version 1.4
   @author Indrit Selimi
*/

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;


class About extends JDialog
{  
   public About(JFrame owner)
   {  
      super(owner, "Information on Voronoi Game Applet", true);         
      Container contentPane = getContentPane();
      contentPane.setBackground(colore);
      setLocation(100, 150);
      setResizable(false);
      
      contentPane.add(new JLabel(
         "<HTML> "+ 
         "<BODY   text = \"#422C2C\"><BLOCKQUOTE><H2>Voronoi Game Applet</H2> Version 1.1 <BR>"
         + "<HR><strong>By Indrit Selimi<BR>E-mail: <font color = \"#A1FB6\"> indritselimi@hotmail.com</strong></font></BLOCKQUOTE></BODY></HTML>"),
         BorderLayout.CENTER);
      
      JButton ok = new JButton("Ok");
      ok.setBackground(colore);
      ok.addActionListener(new 
         ActionListener() 
         {  
            public void actionPerformed(ActionEvent evt) 
            { 
               setVisible(false); 
            } 
         });

     
      
      JPanel panel = new JPanel();
      panel.setBackground(colore);
      panel.add(ok);
      contentPane.add(panel, BorderLayout.SOUTH);

      setSize(280, 230);
      
   }
   
   public void setImage(ImageIcon img)
   {
       JPanel panello = new JPanel();
       panello.setBackground(colore);
       JLabel imagine = new JLabel(img);
      //panello.add(imagine);    
       
       //panello.add(new JLabel("<HTML><BODY text = \"#583737\"><p>" +
       getContentPane().add(panello, BorderLayout.NORTH);
   }
   
    private static final  Color colore = new Color(233, 222, 205);
       
}

