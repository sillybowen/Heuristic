#include <fstream>
#include <cassert>
#include <cstring>
#include <set>
#include "matrix_graph.h"

MatrixGraph::MatrixGraph(int numVertices, const vector<edge>& mstEdges,
    const vector<edge>& matchEdges) : numVertices_(numVertices) {
  edgeMatrix_ = new double*[numVertices_];
  eulerDFSMatrix_ = new unordered_multiset<int>*[numVertices_];
  vertexDegrees_ = new int[numVertices_];

  for (int i = 0; i < numVertices_; ++i) {
    vertexDegrees_[i] = 0;
    edgeMatrix_[i] = new double[numVertices_];
    eulerDFSMatrix_[i] = new unordered_multiset<int>;
    for (int j = 0; j < numVertices_; ++j) {
      edgeMatrix_[i][j] = 0.0;
    }
  }

  const int mstEdgeSize = mstEdges.size();
  const int matchEdgeSize = matchEdges.size();
  for (int i = 0; i < mstEdgeSize; ++i) {
    vertexDegrees_[mstEdges[i].p[0]] += 1;
    vertexDegrees_[mstEdges[i].p[1]] += 1;
    edgeMatrix_[mstEdges[i].p[0]][mstEdges[i].p[1]] = mstEdges[i].dis;
    eulerDFSMatrix_[mstEdges[i].p[0]]->insert(mstEdges[i].p[1]);
    // Copy for the other direction: p[1] -> p[0]
    edgeMatrix_[mstEdges[i].p[1]][mstEdges[i].p[0]] = mstEdges[i].dis;
    eulerDFSMatrix_[mstEdges[i].p[1]]->insert(mstEdges[i].p[0]);
  }
  for (int i = 0; i < matchEdgeSize; ++i) {
    vertexDegrees_[matchEdges[i].p[0]] += 1;
    vertexDegrees_[matchEdges[i].p[1]] += 1;
    edgeMatrix_[matchEdges[i].p[0]][matchEdges[i].p[1]] = matchEdges[i].dis;
    eulerDFSMatrix_[matchEdges[i].p[0]]->insert(matchEdges[i].p[1]);
    // Copy for the other direction: p[1] -> p[0]
    edgeMatrix_[matchEdges[i].p[1]][matchEdges[i].p[0]] = matchEdges[i].dis;
    eulerDFSMatrix_[matchEdges[i].p[1]]->insert(matchEdges[i].p[0]);
  }

  numEdges_ = mstEdgeSize + matchEdgeSize;
}

MatrixGraph::~MatrixGraph() {
  delete [] vertexDegrees_;

  for (int i = 0; i < numVertices_; ++i) {
    delete [] edgeMatrix_[i];
    delete eulerDFSMatrix_[i];
  }
  delete [] edgeMatrix_;
  delete [] eulerDFSMatrix_;
}

// Assumption: vertices are named from 0 to (numVertices_ - 1)
void MatrixGraph::ifEulerianGraph() const {
  for (int i = 0; i < numVertices_; ++i) {
    if (vertexDegrees_[i] != (int) eulerDFSMatrix_[i]->size()) {
      cout << "For V:" << i << " vertexDeg: " << vertexDegrees_[i] 
        << "; unorderSet: " << eulerDFSMatrix_[i]->size() << endl;
      assert(0);
    }
    assert(vertexDegrees_[i] > 0);
    if (vertexDegrees_[i] % 2 != 0) {
      cout << "vertex i= " << i << "; degree= " << vertexDegrees_[i] << endl;
      assert(!(vertexDegrees_[i] % 2));
    }
  }
}

void MatrixGraph::eulerCircuitDFS(list<int>& toSpliceLs, int startVertex) {
  toSpliceLs.push_back(startVertex);
  unordered_multiset<int>::iterator it = eulerDFSMatrix_[startVertex]->begin();

  if (it == eulerDFSMatrix_[startVertex]->end()) {  // All edges are used
    return;
  } else {
    int i = *it;  // Go for edge: startVertex <-> i
    // Remove this used edge
    eulerDFSMatrix_[startVertex]->erase(it);
    // Only remove one element with key == startVertex, using iterator
    it = eulerDFSMatrix_[i]->find(startVertex);
    eulerDFSMatrix_[i]->erase(it);
    --vertexDegrees_[i];
    --vertexDegrees_[startVertex];
    --numEdges_;
    eulerCircuitDFS(toSpliceLs, i);
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
  cout << "Eulerian Circuit Result: ";
  printEulerCircuitLs(pEulerCirLs);

  return pEulerCirLs;
}
vector<int> MatrixGraph::giveResult() {
  return ans_seq;
}
void MatrixGraph::trimEulerCircuitToTSP(list<int>* pEulerCircuit) {
  int startVert = pEulerCircuit->front();
  set<int> vertSet;
  list<int>::iterator it;
  for (it = pEulerCircuit->begin(); it != pEulerCircuit->end(); ) {
    if (vertSet.find(*it) == vertSet.end()) {
      vertSet.insert(*it);
      ans_seq.push_back((*it));
      //      cout<<"ans_seq"<<ans_seq.size()<<endl;
      //      cout<<ans_seq[ans_seq.size()-1]<<endl;
      ++it;
    } else {  // Remove redundancy
      it = pEulerCircuit->erase(it);
    }
  }
  // Pop in the final return back vertex (the original entry point)
  pEulerCircuit->push_back(startVert);
  ans_seq.push_back(startVert);
  // printEulerCircuitLs(pEulerCircuit);
}

double MatrixGraph::sumTSPDistance(const list<int>* pTSPTrip,
    vector<point>& points) const {
  int vertCountArr[numVertices_];
  double totalTSPLen = 0.0;
  memset(vertCountArr, 0, sizeof(int) * numVertices_);

  list<int>::const_iterator cit = pTSPTrip->begin(), pcit;
  // vertCountArr[*cit] += 1;  // Don't count startVertex twice
  pcit = cit;
  ++cit;
  for (; cit != pTSPTrip->end(); ++cit) {
    vertCountArr[*cit] += 1;
    double new_edge_dis = points[*cit].dis(points[*pcit]);
    //    cout << "Added Edge: " << *cit << "-" << *pcit << " Dis: "
    //      << new_edge_dis << endl;
    totalTSPLen += new_edge_dis;
    pcit = cit;
  }

  for (int i = 0; i < numVertices_; ++i) {
    if (vertCountArr[i] != 1) {
      //      cout << "Vertex: " << i << "; occured: " << vertCountArr[i] << " times!!!\n";
      return -1.0;
    }
  }

  return totalTSPLen;
}

void MatrixGraph::printEulerCircuitLs(list<int>* pEulerCirLs) const {
  // Output found euler circuit
  cout << "List size= " << pEulerCirLs->size() << endl;
  list<int>::iterator iter;
  int i = 0;
  //  for (iter = pEulerCirLs->begin(); iter != pEulerCirLs->end(); ++iter)
    //    cout << i++<<' '<<*iter << endl;
  //  cout << endl;
}

void MatrixGraph::printOutputTSPTrip(const list<int>* pTSPTrip,
    const char* outFileN) const {
  ofstream outF;
  outF.open(outFileN);
  assert(outF.is_open());

  list<int>::const_iterator cit;
  for (cit = pTSPTrip->begin(); cit != pTSPTrip->end(); ++cit) {
    // Plus 1 to make sure outputing "In-File" city names
    outF << (*cit + 1) << endl;
  }

  outF.close();
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
      if (eulerDFSMatrix_[i]->find(j) != eulerDFSMatrix_[i]->end())
        cout << setw(EULERDFSEDGEWIDTH) << '1' << ' ';
      else
        cout << setw(EULERDFSEDGEWIDTH) << '0' << ' ';
    }
    cout << endl;
  }
}
