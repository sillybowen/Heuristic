#ifndef MATRIX_GRAPH_HEADER
#define MATRIX_GRAPH_HEADER

#include <iostream>
#include <iomanip>
#include <vector>
#include "basic_geo.h"  // For struct edge

#define EDGEDISTANCEWIDTH 6

using namespace std;

class MatrixGraph {
public:
  MatrixGraph(int numVertices, const vector<edge>& mstEdges,
      const vector<edge>& matchEdges);
  ~MatrixGraph();

  void dumpGraph() const;

private:
  int numVertices_;
  int numEdges_;
  double** edgeMatrix_;

  MatrixGraph(const MatrixGraph&);
  MatrixGraph& operator=(const MatrixGraph&);
};

#endif
