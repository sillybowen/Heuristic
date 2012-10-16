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
  static final int numOfStonesEach = Constants.NUM_OF_STONES_EACH;
  private static boolean isApplication = false;
  private static JFrame frameT;
  private static VoronoiGame gameGui;
  private static PairSet placedStonesSet;

  private static int VORONOI_PORT = Constants.DEFAULT_GAME_PORT;
  private static volatile int numOfPlys = 0;
  private ServerSocket serverSocket = null;

  public Server() { this(VORONOI_PORT); }

  public Server(int srvPort) {
    gameGui = startVoronoiGame();
    placedStonesSet = new PairSet();

    try {
      serverSocket = new ServerSocket(srvPort);
    } catch (IOException ioe) {
      ioe.printStackTrace();
    }
    System.out.println("Accepting conns on port: " + srvPort);

    while (numOfPlys < Constants.MAX_NUM_PLAYERS) {
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
    }
  }

  public static void main(String[] args) {
    try {
      if (args.length > 0) {
        VORONOI_PORT = Integer.parseInt(args[0]);
      }
    } catch (Exception e) {
      System.out.println("Invalid port");
    } finally {
      Server srv = new Server();
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
    System.exit(-1);
  }

}

class ServerConnection extends Thread {
  private static int stonesUsed = 0;
  private static Socket[] cltSockets_ = new Socket[Constants.MAX_NUM_PLAYERS];
  private static String[] players_ = new String[Constants.MAX_NUM_PLAYERS];
  private static String stonesData = new String();
  private static boolean taketurn = false;
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

  public void run() {
      String area = new String();
    while (true) {
      if (Server.getNumOfPlys() != Constants.MAX_NUM_PLAYERS)
        continue;

      try {
        if ((ind_ == 1) ^ taketurn)
          continue;
        if (stonesUsed == Server.numOfStonesEach * 2) {
          Server.forceClose();
        }

        //Send aggregated data back to client
        out.println(stonesData+area);
        System.out.println("#Server: " + stonesData + area);
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
            stonesData = line + stonesData;
            area = " | "
                +Double.toString(Server.getVoronoiGame().RedArea()) + ","
                +Double.toString(Server.getVoronoiGame().BlueArea()) ;
          } else {
            System.out.println("Illegal placement! " + players_[ind_] +
                " wasted a stone!");
          }

          // VoronoiGame.artificialMouseClick(x, y, 0.65);
          Server.getVoronoiGame().humanPutStone(x, y, players_[ind_]);
        } else {
          System.out.println("Wrong input, lose this stone!");
        }

        // System.out.println("Server got From ply: " + ind_ + " : " + line + '\n');
      } catch (IOException e) {
        System.out.println("Read failed");
        System.exit(-1);
      }

      taketurn = !taketurn;
    }
  }

}
