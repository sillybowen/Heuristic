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
    inputfile>>aCity.id_at_input>>aCity.x>>aCity.y>>aCity.z;
    if (inputfile.eof())
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
  ma_.work();

  vector<edge> edgesSet1 = m_.givenAnsEdges();
  vector<edge> edgesSet2 = ma_.givenAnsEdges();
  cout << "Output MST size: " << (int) edgesSet1.size() << endl;
  cout << "Output matching size: " << (int) edgesSet2.size() << endl;

  int numOfTotalEdges = (int) p.size();

  MatrixGraph myEuler(numOfTotalEdges, edgesSet1, edgesSet2);
  myEuler.dumpEulerDFSGraph();

//   Matching ma_(ps);
//  ma_.work();

//  euler();

/*  vector<point> ps;
  point p; 
  p.set(0,0,1,1,11);
  point p2;
  p2.set(1,1,1,1,1);
  point p3;
  p3.set(2,2,1,1,2);
  point p4;
  p4.set(3,3,101,101,101);
  point p5;
  p5.set(4,4,101,101,102);
  point p6;
  p6.set(5,5,101,101,103);
  ps.push_back(p);
  ps.push_back(p2);
  ps.push_back(p3);
  ps.push_back(p4);
  ps.push_back(p5);
  ps.push_back(p6);
  Matching ma_(ps);
  ma_.work();
*/

}

void outit() {
}

int main() {
  init();
  work();
  outit();
}
