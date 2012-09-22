#ifndef MATRIX_GRAPH_HEADER
#define MATRIX_GRAPH_HEADER

class MatrixGraph {
public:
  MatrixGraph(int numVertices);
  ~MatrixGraph() {
    for (int i = 0; i < numVertices_; ++i) {
      delete [] edgeMatrix_[i];
    }
    delete [] edgeMatrix_;
  }

private:
  const int numVertices_;
  int numEdges_;
  char* vertNames_;
  double** edgeMatrix_;
};

#endif
