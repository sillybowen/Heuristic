#!/usr/bin/python -u

"""Dummy program.
Readline from input, increase it and output to stdout with a newline.

EOF in stdin means game over.
"""
import time
import sys

def main():
  if len(sys.argv) == 1:
    print 'Usage: ./dummy.py TEAM_NAME'
    sys.exit(1)
  print sys.argv[1]

  while True:
    try:
      s = raw_input()
    except EOFError:
      # print "EOFError"
      # Don't pollute output, output anything to stderr.
      break;
    # Write server's output to stderr, for debugging
    # sys.stderr.write("%s %s\n" % ("Server send:", s))
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
    #sys.stderr.write("%s\n" % s)

    time.sleep(3)
    print "%d,%d " % (firstx + 50, firsty + 50)

if __name__ == "__main__":
  main()
