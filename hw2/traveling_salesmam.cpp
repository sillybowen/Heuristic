#include <iostream>
#include <fstream>
#include <vector>
#include "matching.h"
#include "matrix_graph.h"
using namespace std;

#define MAXNUMOFPOINTS 1000

vector<point> p;
void init() {
  ifstream inputfile;
  // Don't add default "point"s if there are NOT so many cities
  p.reserve(MAXNUMOFPOINTS);
  inputfile.open("input_travel");
  for (int i = 0; i<MAXNUMOFPOINTS; i++) {
    int x;
    point aCity;
    if(!(inputfile>>aCity.id_at_input>>aCity.x>>aCity.y>>aCity.z))
      break;
    aCity.id_at_main = i;
    p.push_back(aCity);
  }

  cout << "p.size() = " << p.size() << endl;
  for (int i = 0; i < p.size(); ++i) {
    cout << "p element = " << p[i].id_at_input << " " << p[i].x
      << " " << p[i].y << " " << p[i].z << endl;
  }
  inputfile.close();
}

void work() {
  Mst m_(p);
  m_.work();
  Matching ma_(m_.giveOddDegreePoints());
  ma_.work2();
  m_.outCost();
  ma_.outCost();
  vector<edge> edgesSet1 = m_.givenAnsEdges();
  vector<edge> edgesSet2 = ma_.givenAnsEdges();
  m_.output();
  ma_.output();
  cout << "Output MST size: " << (int) edgesSet1.size() << endl;
  cout << "Output matching size: " << (int) edgesSet2.size() << endl;
  int numOfTotalEdges = (int) p.size();
  MatrixGraph myEuler(numOfTotalEdges, edgesSet1, edgesSet2);
  myEuler.dumpEulerDFSGraph();
}

void outit() {
}

int main() {
  init();
  work();
  outit();
}
