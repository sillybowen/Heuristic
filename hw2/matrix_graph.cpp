#include <cassert>
#include <set>
#include "matrix_graph.h"

MatrixGraph::MatrixGraph(int numVertices, const vector<edge>& mstEdges,
    const vector<edge>& matchEdges) : numVertices_(numVertices) {
  edgeMatrix_ = new double*[numVertices_];
  eulerDFSMatrix_ = new int*[numVertices_];
  vertexDegrees_ = new int[numVertices_];

  for (int i = 0; i < numVertices_; ++i) {
    vertexDegrees_[i] = 0;
    edgeMatrix_[i] = new double[numVertices_];
    eulerDFSMatrix_[i] = new int[numVertices_];
    for (int j = 0; j < numVertices_; ++j) {
      edgeMatrix_[i][j] = 0.0;
      eulerDFSMatrix_[i][j] = 0;  // '0' means no edge (or edge has been removed)
    }
  }

  const int mstEdgeSize = mstEdges.size();
  const int matchEdgeSize = matchEdges.size();
  for (int i = 0; i < mstEdgeSize; ++i) {
    vertexDegrees_[mstEdges[i].p[0]] += 1;
    vertexDegrees_[mstEdges[i].p[1]] += 1;
    edgeMatrix_[mstEdges[i].p[0]][mstEdges[i].p[1]] = mstEdges[i].dis;
    eulerDFSMatrix_[mstEdges[i].p[0]][mstEdges[i].p[1]] = 1;
    // Copy for the other direction: p[1] -> p[0]
    edgeMatrix_[mstEdges[i].p[1]][mstEdges[i].p[0]] = mstEdges[i].dis;
    eulerDFSMatrix_[mstEdges[i].p[1]][mstEdges[i].p[0]] = 1;
  }
  for (int i = 0; i < matchEdgeSize; ++i) {
    vertexDegrees_[matchEdges[i].p[0]] += 1;
    vertexDegrees_[matchEdges[i].p[1]] += 1;
    edgeMatrix_[matchEdges[i].p[0]][matchEdges[i].p[1]] = matchEdges[i].dis;
    eulerDFSMatrix_[matchEdges[i].p[0]][matchEdges[i].p[1]] = 1;
    // Copy for the other direction: p[1] -> p[0]
    edgeMatrix_[matchEdges[i].p[1]][matchEdges[i].p[0]] = matchEdges[i].dis;
    eulerDFSMatrix_[matchEdges[i].p[1]][matchEdges[i].p[0]] = 1;
  }

  numEdges_ = mstEdgeSize + matchEdgeSize;
}

MatrixGraph::~MatrixGraph() {
  delete [] vertexDegrees_;

  for (int i = 0; i < numVertices_; ++i) {
    delete [] edgeMatrix_[i];
    delete [] eulerDFSMatrix_[i];
  }
  delete [] edgeMatrix_;
  delete [] eulerDFSMatrix_;
}

void MatrixGraph::eulerCircuitDFS(list<int>& toSpliceLs, int startVertex) {
  toSpliceLs.push_back(startVertex);
  for (int i = 0; i < numVertices_; ++i) {
    if (eulerDFSMatrix_[startVertex][i] == 1) {
      // Remove this used edge
      eulerDFSMatrix_[i][startVertex] = 0;
      eulerDFSMatrix_[startVertex][i] = 0;
      --vertexDegrees_[i];
      --vertexDegrees_[startVertex];
      --numEdges_;
      eulerCircuitDFS(toSpliceLs, i);
      break;
    }
  }
}

list<int>* MatrixGraph::findEulerCircuit(int startVertex) {
  int copyStartVert = startVertex;
  if (startVertex >= numVertices_ || startVertex < 0)
    return NULL;

  list<int> tmpVertLs;
  list<int>* pEulerCirLs = new list<int>();
  list<int>::iterator crit = pEulerCirLs->begin();

  while (numEdges_ > 0) {
    assert(tmpVertLs.size() == 0);
    eulerCircuitDFS(tmpVertLs, startVertex);
    // First remove the last element of tmpVertLs before splice (that's redundant)
    // ! only push_back the original starting vertex when it's not the first loop
    if (!pEulerCirLs->empty())
      tmpVertLs.pop_back();
    // Splice tmpVertLs into pEulerCirLs
    pEulerCirLs->splice(crit, tmpVertLs);

    for (crit = pEulerCirLs->begin(); crit != pEulerCirLs->end(); ++crit) {
      if (vertexDegrees_[*crit] > 0) {
        startVertex = *crit;
        break;
      }
    }
  }
  printEulerCircuitLs(pEulerCirLs);

  return pEulerCirLs;
}

void MatrixGraph::trimEulerCircuitToTSP(list<int>* pEulerCircuit) {
  set<int> vertSet;
  list<int>::iterator it;
  for (it = pEulerCircuit->begin(); it != pEulerCircuit->end(); ) {
    if (vertSet.find(*it) == vertSet.end()) {
      vertSet.insert(*it);
      ++it;
    } else {  // Remove redundancy
      it = pEulerCircuit->erase(it);
    }
  }

  printEulerCircuitLs(pEulerCircuit);
}

void MatrixGraph::printEulerCircuitLs(list<int>* pEulerCirLs) const {
  // Output found euler circuit
  cout << "List size= " << pEulerCirLs->size() << endl;
  list<int>::iterator iter;
  for (iter = pEulerCirLs->begin(); iter != pEulerCirLs->end(); ++iter)
    cout << *iter << " ";
  cout << endl;
}

void MatrixGraph::dumpDisGraph() const {
  for (int i = 0; i < numVertices_; ++i) {
    for (int j = 0; j < numVertices_; ++j) {
      cout << setw(EDGEDISTANCEWIDTH) << edgeMatrix_[i][j] << ' ';
    }
    cout << endl;
  }
}

void MatrixGraph::dumpEulerDFSGraph() const {
  cout << "Number of Edges: " << numEdges_ << endl << "    ";
  for (int i = 0; i < numVertices_; ++i)
    cout << setw(EULERDFSEDGEWIDTH) << i << ' ';
  cout << endl << "    ";
  for (int i = 0; i < numVertices_; ++i)
    cout << setw(EULERDFSEDGEWIDTH) << "--" << ' ';
  cout << endl;

  for (int i = 0; i < numVertices_; ++i) {
    cout << "D(" << vertexDegrees_[i] << ')';
    for (int j = 0; j < numVertices_; ++j) {
      cout << setw(EULERDFSEDGEWIDTH) << eulerDFSMatrix_[i][j] << ' ';
    }
    cout << endl;
  }
}
