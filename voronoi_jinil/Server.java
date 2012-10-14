import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {
  ServerSocket serverSocket = null;
  Socket[] clientSocket = new Socket[2];
  static int port = 5678;	//default

  public Server(int port) {
    try {
      serverSocket = new ServerSocket(port);
    } catch (IOException e1) {
      // TODO Auto-generated catch block
      e1.printStackTrace();
    }

    System.out.println("Accepting connections on port " + port);
    int i = 0;
    while (i <= 2) {	//waits for 2 clients to connect. Once they both disconnenct the server terminates. 
      try {
        //establishes client connection. Waits till a client connects
        clientSocket[i] = serverSocket.accept();
        clientSocket[i].setKeepAlive(true);
        clientSocket[i].setTcpNoDelay(true);

        ObjectOutputStream oos = new ObjectOutputStream(

        //start new thread, one per client
        /*
           ServerConnection sconn = new ServerConnection(clientSocket);
           Thread  clientThread = new Thread(sconn);
           clientThread.start();
        */

        i++;
      } catch (IOException e) {
        System.out.println("could not accept connection on port " + port);
      }
    }
  }

  public Server() {
    this(port);
  }

  public boolean getStopProcessing() {
    return true;
  }

  public static void main(String[] args) {

    System.out.println("Start");
    try {
      if (args.length > 0) {
        port = Integer.parseInt(args[0]);
      }	
    } catch (Exception e) {
      System.out.println("Invalid port");
    } finally {
      Server s = new Server();
    }
  }

  public static void forceClose() {
    System.exit(-1);
  }
}
