"""The judge client used to handle socket to server and provide
a clear stdin and stdout protocol to programmer.

TODO: verbose mode to print all input and output. No way when bridging.
TODO: make host and port as arguments.
"""

import socket
import subprocess

HOST = 'localhost'
PORT = 50007

def main():
  client_socket = socket.create_connection((HOST, PORT))

  # Bridge the socket and stdin/stdout.
  # Run dummy program
  # -u means no buffer for input and output, neccesary with python script.
  #proc = subprocess.Popen(["python", "-u", "dummy.py"],
  proc = subprocess.Popen(["./a.out"],
    stdin=client_socket,
    stdout=client_socket,
    close_fds=True)

  # Client knows nothing about the running process, just wait.
  proc.wait()

  client_socket.close()

if __name__ == "__main__":
  main()
