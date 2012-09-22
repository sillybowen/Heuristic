#include "matrixGraph.h"

MatrixGraph::MatrixGraph(int numVertices) : numVertices_(numVertices) {
  edgeMatrix_ = new double*[numVertices_];
  for (int i = 0; i < numVertices_; ++i) {
    edgeMatrix_[i] = new double[numVertices_];
  }
}
