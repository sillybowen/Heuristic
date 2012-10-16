#!/usr/bin/python
"""The judge client used to handle socket to server and provide
a clear stdin and stdout protocol to programmer.

TODO: verbose mode to print all input and output. No way when bridging.
TODO: make host and port as arguments.
"""

import sys
import socket
import subprocess

HOST = 'localhost'
PORT = 5678

def main():
  if len(sys.argv) < 3:
    print 'Usage: ./client.py  YOUR_PROGRAM  PORT_NUM  TEAM_NAME'
    sys.exit(1)
  PORT = int(sys.argv[2])

  client_socket = socket.create_connection((HOST, PORT))

  # Bridge the socket and stdin/stdout.
  # Run dummy program
  # -u means no buffer for input and output, neccesary with python script.
  proc = subprocess.Popen([sys.argv[1], sys.argv[3]],
  # proc = subprocess.Popen(["./a.out"],
    stdin=client_socket,
    stdout=client_socket,
    close_fds=True)

  # Client knows nothing about the running process, just wait.
  proc.wait()

  client_socket.close()

if __name__ == "__main__":
  main()
