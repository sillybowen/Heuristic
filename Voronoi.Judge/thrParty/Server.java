import java.net.*;
import java.io.*;
import java.nio.channels.*;
import java.util.*;
import java.awt.image.BufferedImage;
import javax.imageio.ImageIO;
import javax.mail.*;
import javax.mail.internet.*;
import javax.activation.*;


//import java.io.FileWriter;
//import java.io.FileReader;
//import java.io.IOException;
//import java.io.File;
//import java.io.BufferedReader;

             
/**
 * Implementa un Server
 * 
 * @author Indrit Selimi
 * @version 1.0
 */

public class Server
{
    public static void main(String[] args)
    {
        try
        {
            ServerSocket s = new ServerSocket(VORONOI_PORT);
            String root;
            String sisOperativo = System.getProperty("os.name");
            if((sisOperativo.toLowerCase()).startsWith("win"))
            {
                root = "C:";
            }
            else if((sisOperativo.toLowerCase()).startsWith("lin"))
            {
                root = "/home/indrit";
            }
            else
            {
                root = "mac";
            }
         
            File voronoiFile = new File(root + File.separator + "Voronoi");
            if(!voronoiFile.exists())
                voronoiFile.mkdir();
            File autentFile = new File(voronoiFile, "AUTHENTICATION.txt");
            FileWriter  fileBaseDatiO = new FileWriter(autentFile);
            FileReader  fileBaseDatiI = new FileReader(autentFile);           
                
            
            PrintWriter writeBase = new PrintWriter(fileBaseDatiO, true);
            BufferedReader readBase = new BufferedReader(fileBaseDatiI);
            
            Properties proprieta = System.getProperties();
            proprieta.put("mail.smtp.host", "your.server");            
            
            writeBase.println("predefinito" + "|" +  "predefinito" + "|" + "predefinito" + "|" + "predefinito");
            
            Thread time = new ThreadTimeControl(voronoiFile, root);
            time.setPriority(10);
            time.start();            

            while(true)
            {
                Socket canale = s.accept();
                Thread t = new ThreadCommunication(canale, writeBase, readBase, proprieta, root);
                t.setDaemon(true);
                t.setPriority(4);
                t.start(); 
            }
            
            
        }
        catch(IOException  e)
        {
            e.printStackTrace();
            //writeBase.close();            
        }
      }
    

    private static final int VORONOI_PORT = 1212; 
}

    class ThreadCommunication extends Thread
    {
        public ThreadCommunication(Socket unSocket, PrintWriter unWriter, BufferedReader unReader, Properties leProprieta,String root)
        {
            canale = unSocket;
            this.writeBase = unWriter;
            this.readBase = unReader;
            this.proprieta = leProprieta;
            this.root = root;
        }
        
        public void run()
        {
            try
            {
                InputStream inStream = canale.getInputStream();
                BufferedReader in = new BufferedReader(new InputStreamReader(inStream));
                PrintWriter out = new PrintWriter(canale.getOutputStream(), true);
                sessioneUtente = Session.getDefaultInstance(proprieta);
                checkDatiUtente(in);
                verificaDati();            
               
                while(!codice.equals("END_TRASMISSION"))
                {
                 if(codice.equals("SaveAsCommand"))
                 {
                     fileSave = new File(root + File.separator + "Voronoi" + File.separator + directoryUtente + File.separator  + nomeFile);
                     file = new FileWriter(fileSave);                     
                     outLocale = new PrintWriter(file, true);
                     String riga;  
                     while(!(((riga = in.readLine())).equals("Fine")))
                     {
                        outLocale.println(riga);                    
                        //if(outLocale.checkError())
                          //  System.out.println("Errore");
                     }
                     file.close();
                }                  
                else if(codice.equals("LoadCommand"))
                {
                    fileSave = new File(root + File.separator + "Voronoi" + File.separator + directoryUtente + File.separator  + nomeFile);
                    readerLocale = new FileReader(fileSave);
                    inputLocale = new BufferedReader(readerLocale);
                    String riga;  
                    while((riga = inputLocale.readLine()) != null)
                    {
                       out.println(riga);
                    }
                    out.println("Fine");
                    readerLocale.close();
                }
                
                else if(codice.equals("DammiFile"))
                {
                    File directory = new File(root + File.separator + "Voronoi" + File.separator + directoryUtente);
                    spedisciFiles(out, directory);
                                             
                }
                else if(codice.equals("CancellaFile"))
                {
                    File cancella = new File(root + File.separator + "Voronoi" + File.separator + directoryUtente + File.separator + nomeFile);
                    if(!cancella.delete())
                        cancella.delete();
                }
                else if(codice.equals("SalvaImmagine"))
                {
                    try
                    {
                        //BufferedImage immagine = ImageIO.read(inStream);
                        BufferedImage immagine = ImageIO.read(ImageIO.createImageInputStream(inStream));
                        File imageFile = new File(root + File.separator + "Voronoi" + File.separator + directoryUtente + File.separator + "Image.jpg");
                        ImageIO.write(immagine, "JPEG", imageFile);
                    }
                    catch(IOException ee){  }
                    catch(IllegalArgumentException ee){  }
                    
                }
                
                else if(codice.equals("SpedisciMail"))            
                {
                    spedisciMail(new File(root + File.separator + "Voronoi" + File.separator + directoryUtente + File.separator + "Image.jpg"));                      
                }
                    
                checkDatiUtente(in);               
                    
          }
       }
       catch(IOException e){ }       
       finally
       {
           try{canale.close();} catch(IOException eccc){}
       }
     }
       
     public void checkDatiUtente(BufferedReader ingresso)
     {
         try
         {
             String riga;
             if((riga = ingresso.readLine()) != null)
             {
                 try
                 {
                     StringTokenizer divisore = new StringTokenizer(riga, "|");
                     codice = divisore.nextToken();
                     utente = divisore.nextToken();
                     password = divisore.nextToken();                
                     nomeFile = divisore.nextToken();
                     if(divisore.hasMoreTokens())
                     email = divisore.nextToken();
                 }
                 catch(NoSuchElementException eee){}
             }
            
         }
         catch(IOException ecccc){}         
          
      }
       
       public void verificaDati()
       {
           String riga;
           boolean done = false;
           
           try
           {
               File fileTest = new File(root + File.separator + "Voronoi" + "AUTHENTICATION.txt");
               FileOutputStream fileTesty = new FileOutputStream(fileTest);
               FileChannel channel = fileTesty.getChannel(); 
               FileLock lock = channel.lock();
               while((!done) && (riga = readBase.readLine()) != null)
               {
                   StringTokenizer divisore = new StringTokenizer(riga, "|");
                   try
                   {
                       if(utente.equals(divisore.nextToken()) && password.equals(divisore.nextToken()))
                       {
                           directoryUtente = divisore.nextToken();
                           if(divisore.hasMoreTokens())
                             email =  divisore.nextToken();
                           done = true;
                       }
                   
                   }
                   catch(NoSuchElementException ecc)
                   {
                      //questo va gestito!!!
                   }
               
               }
               if(!done)
               {
                   directoryUtente = utente.concat(password);
                   File dirUtente = new File(root + File.separator + "Voronoi" + File.separator + directoryUtente);
                   dirUtente.mkdir();                                     
                   
                   if(email == null)
                       writeBase.println(utente + "|" +  password + "|" + directoryUtente);
                   else
                       writeBase.println(utente + "|" +  password + "|" + directoryUtente + "|" + email);
                                  
               }
               lock.release();
                     
            
          }
          catch(IOException ecc){}
         
        }
        
       public void spedisciFiles(PrintWriter out, File dirUtente)
       {
           String[] listaFile = dirUtente.list();
           if(listaFile !=  null)
           {
           for (int i = 0; i < listaFile.length; i++)
           {
               out.println(listaFile[i]);
           }
           out.println("Fine");
           }
           
       }
       
       public void spedisciMail(File unFile)
       {           
           Message messaggio = new MimeMessage(sessioneUtente);
           try
           {
               messaggio.setFrom(new InternetAddress("indritselimi@hotmail.com", "Unipg"));
               messaggio.addRecipient(Message.RecipientType.TO, new InternetAddress(email));
               messaggio.setSubject("Your captured Voronoi Game image");
              
               BodyPart parteTesto = new MimeBodyPart();
               parteTesto.setText("Enjoy the Voronoi Game! " + "For any information please contact me at: " +
                "inditselimi@hotmail.com");
               
               
               Multipart multipart = new MimeMultipart();
               multipart.addBodyPart(parteTesto);
               
               BodyPart allegato = new MimeBodyPart();
               DataSource sorgente = new FileDataSource(unFile);
               allegato.setDataHandler(new DataHandler(sorgente));
               allegato.setFileName("VoronoiGame Image.jpg");
               multipart.addBodyPart(allegato);
               
               messaggio.setContent(multipart);
               
               Transport.send(messaggio);               
               
           }
           catch(AddressException aecc){}
           catch(MessagingException aecc){}
           catch(UnsupportedEncodingException aecc){}  
           
           unFile.delete();
                      
           
       } 
      
       
       
       
       private File fileSave; //partendo da nomeFile
       private FileWriter file; //partendo da fileSave
       private PrintWriter outLocale; //Scrive nel file locale utente, partendo da file
       private FileReader readerLocale;
       private BufferedReader inputLocale;
       private Socket canale;
       
       
       private String nomeFileTemp;
       private String directoryUtente;
       
       private String codice;  
       private String utente;
       private String password;
       private String nomeFile;
       private String email;
        
       private static  PrintWriter writeBase;
       private static BufferedReader readBase;
        
       private boolean isSaveCommand = false;
       
       private Properties proprieta;
       private String root;
       private Session sessioneUtente;  
        
            
  }
  
       class ThreadTimeControl extends Thread
       {
           public ThreadTimeControl(File dirRoot, String root)
           {
              rootDirectory = dirRoot;
              this.root = root;
           }
           public void run()
           {
              
               String[] listaDirectory = rootDirectory.list();
               if(listaDirectory !=  null)
               {
                   for (int i = 0; i < listaDirectory.length; i++)
                   {
                       File dirUtente = new File(root + File.separator + "Voronoi" + File.separator + listaDirectory[i]); 
                       if(dirUtente.isDirectory())
                       {
                           long tempo = dirUtente.lastModified();
                           
                           if(Math.abs(tempo -  oggi) > mese)
                           {
                               String[] listaFile = dirUtente.list();
                               if(listaFile !=  null)
                               {
                                   for (int j = 0; j < listaFile.length; j++)
                                   {
                                        File fileUtente = new File(dirUtente, listaFile[j]); 
                                        fileUtente.delete();                                        
                                   }                                 
                               }
                               //dirUtente.delete();
                           }
                        }
                    }
                }
                try
                {
                    Thread.sleep(3600 * 24 * 1000);
                    this.run();
                }
                catch(InterruptedException intecc){Thread.currentThread().interrupt();} 
            }
            
            private File rootDirectory;            
            final long mese = 1000 * 3600 * 24 * 30;  // 30 giorni
            final long oggi = 299894257 * 3600 + 3101;
            private String root;
            
           }
       

  
