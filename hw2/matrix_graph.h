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

  void printEulerCircuitLs(list<int>* pEulerCirLs) const;
  void dumpDisGraph() const;
  void dumpEulerDFSGraph() const;

private:
  int numVertices_;
  int numEdges_;
  double** edgeMatrix_;
  unordered_set<int>** eulerDFSMatrix_;
  int* vertexDegrees_;

  MatrixGraph(const MatrixGraph&);
  MatrixGraph& operator=(const MatrixGraph&);
};

#endif
