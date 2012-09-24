#include <cstdlib>
#include <ctime>
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
#define MAXNUMOFDEGREES 7
#define MAXVERTEXINDEX 97

// N is suggested # of edges
void generateEdges(const char* outFileName, int N) {
  ofstream outFile;
  outFile.open(outFileName);
  assert(outFile.is_open());
  int numOfEdges = 0;
  srand(time(NULL));

  while (numOfEdges < N) {
    int startV = rand() % MAXVERTEXINDEX;
    int startVDegree = ((rand() % MAXNUMOFDEGREES) | 0x1) + 1;
    cout << "startV = " << startVDegree << endl;
    assert(startVDegree <= (MAXNUMOFDEGREES + 1) && startVDegree > 1);
    assert(!(startVDegree % 2));
    for (int i = 0; i < startVDegree; ++i) {
      int endV;
      while ((endV = rand() % MAXVERTEXINDEX) == startV);
      assert(endV != startV);
      // Form an edge startV <-> endV
      outFile << startV << " " << endV << endl;
      ++numOfEdges;
    }
  }
}

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
  if (argc < 4) {
    cout << "Please enter Euler Circuit Test_file(will Auto create one)\n"
      << "and |Max Num Edges| also |StartVertex|!\n";
    return 1;
  }

  vector<edge> testEdges, emptyEdge;
  // generateEdges(argv[1], atoi(argv[2]));
  int numOfVertices = readInput(argv[1], testEdges);
  int startV = atoi(argv[3]);

  MatrixGraph myEuler(MAXNUMOFEDGES, testEdges, emptyEdge);
  myEuler.ifEulerianGraph();
  // myEuler.dumpEulerDFSGraph();

  // list<int>* finalCircuit = myEuler.findEulerCircuit(startV);
  // myEuler.trimEulerCircuitToTSP(finalCircuit);

  // cout << endl;
  // myEuler.dumpEulerDFSGraph();
  // delete finalCircuit;

  return 0;
}
