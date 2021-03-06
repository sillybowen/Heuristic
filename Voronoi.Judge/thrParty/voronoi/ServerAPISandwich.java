package voronoi;

import java.net.*;
import java.io.*;
import java.nio.channels.*;
import java.util.*;
import java.util.concurrent.Semaphore;
import java.awt.image.BufferedImage;
import javax.imageio.ImageIO;
import javax.mail.*;
import javax.mail.internet.*;
import javax.activation.*;

import voronoi.utils.Constants;

import javax.swing.*;
import java.awt.Toolkit;
import java.awt.Image;
import java.applet.Applet;

public class Server {
  static int numOfStonesEach = Constants.NUM_OF_STONES_EACH;
  private static boolean isApplication = false;
  private static JFrame frameT;
  private static VoronoiGame gameGui;
  private static PairSet placedStonesSet;

  private static int VORONOI_PORT = Constants.DEFAULT_GAME_PORT;
  private static volatile int numOfPlys = 0;
  private static boolean thirdPlayer = false;

  private ServerSocket serverSocket = null;

  public static boolean isThirdPlayer() {
    return thirdPlayer;
  }

  public Server() { this(VORONOI_PORT); }

  public Server(int srvPort) {
    gameGui = startVoronoiGame();
    placedStonesSet = new PairSet();

    try {
      serverSocket = new ServerSocket(srvPort);
    } catch (IOException ioe) {
      ioe.printStackTrace();
    }
    System.out.println("Game Start:\nNum of Stones Each: " + numOfStonesEach
        + "\nAccepting conns on port: " + srvPort);

    while ((!thirdPlayer&& numOfPlys < Constants.MAX_NUM_PLAYERS)
           ||(thirdPlayer && numOfPlys<Constants.MAX_NUM_PLAYERS2)) {
      Socket cltsocket = null;
      try {
        // Establish client conn, waits until one connect
        cltsocket = serverSocket.accept();
        cltsocket.setKeepAlive(true);
        cltsocket.setTcpNoDelay(true);
      } catch (IOException e) {
        System.out.println("Can NOT accept conn on port: " + srvPort);
      }

      // Start one new thread per client
      ServerConnection sconn = new ServerConnection(cltsocket, numOfPlys);
      Thread cltThr = new Thread(sconn);
      cltThr.start();
      ++numOfPlys;
      System.out.println("# of ply"+numOfPlys);
    }
    System.out.println("outside while");
  }

  public static void main(String[] args) {
    thirdPlayer = false;
    if (args.length < 2) {
      System.out.println("Format: java -cp compiledClasses/ voronoi.Server"
          + " srv_port  num_stones_each  num_of_players(2 or 3)");
      System.exit(1);
    }

    try {
      if (args.length > 1) {
        VORONOI_PORT = Integer.parseInt(args[0]);
        numOfStonesEach = Integer.parseInt(args[1]);
      }
      if (args.length > 2) {
        if (Integer.parseInt(args[2]) == 3) {
          thirdPlayer = true;
          System.out.println("Sandwich Game: 3 players");
        }
      }
    } catch (Exception e) {
      System.out.println("Invalid port");
    } finally {
      Server srv = new Server(VORONOI_PORT);
    }
  }

  public static VoronoiGame startVoronoiGame() {
    return VoronoiGame.startGame();
  }

  // Check whether a stone is a legal placement, if true, record it
  public static boolean checkNewStoneThenAdd(int x, int y) {
    if (x < 0 || x > Constants.WIDTH_OF_SQUARE || y < 0
        || y > Constants.WIDTH_OF_SQUARE) {
      return false;
        }
    if (placedStonesSet.find(x, y)) {  // Place already taken
      return false;
    }
    return placedStonesSet.insert(x, y);
  }

  public static VoronoiGame getVoronoiGame() {
    return gameGui;
  }

  public static int getNumOfPlys() {
    return numOfPlys;
  }

  public static void forceClose() {
    // System.exit(-1);
    while (true) { }
  }

}

class ServerConnection extends Thread {
  private static int stonesUsed = 0;
  private static Socket[] cltSockets_ = new Socket[Constants.MAX_NUM_PLAYERS2];
  private static String[] players_ = new String[Constants.MAX_NUM_PLAYERS2];
  private static String stonesData = new String();
  private static boolean taketurn = false;
  private static int taketurn2 = 0;

  // private static final Semaphore mutex = new Semaphore(1);

  private BufferedReader in = null;
  private PrintWriter out = null;
  private int ind_;

  // index = 0 or 1
  public ServerConnection(Socket cltsocket, int index) {
    try {
      ind_ = index;
      this.in = new BufferedReader(new InputStreamReader(
            cltsocket.getInputStream()));
      this.out = new PrintWriter(cltsocket.getOutputStream(), true);

      String teamName = in.readLine();
      setPlayerName(index, teamName);
      setCltSocket(index, cltsocket);

      System.out.println("Connected to player: " + players_[index] + " Addr: "
          + cltsocket.getInetAddress().toString() + ", " + cltsocket.getPort());
    } catch (IOException e) {
      System.out.println("ServerConnection thread Read failed");
      System.exit(-1);
    }
  }

  public static void setCltSocket(int index, Socket cltsocket) {
    cltSockets_[index] = cltsocket;
  }

  public static void setPlayerName(int index, String name) {
    players_[index] = name;
  }

  public void run2() {
    System.out.println("int run 2 (Sandwich Voronoi Game)");
    String area = new String();
    while (true) {
      if (Server.getNumOfPlys() != Constants.MAX_NUM_PLAYERS2)
        continue;
      try {
        if (ind_ !=  (taketurn2%3))
          continue;
        if (stonesUsed == Server.numOfStonesEach * 3) {
          in.close();
          out.close();
          System.out.println("Game Over: " + ((Server.getVoronoiGame().RedArea()*2 >
                  Server.getVoronoiGame().BlueArea())?
                (players_[1] + " wins") : 
                (players_[0] +" and "+players_[2]+ " wins")));
          Server.forceClose();
        }
        //Send aggregated data back to client
        out.println(stonesData+area);
        System.out.println("#Server# " + stonesData + area + "\n");
        ++stonesUsed;
        String line = in.readLine();
        String strippedLine = line.replace("\n","").replace(" ", "");
        String[] splitStrs = strippedLine.split(",");
        int x;
        int y;
        if (splitStrs.length == 2) {
          x = Integer.parseInt(splitStrs[0]);
          y = Integer.parseInt(splitStrs[1]);
          if (Server.checkNewStoneThenAdd(x, y)) {
            System.out.println("Server got From ply: " + ind_ + " : (" + x + ", "
                + y + ")");
            stonesData = ((ind_ + 1) % (Server.getNumOfPlys())) + " "
              + strippedLine + "," + ind_ + " " + stonesData;
            // VoronoiGame.artificialMouseClick(x, y, 0.65);

            try {
              Server.getVoronoiGame().getIsPaintingSemaphore().acquire();
            } catch (InterruptedException ie) {
              ie.printStackTrace();
            }
            Server.getVoronoiGame().humanPutStone2(x, y, players_[ind_],ind_);

            area = " | "
              +Double.toString(Server.getVoronoiGame().RedArea()) + ","
              +Double.toString(Server.getVoronoiGame().BlueArea());
          } else {
            System.out.println("Illegal placement! " + players_[ind_] +
                " wasted a stone!");
          }
        } else {
          System.out.println("Wrong input, lose this stone!");
        }
      } catch (IOException e) {
        System.out.println("Read failed");
        System.exit(-1);
      }
      taketurn2 = (taketurn2+1) % 3;
    }
  }

  public void run() {
    if (Server.isThirdPlayer()) {
      run2();
      return ;
    }

    String area = new String();
    while (true) {
      if (Server.getNumOfPlys() != Constants.MAX_NUM_PLAYERS)
        continue;
      try {
        if ((ind_ == 1) ^ taketurn)
          continue;
        if (stonesUsed == Server.numOfStonesEach * 2) {
          in.close();
          out.close();
          System.out.println("Game Over: " + ((Server.getVoronoiGame().RedArea() >
                  Server.getVoronoiGame().BlueArea())?
                (players_[1] + " wins") : (players_[0] + " wins")));
          Server.forceClose();
        }

        //Send aggregated data back to client
        out.println(stonesData+area);
        System.out.println("#Server# " + stonesData + area + "\n");
        ++stonesUsed;
        String line = in.readLine();
        String strippedLine = line.replace("\n","").replace(" ", "");
        String[] splitStrs = strippedLine.split(",");
        int x;
        int y;
        if (splitStrs.length == 2) {
          x = Integer.parseInt(splitStrs[0]);
          y = Integer.parseInt(splitStrs[1]);
          if (Server.checkNewStoneThenAdd(x, y)) {
            System.out.println("Server got From ply: " + ind_ + " : (" + x + ", "
                + y + ")");
            stonesData = ((ind_ + 1) % (Server.getNumOfPlys())) + " "
              + strippedLine + "," + ind_ + " " + stonesData;
            // VoronoiGame.artificialMouseClick(x, y, 0.65);

            try {
              Server.getVoronoiGame().getIsPaintingSemaphore().acquire();
            } catch (InterruptedException ie) {
              ie.printStackTrace();
            }
            Server.getVoronoiGame().humanPutStone(x, y, players_[ind_]);

            area = " | "
              +Double.toString(Server.getVoronoiGame().RedArea()) + ","
              +Double.toString(Server.getVoronoiGame().BlueArea());
          } else {
            System.out.println("Illegal placement! " + players_[ind_] +
                " wasted a stone!");
          }
        } else {
          System.out.println("Wrong input, lose this stone!");
        }

      } catch (IOException e) {
        System.out.println("Read failed");
        System.exit(-1);
      }
      taketurn = !taketurn;
    }
  }
}
