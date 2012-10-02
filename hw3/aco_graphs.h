#ifndef ACO_Graphs_Header_
#define ACO_Graphs_Header_

#include <vector>
#include <unordered_set>
#include "hospital_patient.h"

using std::vector;
using std::unordered_multiset;

class ACOGraphs {
public:
  ACOGraphs(int numVertices, const vector<HospitalPatient*>& hosPatVect);
  ~ACOGraphs();

private:
  const int numOfVertices_;
  typedef unordered_multiset<int> EdgesMultiSet;
  struct EdgeInfo {
    int pheromones;
    int distance;  // |x1-x2| + |y1-y2|
  };

  vector<EdgesMultiSet*> adjVertList_;
  vector<EdgeInfo*>      matrixGraph_;
};

#endif
