#include <cstring>
#include "aco_graphs.h"

ACOGraphs::ACOGraphs(int numVertices, const vector<HospitalPatient*>& hosPatVect)
  : numOfVertices_(numVertices) {
  for (int i = 0; i < numVertices; ++i) {
    matrixGraph_.push_back(new EdgeInfo[numVertices]);
  }

  // Fill in EdgeInfo matrix
  for (int i = 0; i < numVertices; ++i)
    matrixGraph_[i][i].distance = -1;  // Self loop is invalid
  for (int i = 0; i < numVertices; ++i) {
    for (int j = i + 1; j < numVertices; ++j) {
      matrixGraph_[i][j].pheromones = 0;
      matrixGraph_[i][j].distance = hosPatVect[i]->distance(hosPatVect[j]);
      matrixGraph_[j][i].pheromones = 0;
      matrixGraph_[j][i].distance = matrixGraph_[i][j].distance;
    }
  }

  // Fill in adjacent graph
  for (int i = 0; i < numVertices; ++i) {
    adjVertList_.push_back(new EdgesMultiSet);
    for (int j = 0; j < numVertices; ++j) {
      if (hosPatVect[j]->isPatient()) {
        adjVertList_[i]->insert(j);
      }
    }
  }
}

ACOGraphs::~ACOGraphs() {
  for (int i = 0; i < numOfVertices_; ++i) {
    delete adjVertList_[i];
    delete [] matrixGraph_[i];
  }
}
