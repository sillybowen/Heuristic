#include <climits>
#include <cstring>
#include "aco_graphs.h"

ACOGraphs::ACOGraphs(int numVertices, int firHosInd,
    const vector<HospitalPatient*>& hosPatVect) : numOfVertices_(numVertices),
                                                  fir_hos_ind_(firHosInd),
                                                  hosPatVect_(hosPatVect) {
  for (int i = 0; i < numVertices; ++i) {
    matrixGraph_.push_back(new EdgeInfo[numVertices]);
  }

  // Fill in EdgeInfo matrix
  for (int i = 0; i < numVertices; ++i) {
    // Self loop is invalid
    matrixGraph_[i][i].pheromones = -1;
    matrixGraph_[i][i].distance = -1;
  }
  for (int i = 0; i < numVertices; ++i) {
    int nearestHosId = 0;
    int nearestHosDis = INT_MAX;
    for (int j = i + 1; j < numVertices; ++j) {
      matrixGraph_[i][j].pheromones = 0;
      matrixGraph_[i][j].distance = hosPatVect[i]->distance(hosPatVect[j]);
      matrixGraph_[j][i].pheromones = 0;
      matrixGraph_[j][i].distance = matrixGraph_[i][j].distance;
      if ((hosPatVect_[i]->isPatient()) && !(hosPatVect_[j]->isPatient())
          && matrixGraph_[i][j].distance < nearestHosDis) {
        nearestHosDis = matrixGraph_[i][j].distance;
        nearestHosId = j;
      }
    }

    if (hosPatVect_[i]->isPatient())
      hosPatVect_[i]->setNearestHospital(nearestHosId, nearestHosDis);
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

void ACOGraphs::outputAdjG() const {
  for (int i = 0; i < numOfVertices_; ++i) {
    cout << "Vert(" << i << '|' << ((hosPatVect_[i]->isPatient())? 'P':'H')
      << "): ";
    EdgesConstIter cit = adjVertList_[i]->begin();
    while (cit != adjVertList_[i]->end()) {
      cout << *cit << " ";
      ++cit;
    }
    cout << endl;
  }
}

void ACOGraphs::outputMatrixG() const {
  for (int i = 0; i < numOfVertices_; ++i) {
    if (hosPatVect_[i]->isPatient())
      cout << 'P' << i << "  ";
    else
      cout << 'H' << i - fir_hos_ind_ << "  ";
  }
  cout << endl;

  for (int i = 0; i < numOfVertices_; ++i) {
    for (int j = 0; j < numOfVertices_; ++j) {
      cout << matrixGraph_[i][j].pheromones << '|' << matrixGraph_[i][j].distance
        << "  ";
    }
    cout << "NearHos " << hosPatVect_[i]->getNearestHospitalID() << '|'
      << hosPatVect_[i]->getNearestHospitalDis() << endl;
  }
}
