#include <cstdlib>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <list>
#include "basic_geo.h"
#include "matrix_graph.h"

using namespace std;

#define MAXNUMOFEDGES 100

/*
 * Format (all int):
 * start_p1   end_p1
 * start_p2   end_p2
 * ...
 * start_pn   end_pn
 */
int readInput(const char* filename, vector<edge>& outputEdges) {
  ifstream inputf;
  inputf.open(filename);
  assert(inputf.is_open());
  outputEdges.reserve(MAXNUMOFEDGES);
  set<int> verticesSet;

  int tmpx, tmpy;
  for (int i = 0; i < MAXNUMOFEDGES; ++i) {
    inputf >> tmpx >> tmpy;
    if (inputf.eof())
      break;
    assert(tmpx != tmpy);
    verticesSet.insert(tmpx);
    verticesSet.insert(tmpy);

    edge anEdge(tmpx, tmpy, -1.0);  // "-1.0" -- mark for invalid distance
    outputEdges.push_back(anEdge);
  }

  return (int) verticesSet.size();
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    cout << "Please enter Euler Circuit Test input file and StartVertex!\n";
    return 1;
  }

  vector<edge> testEdges, emptyEdge;
  int numOfVertices = readInput(argv[1], testEdges);
  int startV = atoi(argv[2]);
  MatrixGraph myEuler(numOfVertices + 1, testEdges, emptyEdge);
  myEuler.dumpEulerDFSGraph();

  list<int>* finalCircuit = myEuler.findEulerCircuit(startV);
  myEuler.trimEulerCircuitToTSP(finalCircuit);

  cout << endl;
  myEuler.dumpEulerDFSGraph();
  delete finalCircuit;

  return 0;
}
