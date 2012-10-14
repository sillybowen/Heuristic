"""Dummy program.
Readline from input, increase it and output to stdout with a newline.

EOF in stdin means game over.
"""
import time

def main():
  while True:
    try:
      s = raw_input()
    except EOFError:
      # print "EOFError"
      # Don't pollute output, output anything to stderr.
      break;
    # Get the first tuple of the server input
    tuples = s.split()
    if len(tuples):
      firstTuple = tuples[0]
      splitFirTuple = firstTuple.split(',')
      firstx = int(splitFirTuple[0])
      firsty = int(splitFirTuple[1])
    else:
      firstx = 100
      firsty = 100
    # raw_input doesn't return \n

    time.sleep(1)
    print "%d,%d " % (firstx + 1, firsty + 1)

if __name__ == "__main__":
  main()
