#ifndef MATRIX_GRAPH_HEADER
#define MATRIX_GRAPH_HEADER

#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <unordered_set>
#include "basic_geo.h"  // For struct edge

#define EDGEDISTANCEWIDTH 6
#define EULERDFSEDGEWIDTH 3

using namespace std;

class MatrixGraph {
public:
  MatrixGraph(int numVertices, const vector<edge>& mstEdges,
      const vector<edge>& matchEdges);
  ~MatrixGraph();
  void ifEulerianGraph() const;
  void eulerCircuitDFS(list<int>& toSpliceLs, int startVertex);
  // This transfer ownership of the found "circuit list" to the caller
  list<int>* findEulerCircuit(int startVertex);
  void trimEulerCircuitToTSP(list<int>* pEulerCircuit);
  // Also check whether every node has been visited from index 0 to numVertices_-1
  // Must make sure vertices numbering are from 0 to numVertices_-1
  double sumTSPDistance(const list<int>* pTSPTrip, vector<point>& points) const;

  void printOutputTSPTrip(const list<int>* pTSPTrip, const char* outFileN) const;
  void printEulerCircuitLs(list<int>* pEulerCirLs) const;
  void dumpDisGraph() const;
  void dumpEulerDFSGraph() const;
  vector<int> giveResult();
private:
  int numVertices_;
  int numEdges_;
  double** edgeMatrix_;
  unordered_multiset<int>** eulerDFSMatrix_;
  int* vertexDegrees_;
  vector<int> ans_seq;
  MatrixGraph(const MatrixGraph&);
  MatrixGraph& operator=(const MatrixGraph&);
};

#endif
