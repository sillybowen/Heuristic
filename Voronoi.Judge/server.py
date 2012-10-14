"""Judge server to simulate the Java server.
Judge server is responsible for timeout. When timeout, just close both socket
to notice clients that game is over.

TODO: timeout

makefile() and thus readline() do not support timeout and require blocking socket.
So we can wrap socket readline() with timeout (TODO).
"""
import socket

# Server
HOST = 'localhost'        # Symbolic name meaning all available interfaces
PORT = 50007              # Arbitrary non-privileged port

def main():
  server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

  # Reuse port.
  server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

  server_socket.bind((HOST, PORT))
  server_socket.listen(5)

  # Accept the first player.
  conn1, addr = server_socket.accept()
  print 'Connected by', addr
  f1 = conn1.makefile()

  # Accept the second player.
  conn2, addr = server_socket.accept()
  print 'Connected by', addr
  f2 = conn2.makefile()

  # Initial state.
  dataArr = []
  dataArr.append('-1,-1 ')

  # TODO: End condition.
  c = 5
  taketurn = True
  while c:
    c -= 1 # Counter

    if (taketurn):
      # First player
      conn1.sendall("%s\n" % ''.join(dataArr))
      # TODO: Timer here.

      newdata = f1.readline().rstrip('\n')
      dataArr.insert(0, newdata)
      print "Player 1: LenOfData: %d, Data: %s" % (len(dataArr), ''.join(dataArr))
      # Handle socket close.
      if not newdata:
        break
    taketurn = False

    if (not taketurn):
      # Second
      conn2.sendall("%s\n" % ''.join(dataArr))
      # TODO: Timer here.

      newdata = f2.readline().rstrip('\n')
      dataArr.insert(0, newdata)
      print "Player 2: LenOfData: %d, Data: %s" % (len(dataArr), ''.join(dataArr))
      # Handle socket close.
      if not newdata:
        break
    taketurn = True

  print "Closing connections..."

  # Close
  f1.close()
  f2.close()
  conn1.close()
  conn2.close()

if __name__ == "__main__":
  main()
