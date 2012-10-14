package voronoi;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import java.util.*;


public class FileChooserFrame extends JFrame 
{
    
    /**
     * Costruttore degli oggetti di classe  Lista
     */
    public FileChooserFrame(String operazione,DefaultListModel modello, Client unCliente, Pannello unGame)
    {
        listModel = modello;       
        hireString = operazione;
        cliente = unCliente;
        pannello = unGame;       
         
        setTitle("File Manager Window");
        setSize(300, 200);        
        
        list = new JList(listModel);
        list.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        //list.setSelectedIndex(0);        
        list.setVisibleRowCount(5);
        list.setLayoutOrientation(JList.HORIZONTAL_WRAP);
        JScrollPane listScrollPane = new JScrollPane(list);
        list.addListSelectionListener(
            new ListSelectionListener()
            {
                public void valueChanged(ListSelectionEvent ett) 
                { 
                    
                    if (!ett.getValueIsAdjusting()) 
                    {                        
                        try
                        {
                           nome.setText((String)list.getSelectedValue());
                        }
                        catch(ArrayIndexOutOfBoundsException arrayEcc){}
                        
                        if (list.getSelectedIndex() == -1) 
                        {                           
                            cancelButton.setEnabled(false);                           
                        }                         
                        else 
                        {
                            cancelButton.setEnabled(true);
                        }
                    }
                }
            });
         list.addMouseListener(
            new MouseListener()
            {

                public void mouseClicked(MouseEvent e)
                {
                    
                    if(e.getClickCount() == 2)
                    {
                        nomeFile = (String)list.getSelectedValue();
                        cliente.setNomeFile(nomeFile); 
                        if(hireString.equals("Save"))
                            cliente.salvaDati();
                        else
                        {
                            cliente.caricaDati();
                            pannello.repaint();
                        }
                        FileChooserFrame.this.setVisible(false);
                    }
                }
                public void mousePressed(MouseEvent e){}
                public void mouseReleased(MouseEvent e){}
                public void mouseEntered(MouseEvent e){}
                public void mouseExited(MouseEvent e){}
            });
            
        list.setToolTipText("Double click on file name quick operation mode");
                            

        JButton saveButton = new JButton(hireString);
        SaveLoadListener hireListener = new SaveLoadListener(saveButton);
        //saveButton.setActionCommand(hireString);
        saveButton.addActionListener(hireListener);
        saveButton.setEnabled(false);
        saveButton.setToolTipText(operazione + " the file into/from the personal directory"); 

        cancelButton = new JButton(fireString);
        cancelButton.setBackground(colore);
        cancelButton.setForeground(coloreT);
        //cancelButton.setActionCommand(fireString);
        cancelButton.addActionListener(new CancelListener());
        cancelButton.setToolTipText("Eliminate the file if selected, otherwise do nothing");

        nome = new JTextField(10);
        nome.addActionListener(hireListener);
        nome.getDocument().addDocumentListener(hireListener);
        
       

        //Create a panel that uses BoxLayout.
        JPanel buttonPane = new JPanel();
        buttonPane.setBackground(colore);
        buttonPane.setLayout(new BoxLayout(buttonPane, BoxLayout.LINE_AXIS));
        buttonPane.add(cancelButton);
        buttonPane.add(Box.createHorizontalStrut(5));
        buttonPane.add(new JSeparator(SwingConstants.VERTICAL));
        buttonPane.add(Box.createHorizontalStrut(5));
        buttonPane.add(nome);
        buttonPane.add(saveButton);
        buttonPane.setBorder(BorderFactory.createEmptyBorder(5,5,5,5));

        getContentPane().add(listScrollPane, BorderLayout.CENTER);
        getContentPane().add(buttonPane, BorderLayout.PAGE_END);
    }

    class CancelListener implements ActionListener 
    {
        public void actionPerformed(ActionEvent e) 
        {
            //This method can be called only if
            //there's a valid selection
            //so go ahead and remove whatever's selected.
            int index = list.getSelectedIndex();
            if(!(index < 0))
            {
                nomeFile = (String)listModel.get(index);
                listModel.remove(index);
            }
           
            int size = listModel.getSize();
            
            if (index == listModel.getSize()) 
            {
                //removed item in last position
                index--;
            }
            list.setSelectedIndex(index);
            list.ensureIndexIsVisible(index);
            
            cliente.setNomeFile(nomeFile);
            cliente.cancellaFile();
            
        }
    }

    //This listener is shared by the text field and the hire button.
    class SaveLoadListener implements ActionListener, DocumentListener 
    {
        private boolean alreadyEnabled = false;
        private JButton button;
        
        
        public SaveLoadListener(JButton button) 
        {
            this.button = button;
            this.button.setBackground(colore);
            this.button.setForeground(coloreT);
        }

        //Required by ActionListener.
        public void actionPerformed(ActionEvent e) 
        {
             nomeFile = nome.getText();
             if(!nomeFile.endsWith(".txt"))
                nomeFile = nomeFile.concat(".txt");
             if(hireString.equals("Save"))
             {
                 if (nomeFile.equals("") || alreadyInList(nomeFile)) 
                 {
                     nome.requestFocusInWindow();
                     nome.selectAll();
                     return;               
                 }
                 ArrayList vettore = new ArrayList();
                 int index = 0;                
                 for(int i = 0; i <= listModel.size()-1; i++)
                 {
                     vettore.add(listModel.get(i));
                 }
                 vettore.add(nomeFile);
                 Collections.sort(vettore);                    
                 index = vettore.indexOf(nomeFile);               
                 
                 listModel.insertElementAt(nomeFile, index);
                 

                 cliente.setNomeFile(nomeFile);            
                 cliente.salvaDati();
             }
             else
             {
                 if (!alreadyInList(nomeFile))
                    return;
                 cliente.setNomeFile(nomeFile); 
                 cliente.caricaDati();
                 pannello.repaint();
             }
                     
            FileChooserFrame.this.setVisible(false);
        }


        protected boolean alreadyInList(String name) 
        {
            //if(hireString.equals("Save"))
                //return listModel.contains(name);
            //else
                //return false;
            
            return listModel.contains(name);
        }

        //Required by DocumentListener.
        public void insertUpdate(DocumentEvent e) 
        {
            enableButton();
        }

        //Required by DocumentListener.
        public void removeUpdate(DocumentEvent e) 
        {
            handleEmptyTextField(e);
        }

        //Required by DocumentListener.
        public void changedUpdate(DocumentEvent e) 
        {
            if (!handleEmptyTextField(e)) 
            {
                enableButton();
            }
        }

        private void enableButton() 
        {
            if (!alreadyEnabled) 
            {
                button.setEnabled(true);
            }
        }

        private boolean handleEmptyTextField(DocumentEvent e) 
        {
            if (e.getDocument().getLength() <= 0) 
            {
                button.setEnabled(false);
                alreadyEnabled = false;
                return true;
            }
            return false;
        }
    }    
    

    public String getNomeFile()
    {
        return nomeFile;
    }
    
    

   
    private JList list;
    private DefaultListModel listModel;

    private String hireString;
    private static final String fireString = "Cancel";
    private JButton saveButton;
    private JButton cancelButton;
    private JTextField nome;
    
    private static String nomeFile;
    private Client cliente;
    private Pannello pannello;
    private static final  Color colore = new Color(233, 222, 205);
    private static final  Color coloreT = new Color(57, 24, 24);   
     
  
}
