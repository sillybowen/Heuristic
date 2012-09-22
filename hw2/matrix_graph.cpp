#include "matrix_graph.h"

MatrixGraph::MatrixGraph(int numVertices, const vector<edge>& mstEdges,
    const vector<edge>& matchEdges) : numVertices_(numVertices) {
  edgeMatrix_ = new double*[numVertices_];
  for (int i = 0; i < numVertices_; ++i) {
    edgeMatrix_[i] = new double[numVertices_];
    for (int j = 0; j < numVertices_; ++j)
      edgeMatrix_[i][j] = 0.0;
  }

  const int mstEdgeSize = mstEdges.size();
  const int matchEdgeSize = matchEdges.size();
  for (int i = 0; i < mstEdgeSize; ++i) {
    edgeMatrix_[mstEdges[i].p[0]][mstEdges[i].p[1]] = mstEdges[i].dis;
    // Copy for the other direction: p[1] -> p[0]
    edgeMatrix_[mstEdges[i].p[1]][mstEdges[i].p[0]] = mstEdges[i].dis;
  }
  for (int i = 0; i < matchEdgeSize; ++i) {
    edgeMatrix_[matchEdges[i].p[0]][matchEdges[i].p[1]] = matchEdges[i].dis;
    // Copy for the other direction: p[1] -> p[0]
    edgeMatrix_[matchEdges[i].p[1]][matchEdges[i].p[0]] = matchEdges[i].dis;
  }

  numEdges_ = mstEdgeSize + matchEdgeSize;
}

MatrixGraph::~MatrixGraph() {
  for (int i = 0; i < numVertices_; ++i) {
    delete [] edgeMatrix_[i];
  }
  delete [] edgeMatrix_;
}

void MatrixGraph::dumpGraph() const {
  for (int i = 0; i < numVertices_; ++i) {
    for (int j = 0; j < numVertices_; ++j) {
      cout << setw(EDGEDISTANCEWIDTH) << edgeMatrix_[i][j] << ' ';
    }
    cout << endl;
  }
}
